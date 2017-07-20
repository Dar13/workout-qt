#ifndef SETINFODIALOG_H
#define SETINFODIALOG_H

#include <QDialog>

#include "dbmanager.h"

namespace Ui {
class SetInfoDialog;
}

class SetInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetInfoDialog(QWidget *parent = 0, DBManager* dbManager = nullptr);
    ~SetInfoDialog();

    void setElements(int exercise_id, int weight, int reps, QDateTime timestamp);

    int getExerciseID();
    int getWeight();
    int getReps();

public slots:
    void handleExerciseSelect(bool);

private:
    Ui::SetInfoDialog *ui;
    DBManager* _database;
};

#endif // SETINFODIALOG_H