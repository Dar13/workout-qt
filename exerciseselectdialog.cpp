#include <memory>

#include "exerciseselectdialog.h"
#include "exerciseinfodialog.h"

#include "ui_exerciseselectdialog.h"

ExerciseSelectDialog::ExerciseSelectDialog(QWidget *parent, DBManager* dbManager) :
    QDialog(parent),
    ui(new Ui::ExerciseSelectDialog),
    _database(dbManager),
    _favoritesModel(dbManager, true),
    _allModel(dbManager, false)
{
    ui->setupUi(this);

    ui->favorite_list_view->setModel(&_favoritesModel);
    ui->favorite_list_view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->favorite_list_view->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    connect(ui->favorite_list_view->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &ExerciseSelectDialog::handleRowSelection);

    ui->all_list_view->setModel(&_allModel);
    ui->all_list_view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->all_list_view->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    connect(ui->all_list_view->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &ExerciseSelectDialog::handleRowSelection);

    connect(ui->add_exercise_button, &QPushButton::clicked,
            this, &ExerciseSelectDialog::handleAddExercise);
}

ExerciseSelectDialog::~ExerciseSelectDialog()
{
    delete ui;
}

ExerciseInformation ExerciseSelectDialog::getSelectedExercise()
{
    return _selectedExercise;
}

void ExerciseSelectDialog::handleAddExercise(bool)
{
    auto exercise_info = std::make_unique<ExerciseInfoDialog>(new ExerciseInfoDialog(this));
    if(exercise_info->exec() == QDialog::Accepted)
    {
        ExerciseInformation info;
        info.favorite = exercise_info->isFavorite();
        info.name = exercise_info->getName();
        _database->addExerciseInformation(info);
    }
}

void ExerciseSelectDialog::handleEditExercise(bool)
{
    qCritical("IMPLEMENT EDIT EXERCISE");
}

void ExerciseSelectDialog::handleRowSelection(const QModelIndex& curr, const QModelIndex& prev)
{
    if(curr.model())
    {
        ui->buttonBox->setEnabled(true);
        if(curr.model() == reinterpret_cast<QAbstractItemModel*>(&_allModel))
        {
            _selectedExercise = _allModel.getExercise(curr.row());
        }
        else
        {
            _selectedExercise = _favoritesModel.getExercise(curr.row());
        }
    }
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

ExerciseInformation ExerciseSelectTableModel::getExercise(int row)
{
    return _exercises.at(row);
}
