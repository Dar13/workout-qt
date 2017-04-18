#ifndef DBTYPES_H
#define DBTYPES_H

#include <QString>
#include <QDateTime>

struct ExerciseInformation
{
    int id;
    QString name;
    bool favorite;
};

struct SetInformation
{
    int id;
    int exercise_id;
    int weight;
    int reps;
};

struct SetDisplayInformation
{
    QDateTime set_date;
    QString exercise_name;
    int weight;
    int reps;
};

#endif // DBTYPES_H
