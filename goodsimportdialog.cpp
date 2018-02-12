#include <QDebug>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QUuid>
#include "goodsimportdialog.h"
#include "ui_goodsimportdialog.h"
#include "goodsdefine.h"
#include "shopnamestore.h"
#include "goodsnamestore.h"
#include "invoicestore.h"
#include "store.h"

GoodsImportDialog::GoodsImportDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::GoodsImportDialog)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":icon/main.ico"));
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);

    init();
}

GoodsImportDialog::GoodsImportDialog(const QString& invoiceid, const QString& date, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::GoodsImportDialog),
    m_date(date),
    m_invoiceId(invoiceid)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":icon/main.ico"));
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);

    init();
}

GoodsImportDialog::~GoodsImportDialog()
{
    delete ui;

    for (int i = 0; i < m_goods.size(); ++i)
    {
        delete m_goods[i];
        m_goods[i] = NULL;
    }
    m_goods.clear();
}

void GoodsImportDialog::setInvoiceID(const QString& id)
{
    m_invoiceId = id;
}

void GoodsImportDialog::setDate(const QString& date)
{
    m_date = date;
}

void GoodsImportDialog::on_saveGoodsButton_clicked()
{
    QString shopName = ui->shopNameBox->currentText();
    if ("" == shopName)
    {
        QMessageBox::warning(this, tr("order"), tr("清选择商场名称."));
        return;
    }

    QString count = ui->goodsCountEdit->text();
    if ("" == count || "0" == count)
    {
        QMessageBox::warning(this, tr("order"), tr("请输入正确的商品数量."));
        return;
    }

    QString price = ui->goodsPriceEdit->text();
    if ("" == price || "0" == price)
    {
        QMessageBox::warning(this, tr("order"), tr("请输入正确的商品价格."));
        return;
    }

    QString id = ui->goodsIdEdit->text();
    if ("" == id || 3 >= id.length())
    {
        QMessageBox::warning(this, tr("order"), tr("请输入正确的商品ID."));
        return;
    }

    QString name = ui->goodsNameEdit->text();
    if ("" == name)
    {
        QMessageBox::warning(this, tr("order"), tr("请输入正确的商品名称."));
        return;
    }

    QString attr = ui->attributeEdit->text();

    QStandardItemModel* model = (QStandardItemModel*)ui->invoiceView->model();
    if (NULL == model)
    {
        QMessageBox::warning(this, tr("order"), tr("wtf, view model is null."));
        return;
    }

    int ncnt = count.toInt();
    float fprice = price.toFloat() / ncnt;
    for (int i = 0; i < ncnt; i++)
    {
        int col = 0;
        int row = model->rowCount();
        model->setItem(row, col++, new QStandardItem(name));
        model->setItem(row, col++, new QStandardItem(QString::number(fprice)));
        model->setItem(row, col++, new QStandardItem(""));
        model->setItem(row, col++, new QStandardItem(attr));
        model->setItem(row, col++, new QStandardItem("1"));

        Goods* goods = new Goods();
        goods->name = name;
        goods->price = QString::number(fprice);
        goods->attribute = attr;
        goods->count = "1";
        m_goods.append(goods);
    }
}

void GoodsImportDialog::on_delGoodsButton_clicked()
{
    int row = ui->invoiceView->currentIndex().row();
    if (0 <= row && m_goods.size() > row)
    {
        m_goods.remove(row);
        QStandardItemModel* model = (QStandardItemModel*)ui->invoiceView->model();
        model->removeRow(row);
    }
}

