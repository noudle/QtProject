#ifndef LINEEDITDELEGET_H
#define LINEEDITDELEGET_H

#include <QObject>
#include <QLineEdit>
#include <QEvent>
#include <QMouseEvent>
#include <QStyledItemDelegate>
#include <QAbstractItemModel>

#include <QDebug>

class LineEditDeleget : public QStyledItemDelegate
{
public:
    LineEditDeleget();
    ~LineEditDeleget();

    QWidget *createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    //void    destroyEditor(QWidget * editor, const QModelIndex & index) const;
    void	setEditorData(QWidget * editor, const QModelIndex & index) const;
    void	setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;

    //bool	editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index);
    //void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // LINEEDITDELEGET_H
