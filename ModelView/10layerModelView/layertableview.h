#ifndef LAYERLISTVIEW_H
#define LAYERLISTVIEW_H
 
#include <qtableview>
#include <qmouseevent>
#include <qdebug>
#include <qheaderview>
#include <qstandarditemmodel>
#include <qcontextmenuevent>
#include <qmenu>
#include "layertablemodel.h"
#include "layeritemdelegate.h"
 
class LayerTableView : public QTableView
{
    Q_OBJECT
 
public:
    LayerTableView(QWidget *parent = 0);
    ~LayerTableView();
    void setLayerSize(QSize s);
 
public slots:
    void addNewLayer();
    void deleteLayer();
 
protected:
    void mouseMoveEvent(QMouseEvent * event);
    void contextMenuEvent(QContextMenuEvent * event);
 
private:
    LayerItemDelegate *delegate;
    LayerTableModel *model;
    QSize layerSize;
 
private slots:
    void itemClicked(const QModelIndex&);
 
};
 
#endif // LAYERLISTVIEW_H