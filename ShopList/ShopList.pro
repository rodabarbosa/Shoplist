#-------------------------------------------------
#
# Project created by QtCreator 2017-05-06T09:06:30
#
#-------------------------------------------------

QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ShopList
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dbconnection.cpp \
    listwindow.cpp \
    brandentrywindow.cpp \
    categoryentrywindow.cpp \
    productentrywindow.cpp \
    supermarketentrywindow.cpp \
    pricewindow.cpp \
    priceentrywindow.cpp \
    commondata.cpp

HEADERS  += mainwindow.h \
    dbconnection.h \
    listwindow.h \
    brandentrywindow.h \
    categoryentrywindow.h \
    productentrywindow.h \
    supermarketentrywindow.h \
    pricewindow.h \
    priceentrywindow.h \
    commondata.h

FORMS    += mainwindow.ui \
    listwindow.ui \
    brandentrywindow.ui \
    categoryentrywindow.ui \
    productentrywindow.ui \
    supermarketentrywindow.ui \
    pricewindow.ui \
    priceentrywindow.ui

RESOURCES += \
    shoplistres.qrc
