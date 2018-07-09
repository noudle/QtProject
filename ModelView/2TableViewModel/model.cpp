#include "model.h"
//将二维数组中的值显示在tableview中。

TableModel::TableModel(QObject *parent) :QAbstractTableModel(parent)
{
    //二维数初始化，从100开始
    nn = 100;
    for(int i=0;i<ROW;++i){
        for(int j=0;j<COL;++j){
            num[i][j] = nn;
            ++nn;
        }
    }
}

TableModel::~TableModel()
{
}

//体现model的行数，这里有3行
int	TableModel::rowCount(const QModelIndex & parent) const
{
    return ROW;
}

//体现model的列数，这里有2列
int	TableModel::columnCount(const QModelIndex & parent) const
{
    return COL;
}

//体现model中显示的数据，这里只是num中每一个成员值。
QVariant TableModel::data(const QModelIndex & index, int role) const
{
    if (Qt::DisplayRole == role)
    {
        return QString::number(num[index.row()][index.column()]);//返回num二维数组中的值
    }
    return QVariant();
}

//显示标题栏
//QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
//{
//    if(orientation==Qt::Horizontal && role==Qt::DisplayRole)
//    {
//        switch (section) {
//            case 0:
//                return "NUM1";
//                break;
//            case 1:
//                return "NUM2";
//                break;
//            default:
//                break;
//            }
//         }
//        else if(orientation==Qt::Horizontal && role==Qt::TextAlignmentRole)
//        {
//            return int(Qt::AlignHCenter | Qt::AlignVCenter);
//        }
//        return QVariant();
//}

//可编辑表格添加函数
Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    flags |= Qt::ItemIsEditable;
    return flags;
}

bool TableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
     {
         num[index.row()][index.column()] = value.toInt();
         emit dataChanged(index, index);
         return true;
     }
     return false;
}
