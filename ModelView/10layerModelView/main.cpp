#include <QApplication>
 
#include "layertableview.h"
 
int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  LayerTableView tab;
  tab.show();
  return app.exec();
}