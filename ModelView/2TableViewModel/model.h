#ifndef WIDGET_H
#define WIDGET_H

#include <QAbstractTableModel>

#define ROW  3
#define COL  2

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    TableModel(QObject *parent = 0);
    ~TableModel();
    //QAbstractTableModel 中3个必须重新实现的虚函数
    int	rowCount(const QModelIndex & parent = QModelIndex()) const;
    int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    //QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    Qt::ItemFlags flags(const QModelIndex & index) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role);

private:
    int num[ROW][COL];
    int nn;
};
#endif // WIDGET_H
