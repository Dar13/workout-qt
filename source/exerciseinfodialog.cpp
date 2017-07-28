#include "exerciseinfodialog.h"
#include "ui_exerciseinfodialog.h"

ExerciseInfoDialog::ExerciseInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExerciseInfoDialog)
{
    ui->setupUi(this);
}

ExerciseInfoDialog::~ExerciseInfoDialog()
{
    delete ui;
}

void ExerciseInfoDialog::setElements(QString &name, bool is_fav)
{
    ui->exercise_edit->document()->setPlainText(name);
    ui->fav_check->setChecked(is_fav);
}

QString ExerciseInfoDialog::getName()
{
    return ui->exercise_edit->document()->toPlainText();
}

bool ExerciseInfoDialog::isFavorite()
{
    return ui->fav_check->isChecked();
}
