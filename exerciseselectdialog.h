#ifndef EXERCISESELECTDIALOG_H
#define EXERCISESELECTDIALOG_H

#include <QDialog>
#include <QAbstractTableModel>
#include <QItemSelection>

#include "dbmanager.h"

namespace Ui {
class ExerciseSelectDialog;
}

class ExerciseSelectTableModel : public QAbstractTableModel
{
public:
    ExerciseSelectTableModel(DBManager* database = nullptr, bool only_favorites = false);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    ExerciseInformation getExercise(int row);

    Qt::ItemFlags flags(const QModelIndex &index) const;
private:
    bool _only_favorites;
    DBManager* _database;
    QVector<ExerciseInformation> _exercises;
};

class ExerciseSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExerciseSelectDialog(QWidget *parent = 0, DBManager* dbManager = nullptr);
    ~ExerciseSelectDialog();

    ExerciseInformation getSelectedExercise();

public slots:
    void handleAddExercise(bool);
    void handleEditExercise(bool);
    void handleRowSelection(const QModelIndex&, const QModelIndex&);

private:
    Ui::ExerciseSelectDialog *ui;
    DBManager* _database;
    ExerciseSelectTableModel _favoritesModel;
    ExerciseSelectTableModel _allModel;
    ExerciseInformation _selectedExercise;
};

#endif // EXERCISESELECTDIALOG_H
