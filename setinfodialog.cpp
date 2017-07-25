#include <memory>

#include "setinfodialog.h"
#include "exerciseselectdialog.h"

#include "ui_setinfodialog.h"

SetInfoDialog::SetInfoDialog(QWidget *parent, DBManager* dbManager) :
    QDialog(parent),
    ui(new Ui::SetInfoDialog),
    _database(dbManager)
{
    ui->setupUi(this);

    connect(ui->set_exercise, &QPushButton::clicked, this, &SetInfoDialog::handleExerciseSelect);
}

SetInfoDialog::~SetInfoDialog()
{
    delete ui;
}

void SetInfoDialog::setElements(int exercise_id, int weight, int reps, QDateTime timestamp)
{
    ui->set_weight->setValue(weight);
    ui->set_reps->setValue(reps);
    ui->set_date->setDateTime(timestamp);
    ExerciseInformation exercise;
    if(!_database->getExercise(exercise_id, exercise))
    {
        qCritical(tr("Unable to retrieve exercise of ID '%0'!").arg(exercise_id).toStdString().c_str());
        return;
    }

    ui->set_exercise->setText(exercise.name);
}

void SetInfoDialog::handleExerciseSelect(bool)
{
    auto exercise_sel_dlg = std::make_unique<ExerciseSelectDialog>(this, _database);
    if(exercise_sel_dlg->exec() == QDialog::Accepted)
    {
        ui->set_exercise->setText(exercise_sel_dlg->getSelectedExercise().name);
    }
}

int SetInfoDialog::getReps()
{
    return ui->set_reps->value();
}

QDateTime SetInfoDialog::getTimestamp()
{
    return ui->set_date->dateTime();
}

int SetInfoDialog::getWeight()
{
    return ui->set_weight->value();
}

int SetInfoDialog::getExerciseID()
{
    ExerciseInformation set_ex;
    set_ex.name = ui->set_exercise->text();
    set_ex.name = set_ex.name.remove('&');
    if(!_database->getExercise(set_ex.name, set_ex))
    {
        return -1;
    }

    return set_ex.id;
}
