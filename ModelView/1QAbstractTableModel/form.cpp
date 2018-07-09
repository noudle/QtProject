#include <QAbstractTableModel>
#include <QtGui>
 
#include <iostream>
#include <sstream>
 
#include "form.h"
 
 
class MyTableModel : public QAbstractTableModel
{
public:
  MyTableModel();
 
  int rowCount(const QModelIndex& parent) const;
  int columnCount(const QModelIndex& parent) const;
  QVariant data(const QModelIndex& index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
 
protected:
  std::vector<std::vector<float> > Columns;
};
 
Form::Form(QWidget *parent)
    : QWidget(parent)
{
  setupUi(this);
 
  MyTableModel* model = new MyTableModel;
  this->tableView->setModel(model);
 
  this->tableView->resizeColumnsToContents();
 
}
 
MyTableModel::MyTableModel() : QAbstractTableModel()
{
  std::vector<float> column1;
  column1.push_back(10);
  column1.push_back(20);
  column1.push_back(30);
  column1.push_back(40);
 
  Columns.push_back(column1);
 
  std::vector<float> column2;
  column2.push_back(50);
  column2.push_back(60);
  column2.push_back(70);
  column2.push_back(80);
 
  Columns.push_back(column2);
}
 
int MyTableModel::rowCount(const QModelIndex& parent) const
{
  return Columns[0].size();
}
 
int MyTableModel::columnCount(const QModelIndex& parent) const
{
  return Columns.size();
}
 
QVariant MyTableModel::data(const QModelIndex& index, int role) const
{
  if(role == Qt::DisplayRole)
    {
    return Columns[index.column()][index.row()];
    }
  return QVariant::Invalid;
}
 
QVariant MyTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
 
  if(role == Qt::DisplayRole)
    {
    std::stringstream ss;
    if(orientation == Qt::Horizontal)
      {
      ss << "H_" << section;
      return QString(ss.str().c_str());
      }
    else if(orientation == Qt::Vertical)
      {
      ss << "V_" << section;
      return QString(ss.str().c_str());
      }
 
    }
 
  return QVariant::Invalid;
}