#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "workoutlogdialog.h"
#include "exerciseselectdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->log_control, SIGNAL(clicked(bool)), this, SLOT(handleLogControl(bool)));

    connect(ui->exercise_control, SIGNAL(clicked(bool)), this, SLOT(handleExerciseSelect(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDatabase(DBManager *database)
{
    _database = database;
}

void MainWindow::handleLogControl(bool)
{
    WorkoutLogDialog* log_dialog = new WorkoutLogDialog(this, _database);
    log_dialog->exec();
}

void MainWindow::handleExerciseSelect(bool)
{
    ExerciseSelectDialog* ex_sel_dialog = new ExerciseSelectDialog(this, _database);
    if(ex_sel_dialog->exec() == QDialog::Accepted)
    {
        ui->exercise_control->setText(ex_sel_dialog->getSelectedExercise().name);
    }
}
