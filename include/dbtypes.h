#ifndef DBTYPES_H
#define DBTYPES_H

#include <QString>
#include <QDateTime>

struct ExerciseInformation
{
    ExerciseInformation();
    ExerciseInformation(const ExerciseInformation& info);

    int id;
    QString name;
    bool favorite;
};

struct SetInformation;
class DBManager;

struct SetDisplayInformation
{
    SetDisplayInformation();
    SetDisplayInformation(const SetDisplayInformation&);
    SetDisplayInformation(const SetInformation&, DBManager*);

    static SetDisplayInformation fromSetInfo(const SetInformation&, DBManager*);

    int id;
    QString exercise_name;
    int weight;
    int reps;
    QDateTime timestamp;
};

struct SetInformation
{
    SetInformation();
    SetInformation(const SetInformation&);
    SetInformation(const SetDisplayInformation&, DBManager*);

    static SetInformation fromDisplayInfo(const SetDisplayInformation&, DBManager*);

    int id;
    int exercise_id;
    int weight;
    int reps;
    QDateTime timestamp; // Use Qt::ISODate for format
};

#endif // DBTYPES_H
