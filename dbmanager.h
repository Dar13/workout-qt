#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QVector>

#include "dbtypes.h"

class DBManager : public QObject
{
    Q_OBJECT

public:
    explicit DBManager(QObject *parent = 0);
    ~DBManager();

    void addSetInformation(SetInformation& info);
    void addExerciseInformation(ExerciseInformation& info);

    void updateSetInformation(SetInformation& info);
    void updateExerciseInformation(ExerciseInformation& info);

    QVector<SetInformation> getAllSets();
    QVector<ExerciseInformation> getAllExercises();
    QVector<SetDisplayInformation> getAllDisplaySets();

    // TODO: Get a list, based on date range

signals:

public slots:

private:
    QSqlDatabase _database;
};

#endif // DBMANAGER_H
