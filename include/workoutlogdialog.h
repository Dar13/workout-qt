#ifndef WORKOUTLOGDIALOG_H
#define WORKOUTLOGDIALOG_H

#include <QDialog>
#include <QAbstractTableModel>

#include "dbmanager.h"

namespace Ui {
class WorkoutLogDialog;
}

class WorkoutLogTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    WorkoutLogTableModel(DBManager* database = nullptr,
                         ExerciseInformation filter = ExerciseInformation());

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    SetDisplayInformation getSet(int row);

    Qt::ItemFlags flags(const QModelIndex &index) const;

public slots:
    void updateSets();

private:
    ExerciseInformation _filter_exercise;
    DBManager* _database;
    QVector<SetDisplayInformation> _sets;
};

class WorkoutLogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WorkoutLogDialog(QWidget *parent = 0, DBManager* dbManager = nullptr);
    WorkoutLogDialog(QWidget *parent, DBManager *dbManager, ExerciseInformation filter);
    ~WorkoutLogDialog();

    void handleEditSet(bool);
    void handleRowSelection(const QModelIndex&, const QModelIndex&);
    void contextMenu(const QPoint&);

private:
    void editSet(SetInformation& info);

    Ui::WorkoutLogDialog *ui;
    DBManager* _database;
    WorkoutLogTableModel _dataModel;
};

#endif // WORKOUTLOGDIALOG_H
