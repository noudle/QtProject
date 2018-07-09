QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = test

TEMPLATE = app

INCLUDEPATH += . \

SOURCES += \
			form.cpp \
			main.cpp

HEADERS  += \
			form.h \

FORMS    += \
            form.ui  