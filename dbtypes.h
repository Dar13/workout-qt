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

struct SetDisplayInformation
{
    int id;
    QString exercise_name;
    int weight;
    int reps;
    QDateTime timestamp;
};

struct SetInformation
{
    int id;
    int exercise_id;
    int weight;
    int reps;
    QDateTime timestamp; // Format in text is "yyyy-MM-dd HH:mm:ss"
};

#endif // DBTYPES_H