void GoodsImportDialog::on_saveButton_clicked()
{
    float paperTotalPrice = 0;
    for (int i = 0; i < m_goods.size(); ++i)
    {
        paperTotalPrice += m_goods[i]->price.toFloat();
    }
    float totalPrice = calcTotalPrice();
    if (totalPrice <= 0)
    {
        QMessageBox::warning(this, tr("order"), tr("价格计算异常，请检查输入."));
        return;
    }

    // remove old goods first
    Store::instance()->removeGoodsByInvoiceID(m_date, m_invoiceId);

    QString shopName = ui->shopNameBox->currentText();
    QString date = ui->dateEdit->date().toString("yyyyMMdd");
    QString invoiceId = createID(date);
    for (int i = 0; i < m_goods.size(); ++i)
    {
        Goods goods;
        goods.id = createID(date);
        goods.invoiceid = invoiceId;
        goods.date = date;
        goods.shopName = shopName;
        goods.name = m_goods[i]->name;
        goods.price = m_goods[i]->price;
        goods.realPrice = m_goods[i]->realPrice;
        if (goods.realPrice.isEmpty())
        {
            QMessageBox::warning(this, tr("order"), tr("价格计算异常，请检查输入."));
            return;
        }
        goods.count = m_goods[i]->count;
        goods.attribute = m_goods[i]->attribute;
        Store::instance()->insertGoods(&goods);
    }
    m_date = date;
    m_invoiceId = invoiceId;

    InvoiceBase ib;
    ib.id = invoiceId;
    ib.cash = m_cash;
    ib.cashRate = m_cashRate;
    ib.cashChange = m_cashChange;
    ib.coupon = m_coupon;
    ib.couponRate = m_couponRate;
    ib.couponDiscount = m_couponDiscount;
    ib.creditCard = m_creditCard;
    ib.shopName = ui->shopNameBox->currentText();
    ib.date = m_date;
    qDebug() << ib.shopName;
    InvoiceStore::instance()->set(&ib);

    accept();
}

void GoodsImportDialog::init()
{
    setFixedSize(this->width(), this->height());
    ui->invoiceView->verticalHeader()->hide();
    ui->invoiceView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->invoiceView->setSelectionBehavior(QAbstractItemView::SelectRows);

    QDoubleValidator* aDoubleValidator = new QDoubleValidator(0.00,10000000.00,2);
    aDoubleValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->couponEdit->setValidator(aDoubleValidator);
    ui->couponEdit->setText("0");

    aDoubleValidator = new QDoubleValidator(0.00,100.00,2);
    ui->couponDiscountEdit->setValidator(aDoubleValidator);
    ui->couponDiscountEdit->setText("100");

    aDoubleValidator = new QDoubleValidator(0.00,100000.00,2);
    ui->cExchangeRateEdit->setValidator(aDoubleValidator);
    ui->cExchangeRateEdit->setText("1");

    aDoubleValidator = new QDoubleValidator(0.00,10000000.00,2);
    aDoubleValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->goodsPriceEdit->setValidator(aDoubleValidator);
    ui->goodsPriceEdit->setText("0");

    aDoubleValidator = new QDoubleValidator(0.00,10000000.00,2);
    aDoubleValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->payPriceEdit->setValidator(aDoubleValidator);
    ui->payPriceEdit->setText("0");

    aDoubleValidator = new QDoubleValidator(0.00,100000.00,2);
    aDoubleValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->exchangeRateEdit->setValidator(aDoubleValidator);
    ui->exchangeRateEdit->setText("1");

    aDoubleValidator = new QDoubleValidator(0.00,100000.00,2);
    aDoubleValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->changeEdit->setValidator(aDoubleValidator);
    ui->changeEdit->setText("0");

    aDoubleValidator = new QDoubleValidator(0.00,100000.00,2);
    aDoubleValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->creditCardEdit->setValidator(aDoubleValidator);
    ui->creditCardEdit->setText("0");

    QIntValidator* aIntValidator = new QIntValidator;
    aIntValidator->setRange(1, 100);
    ui->goodsCountEdit->setValidator(aIntValidator);
    ui->goodsCountEdit->setText("1");

    const QVector<QString>& shopNames = ShopNameStore::instance()->getNames();
    for (int i = 0; i < shopNames.size(); ++i)
    {
        ui->shopNameBox->addItem(shopNames[i]);
    }

    QStandardItemModel* model = new QStandardItemModel(0, 4);
    ui->invoiceView->setModel(model);
    int col = 0;
    model->setHeaderData(col++, Qt::Horizontal, tr("商品名"));
    model->setHeaderData(col++, Qt::Horizontal, tr("外币价格"));
    model->setHeaderData(col++, Qt::Horizontal, tr("实际价格"));
    model->setHeaderData(col++, Qt::Horizontal, tr("属性"));
    model->setHeaderData(col++, Qt::Horizontal, tr("数量"));

    // init invoice
    if (false == m_invoiceId.isEmpty() && false == m_date.isEmpty())
    {
        // init invoice base information
        InvoiceBase* ib = InvoiceStore::instance()->get(m_invoiceId);
        if (ib == NULL)
        {
            QMessageBox::warning(this, tr("order"), tr("获取小票异常."));
            return;
        }

        m_cash = ib->cash;
        ui->payPriceEdit->setText(m_cash);

        m_cashChange = ib->cashChange;
        ui->changeEdit->setText(m_cashChange);

        m_cashRate = ib->cashRate;
        ui->exchangeRateEdit->setText(m_cashRate);

        m_coupon = ib->coupon;
        ui->couponEdit->setText(m_coupon);

        m_couponDiscount = ib->couponDiscount;
        ui->couponDiscountEdit->setText(m_couponDiscount);

        m_couponRate = ib->couponRate;
        ui->cExchangeRateEdit->setText(m_couponRate);

        m_creditCard = ib->creditCard;
        ui->creditCardEdit->setText(m_creditCard);

        ui->shopNameBox->setCurrentText(ib->shopName);
        ui->dateEdit->setDate(QDate::fromString(ib->date, "yyyyMMdd"));

        Store::instance()->getGoodsByInvoice(m_invoiceId, m_goods);
        for (int i = 0; i < m_goods.size(); i++)
        {
            Goods* g = m_goods[i];
            int col = 0;
            int row = model->rowCount();
            model->setItem(row, col++, new QStandardItem(g->name));
            model->setItem(row, col++, new QStandardItem(g->price));
            model->setItem(row, col++, new QStandardItem(g->realPrice));
            model->setItem(row, col++, new QStandardItem(g->attribute));
            model->setItem(row, col++, new QStandardItem("1"));
        }
    }
}

