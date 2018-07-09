QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = test

TEMPLATE = app

INCLUDEPATH += . \

SOURCES += \
			demo.cpp \
			main.cpp

HEADERS  += \
			main.h \

FORMS    += \
            main.ui  