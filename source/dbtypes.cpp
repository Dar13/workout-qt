#include "dbtypes.h"
#include "dbmanager.h"

SetDisplayInformation::SetDisplayInformation()
    : id(-1), exercise_name(QString()), weight(0), reps(0), timestamp(QDateTime())
{
}

SetDisplayInformation::SetDisplayInformation(const SetDisplayInformation& info)
    : id(info.id),
      exercise_name(info.exercise_name),
      weight(info.weight), reps(info.reps),
      timestamp(info.timestamp)
{
}

SetDisplayInformation::SetDisplayInformation(const SetInformation& info, DBManager* db)
    : id(info.id),
      weight(info.weight), reps(info.reps),
      timestamp(info.timestamp)
{
    ExerciseInformation exercise;
    if(db && db->getExercise(info.exercise_id, exercise))
    {
        this->exercise_name = exercise.name;
    }
    else
    {
        this->exercise_name = QString();
    }
}

SetDisplayInformation SetDisplayInformation::fromSetInfo(const SetInformation& info, DBManager* db)
{
    return SetDisplayInformation(info, db);
}

SetInformation::SetInformation()
    : id(-1), exercise_id(-1), weight(0), reps(0), timestamp(QDateTime())
{
}

SetInformation::SetInformation(const SetInformation& info)
    : id(info.id), exercise_id(info.exercise_id),
      weight(info.weight), reps(info.reps), timestamp(info.timestamp)
{
}

SetInformation::SetInformation(const SetDisplayInformation& info, DBManager* db)
    : id(info.id), weight(info.weight), reps(info.reps), timestamp(info.timestamp)
{
    ExerciseInformation exercise;
    if(db && db->getExercise(info.exercise_name, exercise))
    {
        exercise_id = exercise.id;
    }
    else
    {
        // Not much else I can do here, short of throw an exception...
        exercise_id = -1;
    }
}

SetInformation SetInformation::fromDisplayInfo(const SetDisplayInformation& info, DBManager* db)
{
    return SetInformation(info, db);
}
