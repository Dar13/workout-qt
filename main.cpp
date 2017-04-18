#include "mainwindow.h"
#include <QApplication>

#include "countdownmodel.h"
#include "dbmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    CountdownModel countdownModel(&w);
    DBManager dbManager(nullptr);

    w.setDatabase(&dbManager);

    return a.exec();
}
