#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "dbmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void handleLogControl(bool);
    void handleExerciseSelect(bool);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setDatabase(DBManager* database);

private:
    Ui::MainWindow *ui;
    DBManager* _database;
};

#endif // MAINWINDOW_H
