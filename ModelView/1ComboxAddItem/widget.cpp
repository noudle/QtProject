#include "widget.h"
//将QStringList中的字符串显示在combox中。

ComboxModel::ComboxModel(QStringList &_list,QObject *parent) :list(_list),QAbstractListModel(parent)
{

}

ComboxModel::~ComboxModel()
{

}

//体现model的行数，这里就是list的个数
int	ComboxModel::rowCount(const QModelIndex & parent) const
{
    return list.count();
}

//体现model的列数，这里只有1列
int	ComboxModel::columnCount(const QModelIndex & parent) const
{
    return 1;
}

//体现model中显示的数据，这里只是list中每一个子项。
QVariant ComboxModel::data(const QModelIndex & index, int role) const
{
    if (Qt::DisplayRole == role)
    {
        return list.at(index.row());//返回list每一项
    }
    return QVariant::Invalid;
}


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->resize(200,200);

    QComboBox *combox = new QComboBox(this);
    combox->setGeometry(50,50,100,40);

    QStringList list;
    list<<"123"<<"345"<<"546"<<"apple";
    //创建model并初始化它。
    ComboxModel *model = new ComboxModel(list);

    combox->setModel(model);
}

Widget::~Widget()
{

}
