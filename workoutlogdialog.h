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
public:
    WorkoutLogTableModel(DBManager* database = nullptr);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
private:
    DBManager* _database;
    QVector<SetDisplayInformation> _sets;
};

class WorkoutLogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WorkoutLogDialog(QWidget *parent = 0, DBManager* dbManager = nullptr);
    ~WorkoutLogDialog();

private:
    Ui::WorkoutLogDialog *ui;
    WorkoutLogTableModel _dataModel;
};

#endif // WORKOUTLOGDIALOG_H
