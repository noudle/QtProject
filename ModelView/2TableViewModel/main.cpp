#include <QApplication>
#include <QTableView>
#include "model.h"
#include "lineeditdeleget.h"
#include <QStandardItemModel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTableView *tableview = new QTableView;

    TableModel *model = new TableModel;

    tableview->setModel(model);

    //新建代理，将代理与tableview关联
    tableview->setItemDelegate(new LineEditDeleget());

    tableview->show();

    return a.exec();
}
