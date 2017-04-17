#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->countdown_control, SIGNAL(clicked(bool)), this, SLOT(handleCountdownControl(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleCountdownControl(bool clicked)
{
    (void)clicked;
    // assume a toggle

    if(countdown_state)
    {
        ui->countdown_control->setText(tr("Start"));
    }
    else
    {
        ui->countdown_control->setText(tr("Pause"));
    }
    countdown_state = !countdown_state;
}
