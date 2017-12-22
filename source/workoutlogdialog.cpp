#include <memory>

#include <QMenu>

#include "common.h"
#include "workoutlogdialog.h"
#include "setinfodialog.h"

#include "ui_workoutlogdialog.h"

WorkoutLogDialog::WorkoutLogDialog(QWidget *parent, DBManager* dbManager) :
    QDialog(parent),
    ui(new Ui::WorkoutLogDialog),
    _database(dbManager),
    _dataModel(dbManager)
{
    ui->setupUi(this);

    ui->log_view->setModel(&_dataModel);
    ui->log_view->verticalHeader()->hide();
    ui->log_view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->log_view->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->log_view->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->log_view->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    connect(ui->log_view, &QTableView::customContextMenuRequested,
            this, &WorkoutLogDialog::contextMenu);
    connect(_database, &DBManager::setsUpdated, &_dataModel, &WorkoutLogTableModel::updateSets);
}

WorkoutLogDialog::~WorkoutLogDialog()
{
    delete ui;
}

void WorkoutLogDialog::editSet(SetInformation &info)
{
    auto info_dlg = new SetInfoDialog(this, this->_database);
    if(info_dlg)
    {
        SetDisplayInformation disp_info = SetDisplayInformation::fromSetInfo(info, _database);
        info_dlg->setElements(disp_info);
        if(info_dlg->exec() == QDialog::Accepted)
        {
            info = info_dlg->getSet();
            info.id = disp_info.id;
        }
    }

    delete info_dlg;
}

void WorkoutLogDialog::contextMenu(const QPoint& point)
{
    QObject* sender = this->sender();
    QTableView* view = qobject_cast<QTableView*>(sender);
    CtxMenuSelection selected = CtxMenuSelection::CTX_SEL_NONE;

    auto edit_callback = [&selected](bool) { selected = CtxMenuSelection::CTX_SEL_EDIT; };
    auto del_callback = [&selected](bool) { selected = CtxMenuSelection::CTX_SEL_DEL; };

    if(view != nullptr)
    {
        QAction* edit_ptr = new QAction(tr("&Edit"), view);
        std::unique_ptr<QAction> edit_act(edit_ptr);
        edit_act->setStatusTip(tr("Edit the selected set"));
        connect(edit_act.get(), &QAction::triggered, edit_callback);

        QAction* del_ptr = new QAction(tr("&Delete"), view);
        std::unique_ptr<QAction> del_act(del_ptr);
        del_act->setStatusTip(tr("Delete the selected set"));
        connect(del_act.get(), &QAction::triggered, del_callback);

        QMenu ctx(view);
        ctx.addAction(edit_act.get());
        ctx.addAction(del_act.get());
        ctx.exec(view->mapToGlobal(point));

        if(selected != CtxMenuSelection::CTX_SEL_NONE)
        {
            auto cellIdx = view->indexAt(point);
            WorkoutLogTableModel* view_model = qobject_cast<WorkoutLogTableModel*>(view->model());
            if(view_model)
            {
              SetDisplayInformation set_display = view_model->getSet(cellIdx.row());
              SetInformation set = SetInformation::fromDisplayInfo(set_display, _database);

              switch(selected)
              {
              case CtxMenuSelection::CTX_SEL_EDIT:
                editSet(set);
                _database->updateSetInformation(set);
                break;
              case CtxMenuSelection::CTX_SEL_DEL:
                _database->deleteSetInformation(set);
                break;
              default:
                // Do nothing
                break;
              }
            }
            else
            {
                // Something's gone pretty wrong...
                // TODO: Assert?
            }
        }
    }
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
    return (Qt::ItemIsSelectable | QAbstractTableModel::flags(index));
}

int WorkoutLogTableModel::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return 4;
}

int WorkoutLogTableModel::rowCount(const QModelIndex &parent) const
{
    (void)parent;
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
                value = set_info.timestamp;
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
            value = tr("Timestamp");
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

SetDisplayInformation WorkoutLogTableModel::getSet(int row)
{
    return this->_sets.at(row);
}

void WorkoutLogTableModel::updateSets()
{
    int prev_col_count = this->columnCount(this->createIndex(0,0,nullptr));
    _sets = _database->getAllDisplaySets();
    emit dataChanged(this->createIndex(0,0,nullptr),
                     this->createIndex(1,prev_col_count, nullptr));
}
