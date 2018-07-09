QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = test

TEMPLATE = app

INCLUDEPATH += . \

SOURCES += \
			layeritemdelegate.cpp \
			layertablemodel.cpp \
			layertableview.cpp \
			main.cpp

HEADERS  += \
			layeritemdelegate.h \
			layertablemodel.h \
			layertableview.h
			

FORMS    += \ 