#include "mainwindow.h"
#include <QApplication>

#include "countdownmodel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    CountdownModel countdownModel(&w);

    return a.exec();
}
