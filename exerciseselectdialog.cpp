#include <memory>

#include <QMenu>

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
    connect(ui->favorite_list_view, &QTableView::customContextMenuRequested,
            this, &ExerciseSelectDialog::contextMenu);
    connect(dbManager, &DBManager::exercisesUpdated, &_favoritesModel, &ExerciseSelectTableModel::updateExercises);

    ui->all_list_view->setModel(&_allModel);
    ui->all_list_view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->all_list_view->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    connect(ui->all_list_view->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &ExerciseSelectDialog::handleRowSelection);
    connect(ui->all_list_view, &QTableView::customContextMenuRequested,
            this, &ExerciseSelectDialog::contextMenu);
    connect(dbManager, &DBManager::exercisesUpdated, &_allModel, &ExerciseSelectTableModel::updateExercises);

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
    if(_exerciseSelected)
    {
      editExercise(_selectedExercise);
    }
    // else, pop up a message box saying to select a message first?
}

void ExerciseSelectDialog::handleRowSelection(const QModelIndex& curr, const QModelIndex& prev)
{
    (void)prev;
    if(curr.model())
    {
        // Apparently, you can't nullify a selection in a TableView so this is a one-time assignment
        // TODO: If this isn't true, update this accordingly
        _exerciseSelected = true;
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

enum class CtxMenuSelection
{
    CTX_SEL_NONE    = 0x0,
    CTX_SEL_EDIT    = 0x1,
    CTX_SEL_DEL     = 0x2,
};

void ExerciseSelectDialog::contextMenu(const QPoint& point)
{
    QObject* sender = this->sender();
    QTableView* view = qobject_cast<QTableView*>(sender);
    CtxMenuSelection selected = CtxMenuSelection::CTX_SEL_NONE;

    auto edit_callback = [&selected](bool) { selected = CtxMenuSelection::CTX_SEL_EDIT; };
    auto del_callback = [&selected](bool) { selected = CtxMenuSelection::CTX_SEL_DEL; };

    if(view != nullptr)
    {
        // Context menu!
        QAction* edit_ptr = new QAction(tr("&Edit"), view);
        std::unique_ptr<QAction> edit_act(edit_ptr);
        edit_act->setStatusTip(tr("Edit the selected exercise"));
        connect(edit_act.get(), &QAction::triggered, edit_callback);

        QAction* del_ptr = new QAction(tr("&Delete"), view);
        std::unique_ptr<QAction> del_act(del_ptr);
        del_act->setStatusTip(tr("Delete the selected exercise"));
        connect(del_act.get(), &QAction::triggered, del_callback);

        QMenu ctx(view);
        ctx.addAction(edit_act.get());
        ctx.addAction(del_act.get());
        ctx.exec(view->mapToGlobal(point));

        if(selected != CtxMenuSelection::CTX_SEL_NONE)
        {
            // Look up the cell of the point, retrieve full exercise, and edit it
            auto cellIdx = view->indexAt(point);
            ExerciseSelectTableModel* view_model = qobject_cast<ExerciseSelectTableModel*>(view->model());
            ExerciseInformation exercise = view_model->getExercise(cellIdx.row());
            switch(selected)
            {
            case CtxMenuSelection::CTX_SEL_EDIT:
                editExercise(exercise);
                _database->updateExerciseInformation(exercise);
                break;
            case CtxMenuSelection::CTX_SEL_DEL:
                _database->deleteExerciseInformation(exercise);
                break;
            }
        }
    }
}

void ExerciseSelectDialog::editExercise(ExerciseInformation &info)
{
    auto info_dlg = std::make_unique<ExerciseInfoDialog>(new ExerciseInfoDialog(this));
    if(info_dlg)
    {
        info_dlg->setElements(info.name, info.favorite);
        if(info_dlg->exec() == QDialog::Accepted)
        {
            info.name = info_dlg->getName();
            info.favorite = info_dlg->isFavorite();
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

void ExerciseSelectTableModel::updateExercises()
{
    int prev_col_count = this->columnCount(this->createIndex(0,0,nullptr));
    _exercises = _database->getExercises(_only_favorites);
    emit dataChanged(this->createIndex(0,0,nullptr),
                     this->createIndex(1,prev_col_count, nullptr));
}
