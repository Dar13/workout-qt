#include <memory>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "workoutlogdialog.h"
#include "exerciseselectdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->log_control, &QPushButton::clicked, this, &MainWindow::handleLogControl);
    connect(ui->exercise_log_control, &QPushButton::clicked, this, &MainWindow::handleExerciseLogControl);
    connect(ui->exercise_control, &QPushButton::clicked, this, &MainWindow::handleExerciseSelect);
    connect(ui->save_control, &QPushButton::clicked, this, &MainWindow::handleSaveSet);
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

void MainWindow::handleExerciseLogControl(bool)
{
    ExerciseInformation filter;
    std::unique_ptr<WorkoutLogDialog> ex_log_dialog;
    if(_database->getExercise(ui->exercise_control->text().remove('&'), filter))
    {
      ex_log_dialog = std::make_unique<WorkoutLogDialog>(this, _database, std::move(filter));
    }
    else
    {
      ex_log_dialog = std::make_unique<WorkoutLogDialog>(this, _database);
    }

    ex_log_dialog->exec();
}

void MainWindow::handleExerciseSelect(bool)
{
    auto exercise_sel_dlg = std::make_unique<ExerciseSelectDialog>(this, _database);
    if(exercise_sel_dlg->exec() == QDialog::Accepted)
    {
        ui->exercise_control->setText(exercise_sel_dlg->getSelectedExercise().name);
    }
}

void MainWindow::handleSaveSet(bool)
{
    // Grab current state of:
    //  1) Exercise
    //  2) Weight
    //  3) Repetitions
    // Then take that and put it into the database
    int weight = ui->weight_value->value();
    int reps = ui->rep_value->value();

    QString exercise_str = ui->exercise_control->text();
    // This is to remove the 'shortcut' character from the QPushButton text
    exercise_str = exercise_str.remove('&');

    ExerciseInformation exercise_info;
    if(!_database->getExercise(exercise_str, exercise_info))
    {
        qCritical(tr("Unable to save set information into log.").toStdString().c_str());
        return;
    }

    SetInformation set_info;
    set_info.exercise_id = exercise_info.id;
    set_info.reps = reps;
    set_info.weight = weight;

    _database->addSetInformation(set_info);
}
