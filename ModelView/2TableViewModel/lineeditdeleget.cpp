#include "lineeditdeleget.h"

LineEditDeleget::LineEditDeleget()
{

}

LineEditDeleget::~LineEditDeleget()
{

}

//创建编辑器，这些编辑器是一些控件，如Label，LineEdit,Combox等
QWidget *LineEditDeleget::createEditor(QWidget * parent, const QStyleOptionViewItem & /*option*/, const QModelIndex & /*index*/) const
{
    QLineEdit *line = new QLineEdit(parent);
    return line;
}

//从model中取出数据，放到编辑器中
void	LineEditDeleget::setEditorData(QWidget * editor, const QModelIndex & index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    QString str = index.model()->data(index,Qt::DisplayRole).toString();//获取model中的数据，角色为DisplayRole
    lineEdit->setText(str);
}

//将编辑器中的数据更新到model中
void	LineEditDeleget::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    model->setData(index,lineEdit->text(),Qt::EditRole);
}

//void LineEditDeleget::destroyEditor(QWidget * editor, const QModelIndex & /*index*/) const
//{
//    if(editor)
//        delete editor;
//}


