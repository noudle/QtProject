#include <QtGui>
 
#include <iostream>
 
#include "form.h"
 
Form::Form(QWidget *parent)
    : QWidget(parent)
{
  setupUi(this);
 
  QStandardItemModel* model = new QStandardItemModel;
 

  QList<QStandardItem*> items;

  QStandardItem item0("test");
  items.insert(0, &item0);

  QStandardItem item1("test2");
  items.insert(1, &item1);

  //----------------------

  QList<QStandardItem*> items2;
  QStandardItem item3("V1");
  items2.insert(1, &item3);

  QStandardItem item4("V2");
  items2.insert(2, &item4);

  model->appendRow(items);
  model->appendColumn(items2);
 
  this->tableView->setModel(model);
}