float GoodsImportDialog::calcTotalPrice()
{
    if (false == checkNeededInput())
    {
        return -1;
    }

    float p1 = m_coupon.toFloat() * (m_couponDiscount.toFloat() / 100) / m_couponRate.toFloat();
    float p2 = (m_cash.toFloat() - m_cashChange.toFloat()) / m_cashRate.toFloat();
    float p3 = m_creditCard.toFloat();
    return p1 + p2 + p3;
}

bool GoodsImportDialog::checkNeededInput()
{
    m_cash = ui->payPriceEdit->text();
    m_cashRate = ui->exchangeRateEdit->text();
    m_cashChange = ui->changeEdit->text();
    m_coupon = ui->couponEdit->text();
    m_couponDiscount = ui->couponDiscountEdit->text();
    m_couponRate = ui->cExchangeRateEdit->text();
    m_creditCard = ui->creditCardEdit->text();

    if (m_cash.isEmpty() ||
        m_cashRate.isEmpty() ||
        m_cashChange.isEmpty() ||
        m_coupon.isEmpty() ||
        m_couponDiscount.isEmpty() ||
        m_couponRate.isEmpty() ||
        m_creditCard.isEmpty())
    {
        return false;
    }

    return true;
}

void GoodsImportDialog::on_calcButton_clicked()
{
    float paperTotalPrice = 0;
    for (int i = 0; i < m_goods.size(); ++i)
    {
        paperTotalPrice += m_goods[i]->price.toFloat();
    }

    float totalPrice = calcTotalPrice();
    if (totalPrice <= 0)
    {
        QMessageBox::warning(this, tr("order"), tr("价格计算异常，请检查输入."));
        return;
    }

    QAbstractItemModel* model = ui->invoiceView->model();
    for (int i = 0; i < m_goods.size(); ++i)
    {
        Goods* goods = m_goods[i];
        float price = goods->price.toFloat() / paperTotalPrice * totalPrice;
        goods->realPrice = QString::number(price);

        QModelIndex index = model->index(i, 2);
        model->setData(index, QVariant(goods->realPrice));
    }
}

QString GoodsImportDialog::createID(const QString& date)
{
    QString uuid = QUuid::createUuid().toString();
    uuid.replace("{", "");
    uuid.replace("}", "");
    uuid.replace("-", "");
    return date + "_" + uuid;
}


void GoodsImportDialog::on_goodsIdEdit_textChanged(const QString &id)
{
    qDebug() << id;
    if (id.length() <= 3)
    {
        ui->goodsNameEdit->setText("");
    }
    else
    {
        GoodsName* gn = GoodsNameStore::instance()->get(id);
        if (gn != NULL)
        {
            QString name = gn->brand + " " + gn->name;
            ui->goodsNameEdit->setText(name);
        }
        else
        {
            ui->goodsNameEdit->setText("");
        }
    }
}

void GoodsImportDialog::on_cancelButton_clicked()
{
    accept();
}
