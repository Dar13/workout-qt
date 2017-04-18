#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "workoutlogdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->log_control, SIGNAL(clicked(bool)), this, SLOT(handleLogControl(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDatabase(DBManager *database)
{
    _database = database;
}

void MainWindow::handleLogControl(bool checked)
{
    (void)checked;
    WorkoutLogDialog* log_dialog = new WorkoutLogDialog(this, _database);
    log_dialog->exec();
}
