#include "workoutlogdialog.h"
#include "ui_workoutlogdialog.h"

WorkoutLogDialog::WorkoutLogDialog(QWidget *parent, DBManager* dbManager) :
    QDialog(parent),
    ui(new Ui::WorkoutLogDialog),
    _dataModel(dbManager)
{
    ui->setupUi(this);

    ui->log_view->setModel(&_dataModel);
    ui->log_view->verticalHeader()->hide();
    ui->log_view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->log_view->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->log_view->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->log_view->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
}

WorkoutLogDialog::~WorkoutLogDialog()
{
    delete ui;
}

WorkoutLogTableModel::WorkoutLogTableModel(DBManager *database)
    : _database(database)
{
    if(_database)
    {
        _sets = _database->getAllDisplaySets();
    }
}

Qt::ItemFlags WorkoutLogTableModel::flags(const QModelIndex &index) const
{
    // TODO: Make cells editable, to enable log modification?
    return (Qt::ItemIsSelectable | QAbstractTableModel::flags(index));
}

int WorkoutLogTableModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

int WorkoutLogTableModel::rowCount(const QModelIndex &parent) const
{
    return _sets.length();
}

QVariant WorkoutLogTableModel::data(const QModelIndex &index, int role) const
{
    QVariant value;
    if(role == Qt::DisplayRole)
    {
        if(index.column() < 4 && index.column() >= 0 &&
           index.row() < _sets.length() && index.row() >= 0)
        {
            SetDisplayInformation set_info = _sets.at(index.row());
            switch(index.column())
            {
            case 0:
                value = set_info.set_date;
                break;
            case 1:
                value = set_info.exercise_name;
                break;
            case 2:
                value = set_info.weight;
                break;
            case 3:
                value = set_info.reps;
                break;
            }
        }
    }

    return value;
}

QVariant WorkoutLogTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant value;
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
        case 0:
            value = tr("Date/Time");
            break;
        case 1:
            value = tr("Exercise");
            break;
        case 2:
            value = tr("Weight");
            break;
        case 3:
            value = tr("Reps");
            break;
        }
    }

    return value;
}
