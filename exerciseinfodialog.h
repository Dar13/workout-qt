#ifndef EXERCISEINFODIALOG_H
#define EXERCISEINFODIALOG_H

#include <QDialog>

namespace Ui {
class ExerciseInfoDialog;
}

class ExerciseInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExerciseInfoDialog(QWidget *parent = 0);
    ~ExerciseInfoDialog();

    QString getName();
    bool isFavorite();

private:
    Ui::ExerciseInfoDialog *ui;
};

#endif // EXERCISEINFODIALOG_H
