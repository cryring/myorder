#-------------------------------------------------
#
# Project created by QtCreator 2017-06-02T14:56:04
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ordershark
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        dialog.cpp \
    invoicemanagedialog.cpp \
    newinvoicedialog.cpp \
    viewinvoicedialog.cpp \
    csvloader.cpp \
    orderdetaillist.cpp \
    taobaoexportorderlist.cpp \
    taobaoexportorderdetaillist.cpp \
    orderstore.cpp \
    ordermanagedialog.cpp \
    orderimportdialog.cpp \
    utils.cpp \
    invoice.cpp \
    orders.cpp \
    invoices.cpp \
    store.cpp \
    goodsstore.cpp \
    commonstore.cpp \
    goodsnamestore.cpp \
    vieworderdialog.cpp \
    shopnamestore.cpp \
    shopmanagedialog.cpp \
    goodsnamemanagedialog.cpp \
    settledialog.cpp \
    profitstore.cpp \
    dboperation.cpp

HEADERS += \
        dialog.h \
    invoicemanagedialog.h \
    newinvoicedialog.h \
    viewinvoicedialog.h \
    csvloader.h \
    orderdefine.h \
    orderdetaillist.h \
    taobaoexportorderlist.h \
    taobaoexportorderdetaillist.h \
    csvprocessor.h \
    orderstore.h \
    ordermanagedialog.h \
    orderimportdialog.h \
    singleton.h \
    utils.h \
    invoice.h \
    commondefine.h \
    initdb.h \
    orders.h \
    invoices.h \
    store.h \
    goodsstore.h \
    goodsdefine.h \
    commonstore.h \
    goodsnamestore.h \
    vieworderdialog.h \
    shopnamestore.h \
    shopmanagedialog.h \
    goodsnamemanagedialog.h \
    settledialog.h \
    profitstore.h \
    dboperation.h

FORMS += \
        dialog.ui \
    invoicemanagedialog.ui \
    newinvoicedialog.ui \
    viewinvoicedialog.ui \
    ordermanagedialog.ui \
    orderimportdialog.ui \
    vieworderdialog.ui \
    shopmanagedialog.ui \
    goodsnamemanagedialog.ui \
    settledialog.ui

RESOURCES += \
    res/rc.qrc
