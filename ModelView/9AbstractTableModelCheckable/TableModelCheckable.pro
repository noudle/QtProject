QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = test

TEMPLATE = app

INCLUDEPATH += . \

SOURCES += \
			MyTableModel.cpp \
			form.cpp \
			main.cpp

HEADERS  += \
			form.h \
			MyTableModel.h

FORMS    += \
            form.ui  