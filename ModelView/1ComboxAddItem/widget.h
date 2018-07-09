#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QAbstractListModel>

class ComboxModel : public QAbstractListModel
{
    Q_OBJECT
public:
    ComboxModel(QStringList &_list,QObject *parent = 0);
    ~ComboxModel();
    //QAbstractListModel 中3个必须重新实现的虚函数
    int	rowCount(const QModelIndex & parent = QModelIndex()) const;
    int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

private:
    QStringList list;

};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
};

#endif // WIDGET_H
