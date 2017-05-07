#include "dbmanager.h"

#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>

const QString database_path = "database.db";
const QString create_exercise_query = "CREATE TABLE IF NOT EXISTS ExerciseInfo "
                                      "(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
                                      " name TEXT NOT NULL,"
                                      " favorite BOOLEAN);";
const QString create_set_query = "CREATE TABLE IF NOT EXISTS SetInfo "
                                 "(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
                                 " exercise_id INTEGER NOT NULL REFERENCES ExerciseInfo (id),"
                                 " time TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,"
                                 " weight INTEGER NOT NULL,"
                                 " reps INTEGER NOT NULL);";

const QString insert_exercise = "INSERT INTO ExerciseInfo (name, favorite) "
                                "VALUES (?, ?)";
const QString insert_set = "INSERT INTO SetInfo (exercise_id, weight, reps) "
                           "VALUES (?, ?, ?)";

const QString get_sets = "SELECT S.time, E.name, S.weight, S.reps "
                         "FROM ExerciseInfo as E, SetInfo as S "
                         "WHERE E.id = S.exercise_id";

const QString get_all_exercises = "SELECT id, name, favorite FROM ExerciseInfo;";
const QString get_fav_exercises = "SELECT id, name, favorite FROM ExerciseInfo "
                                  "WHERE favorite != 0;";

// TODO: retrieve sets in date range

const QString update_exercise = "UPDATE ExerciseInfo SET favorite = ?";
const QString update_set = "UPDATE SetInfo SET weight = ?, reps = ?";

// TODO: Rework query execution into separate templated function

DBManager::DBManager(QObject *parent) : QObject(parent)
{
    _database = QSqlDatabase::addDatabase("QSQLITE");
    _database.setDatabaseName(database_path);
    _database.open();

    if(!_database.isOpen())
    {
        qCritical("Unable to open database!");
    }

    // Check and make sure all our tables exist.
    QSqlQuery exercise_query(_database);
    if(!exercise_query.prepare(create_exercise_query))
    {
        qCritical("Unable to prepare query to create ExerciseInfo table");
    }
    if(!exercise_query.exec())
    {
        qCritical("Unable to create ExerciseInfo table!");
    }

    QSqlQuery set_query(_database);
    if(!set_query.prepare(create_set_query))
    {
        qCritical("Unable to prepare query to create SetInfo table");
    }
    if(!set_query.exec())
    {
        qCritical("Unable to create SetInfo table!");
    }
}

DBManager::~DBManager()
{
    _database.close();
}

void DBManager::addExerciseInformation(ExerciseInformation& info)
{
    QSqlQuery query(_database);
    if(!query.prepare(insert_exercise))
    {
        qCritical("Failed to prepare insert query for ExerciseInfo!");
        return;
    }
    query.addBindValue(info.name);
    query.addBindValue(info.favorite);
    if(!query.exec())
    {
        qCritical("Insertion of exercise information failed!");
    }
}

void DBManager::addSetInformation(SetInformation &info)
{
    QSqlQuery query(_database);
    if(!query.prepare(insert_set))
    {
        qCritical("Failed to prepare insert query for ExerciseInfo!");
        return;
    }
    query.addBindValue(info.exercise_id);
    query.addBindValue(info.weight);
    query.addBindValue(info.reps);
    if(!query.exec())
    {
        qCritical("Insertion of set information failed!");
    }
}

void DBManager::updateExerciseInformation(ExerciseInformation &info)
{
    qCritical("IMPLEMENT UPDATE EXERCISE INFO");
    return;
}

void DBManager::updateSetInformation(SetInformation &info)
{
    qCritical("IMPLEMENT UPDATE EXERCISE INFO");
    return;
}

QVector<ExerciseInformation> DBManager::getExercises(bool favorites_only)
{
    QVector<ExerciseInformation> values;
    QSqlQuery query(_database);
    const QString& query_str = (favorites_only) ? get_fav_exercises : get_all_exercises;
    if(query.prepare(query_str) && query.exec())
    {
        if(query.first())
        {
            while(query.isValid())
            {
                ExerciseInformation ex_info;
                ex_info.id = query.value(0).toInt();
                ex_info.name = query.value(1).toString();
                ex_info.favorite = query.value(2).toBool();
                values.push_back(ex_info);
                query.next();
            }
            query.finish();
        }
    }
    else
    {
        qCritical("Unable to prepare/execute retrieval query");
    }

    return values;
}

QVector<SetInformation> DBManager::getAllSets()
{
    return QVector<SetInformation>();
}

QVector<SetDisplayInformation> DBManager::getAllDisplaySets()
{
    QSqlQuery query(_database);
    if(!query.prepare(get_sets) || !query.exec())
    {
        qCritical("Unable to prepare/execute retrieval query");
        return QVector<SetDisplayInformation>();
    }
    else
    {
        if(!query.first())
        {
            // Empty result
            return QVector<SetDisplayInformation>();
        }
    }

    QVector<SetDisplayInformation> values;
    while(query.isValid())
    {
        SetDisplayInformation info;
        info.weight = query.value(2).toInt();
        info.reps = query.value(3).toInt();
        info.set_date = QDateTime::fromString(query.value(0).toString(), Qt::ISODate);
        info.exercise_name = query.value(1).toString();
        values.push_back(info);
        query.next();
    }
    query.finish();

    return values;
}
