#include <QStringListModel>
#include <QtGui>
 
#include <iostream>

#include "form.h"
 
class CheckableStringListModel : public QStringListModel
{
 
public:
  void setMyStringList(const QStringList &strings)
  {
    emit beginResetModel();
    CheckedStatus.resize(strings.size());
    for(unsigned int i = 0; i < CheckedStatus.size(); ++i)
      {
      CheckedStatus[i] = Qt::Unchecked;
      }
    setStringList(strings);
    emit endResetModel();
  }
 
private:
  QVector<Qt::CheckState> CheckedStatus;
 
  Qt::ItemFlags flags(const QModelIndex& index) const
  {
    // If the index is not valid
    if (index.row() < 0 || index.row() >= rowCount())
      {
      return Qt::NoItemFlags;
      }
 
    return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
  }
 
  QVariant data (const QModelIndex  &index , int role ) const
  {
    // From QStringListModel 
    if (index.row() < 0 || index.row() >= rowCount())
      {
      return QVariant();
      }
 
    if (role == Qt::DisplayRole || role == Qt::EditRole)
      {
      return stringList().at(index.row());
      }
 
    if(role == Qt::CheckStateRole)
      {
      return CheckedStatus[index.row()];
      }
 
    return QVariant();
  }
 
  bool setData (const QModelIndex &index, const QVariant &value, int role)
  {
    // From QStringListModel
    if (index.row() < 0 || index.row() >= rowCount())
      {
      return false;
      }
 
    if(role == Qt::EditRole || role == Qt::DisplayRole)
      {
      stringList().replace(index.row(), value.toString());
      emit dataChanged(index, index);
      return true;
      }
    if(role == Qt::CheckStateRole)
      {
      //CheckedStatus[index.row()] = value.value<Qt::CheckState>();
      CheckedStatus[index.row()] = static_cast<Qt::CheckState>(value.toUInt());
 
      emit dataChanged(index, index);
      }
    return true;
 
  }
};
 
Form::Form(QWidget *parent) : QWidget(parent)
{
  setupUi(this);
 
  CheckableStringListModel* model = new CheckableStringListModel;
 
  QStringList list;
  list << "a" << "b" << "c";
  model->setMyStringList(list);
  this->listView->setModel(model);

}
