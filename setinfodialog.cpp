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

void SetInfoDialog::setElements(const SetDisplayInformation &info)
{
    ui->set_weight->setValue(info.weight);
    ui->set_reps->setValue(info.reps);
    ui->set_date->setDateTime(info.timestamp);
    ui->set_exercise->setText(info.exercise_name);
}

void SetInfoDialog::handleExerciseSelect(bool)
{
    auto exercise_sel_dlg = std::make_unique<ExerciseSelectDialog>(this, _database);
    if(exercise_sel_dlg->exec() == QDialog::Accepted)
    {
        ui->set_exercise->setText(exercise_sel_dlg->getSelectedExercise().name);
    }
}

SetInformation SetInfoDialog::getSet()
{
    SetDisplayInformation info;
    info.reps = ui->set_reps->value();
    info.weight = ui->set_weight->value();
    info.timestamp = ui->set_date->dateTime();
    info.exercise_name = ui->set_exercise->text().remove('&');

    return SetInformation::fromDisplayInfo(info, _database);
}
