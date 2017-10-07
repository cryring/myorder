#include <QDebug>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QUuid>
#include "goodsimportdialog.h"
#include "ui_goodsimportdialog.h"
#include "goodsdefine.h"
#include "shopnamestore.h"
#include "goodsnamestore.h"
#include "store.h"

GoodsImportDialog::GoodsImportDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::GoodsImportDialog)
{
    ui->setupUi(this);

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
        QMessageBox::warning(this, tr("order"), tr("please input shopname."));
        return;
    }

    QString count = ui->goodsCountEdit->text();
    if ("" == count)
    {
        QMessageBox::warning(this, tr("order"), tr("please input goods count."));
        return;
    }

    QString price = ui->goodsPriceEdit->text();
    if ("" == price)
    {
        QMessageBox::warning(this, tr("order"), tr("please input goods price."));
        return;
    }

    QString id = ui->goodsIdEdit->text();
    if ("" == id)
    {
        QMessageBox::warning(this, tr("order"), tr("please input goods id."));
        return;
    }

    QString name = ui->goodsNameEdit->text();
    if ("" == name)
    {
        QMessageBox::warning(this, tr("order"), tr("please input goods name."));
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
        QMessageBox::warning(this, tr("order"), tr("calc total price failed, please check the input."));
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
        goods.price = QString::number(m_goods[i]->price.toFloat() / paperTotalPrice * totalPrice);
        goods.count = m_goods[i]->count;
        goods.attribute = m_goods[i]->attribute;
        Store::instance()->insertGoods(&goods);
    }
    m_date = date;
    m_invoiceId = invoiceId;
}

void GoodsImportDialog::init()
{
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

    }
}

float GoodsImportDialog::calcTotalPrice()
{
    if (false == checkNeededInput())
    {
        return -1;
    }

    float p1 = m_coupon.toFloat() * (m_couponDiscount.toFloat() / 100) / m_cExchangeRate.toFloat();
    float p2 = (m_payPrice.toFloat() - m_change.toFloat()) / m_exchangeRate.toFloat();
    float p3 = m_creditCard.toFloat();
    return p1 + p2 + p3;
}

bool GoodsImportDialog::checkNeededInput()
{
    m_payPrice = ui->payPriceEdit->text();
    m_exchangeRate = ui->exchangeRateEdit->text();
    m_change = ui->changeEdit->text();
    m_coupon = ui->couponEdit->text();
    m_couponDiscount = ui->couponDiscountEdit->text();
    m_cExchangeRate = ui->cExchangeRateEdit->text();
    m_creditCard = ui->creditCardEdit->text();

    if (m_payPrice.isEmpty() ||
        m_exchangeRate.isEmpty() ||
        m_change.isEmpty() ||
        m_coupon.isEmpty() ||
        m_couponDiscount.isEmpty() ||
        m_cExchangeRate.isEmpty() ||
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
        QMessageBox::warning(this, tr("order"), tr("calc total price failed, please check the input."));
        return;
    }

    QAbstractItemModel* model = ui->invoiceView->model();
    for (int i = 0; i < m_goods.size(); ++i)
    {
        Goods* goods = m_goods[i];
        float price = goods->price.toFloat() / paperTotalPrice * totalPrice;
        goods->price = QString::number(price);


        QModelIndex index = model->index(i, 2);
        model->setData(index, QVariant(goods->price));
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
