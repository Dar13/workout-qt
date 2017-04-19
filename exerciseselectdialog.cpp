#include "exerciseselectdialog.h"
#include "ui_exerciseselectdialog.h"

ExerciseSelectDialog::ExerciseSelectDialog(QWidget *parent, DBManager* dbManager) :
    QDialog(parent),
    ui(new Ui::ExerciseSelectDialog),
    _favoritesModel(dbManager, true),
    _allModel(dbManager, false)
{
    ui->setupUi(this);

    ui->favorite_list_view->setModel(&_favoritesModel);
    ui->favorite_list_view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->favorite_list_view->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    ui->all_list_view->setModel(&_allModel);
    ui->all_list_view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->all_list_view->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
}

ExerciseSelectDialog::~ExerciseSelectDialog()
{
    delete ui;
}

ExerciseSelectTableModel::ExerciseSelectTableModel(DBManager *database, bool only_favorites)
    : _only_favorites(only_favorites), _database(database)
{
    if(_database)
    {
        _exercises = _database->getExercises(_only_favorites);
    }
}

Qt::ItemFlags ExerciseSelectTableModel::flags(const QModelIndex &index) const
{
    // TODO: Make cells editable, to enable exercise modification?
    return (Qt::ItemIsSelectable | QAbstractTableModel::flags(index));
}

int ExerciseSelectTableModel::rowCount(const QModelIndex &parent) const
{
    return _exercises.length();
}

int ExerciseSelectTableModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant ExerciseSelectTableModel::data(const QModelIndex &index, int role) const
{
    QVariant value;
    if(role == Qt::DisplayRole)
    {
       if(index.column() >= 0 && index.column() < 2 &&
          index.row() >= 0 && index.row() < _exercises.length())
       {
           ExerciseInformation ex_info = _exercises.at(index.row());
           switch(index.column())
           {
           case 0:
               value = ex_info.favorite;
               break;
           case 1:
               value = ex_info.name;
               break;
           }
       }
    }

    return value;
}

QVariant ExerciseSelectTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant value;
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
        case 0:
            value = tr("Favorite");
            break;
        case 1:
            value = tr("Exercise Name");
            break;
        }
    }

    return value;
}
