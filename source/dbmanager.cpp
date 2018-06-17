#include "dbmanager.h"

#include <QSqlQuery>
#include <QSqlError>
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

const QString get_sets = "SELECT S.id, S.time, E.name, S.weight, S.reps "
                         "FROM ExerciseInfo as E, SetInfo as S "
                         "WHERE E.id = S.exercise_id "
                         "ORDER BY S.time DESC;";

// Not used
const QString get_sets_exercise = "SELECT S.id, S.time, E.name, S.weight, S.reps "
                                  "FROM ExerciseInfo as E, SetInfo as S "
                                  "WHERE (S.exercise_id = ? AND E.id = S.exercise_id) "
                                  "ORDER BY S.time DESC;";

const QString get_exercise = "SELECT id, name, favorite FROM ExerciseInfo WHERE name = ?;";
const QString get_exercise_from_id = "SELECT id, name, favorite FROM ExerciseInfo WHERE id = ?;";
const QString get_all_exercises = "SELECT id, name, favorite FROM ExerciseInfo;";
const QString get_fav_exercises = "SELECT id, name, favorite FROM ExerciseInfo "
                                  "WHERE favorite != 0;";

// TODO: retrieve sets in date range

const QString update_exercise = "UPDATE ExerciseInfo SET name = ?,favorite = ? WHERE id = ?";
const QString update_set = "UPDATE SetInfo SET exercise_id = ?, weight = ?, reps = ?, time = ? WHERE id = ?;";

const QString delete_exercise = "DELETE FROM ExerciseInfo WHERE id = ?";
const QString delete_set = "DELETE FROM SetInfo WHERE id = ?;";

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
    else
    {
        emit exercisesUpdated();
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
    else
    {
        emit setsUpdated();
    }
}

void DBManager::updateExerciseInformation(ExerciseInformation &info)
{
    QSqlQuery query(_database);
    if(!query.prepare(update_exercise))
    {
        qCritical("Failed to prepare update query for ExerciseInfo!");
        return;
    }
    query.addBindValue(info.name);
    query.addBindValue(info.favorite);
    query.addBindValue(info.id);
    if(!query.exec())
    {
        qCritical("Update of exercise information failed!");
    }
    else
    {
        emit exercisesUpdated();
    }
}

void DBManager::updateSetInformation(SetInformation &info)
{
    QSqlQuery query(_database);
    if(!query.prepare(update_set))
    {
        qCritical("Failed to prepare update query for SetInfo!");
        return;
    }
    query.addBindValue(info.exercise_id);
    query.addBindValue(info.weight);
    query.addBindValue(info.reps);
    query.addBindValue(info.timestamp.toString(Qt::ISODate));
    query.addBindValue(info.id);
    if(!query.exec())
    {
        qCritical("Update of set information failed!");
    }
    else
    {
        emit setsUpdated();
    }
}

void DBManager::deleteExerciseInformation(ExerciseInformation &info)
{
    QSqlQuery query(_database);
    if(!query.prepare(delete_exercise))
    {
        qCritical("Failed to prepare delete query for ExerciseInfo");
        return;
    }
    query.addBindValue(info.id);
    if(!query.exec())
    {
        qCritical("Delete of exercise information failed!");
    }
    else
    {
        emit exercisesUpdated();
    }
}

void DBManager::deleteSetInformation(SetInformation &info)
{
    QSqlQuery query(_database);
    if(!query.prepare(delete_set))
    {
        qCritical("Failed to prepare query for DELETE command on SetInfo");
        return;
    }
    query.addBindValue(info.id);
    if(!query.exec())
    {
        qCritical("Delete of set information failed!");
    }
    else
    {
        emit setsUpdated();
    }
}

bool DBManager::getExercise(const QString& exercise_name, ExerciseInformation& info)
{
    QSqlQuery query(_database);
    if(!query.prepare(get_exercise))
    {
        qCritical("Failed to prepare lookup query for ExerciseInfo");
        return false;
    }
    query.addBindValue(exercise_name);
    if(!query.exec())
    {
        qCritical(tr("Retrieval of exercise information for exercise '%0' failed.").arg(exercise_name).toStdString().c_str());
        return false;
    }

    // SQLite doesn't support query->size(), so we have to work around that.

    if(!query.isSelect() || !query.first() || !query.isValid())
    {
        qCritical(tr("Unable to process results of the query to retrieve '%0'.").arg(exercise_name).toStdString().c_str());
        return false;
    }

    info.id = query.value(0).toInt();
    info.name = query.value(1).toString();
    info.favorite = query.value(2).toBool();

    return true;
}

bool DBManager::getExercise(int exercise_id, ExerciseInformation& info)
{
    QSqlQuery query(_database);
    if(!query.prepare(get_exercise_from_id))
    {
        qCritical("Failed to prepare lookup query for ExerciseInfo");
        return false;
    }
    query.addBindValue(exercise_id);
    if(!query.exec())
    {
        qCritical(tr("Retrieval of exercise information for exercise ID '%0' failed.").arg(exercise_id).toStdString().c_str());
        return false;
    }

    // SQLite doesn't support query->size(), so we have to work around that.

    if(!query.isSelect() || !query.first() || !query.isValid())
    {
        qCritical(tr("Unable to process results of the query to retrieve '%0'.").arg(exercise_id).toStdString().c_str());
        return false;
    }

    info.id = query.value(0).toInt();
    info.name = query.value(1).toString();
    info.favorite = query.value(2).toBool();

    return true;
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
        info.id = query.value(0).toInt();
        info.weight = query.value(3).toInt();
        info.reps = query.value(4).toInt();
        info.timestamp = QDateTime::fromString(query.value(1).toString(), Qt::ISODate);
        info.exercise_name = query.value(2).toString();
        values.push_back(info);
        query.next();
    }
    query.finish();

    return values;
}

QVector<SetDisplayInformation> DBManager::getAllDisplaySets(ExerciseInformation& info)
{
    // TODO: Perhaps use a dedicated SQL query...
    auto all = this->getAllDisplaySets();
    QVector<SetDisplayInformation> result;

    for(auto itr : all)
    {
        if(itr.exercise_name == info.name)
        {
            result.append(itr);
        }
    }

    return result;
}

