#include "startwindow.hh"
#include "ui_startwindow.h"
#include "mainwindow.hh"

StartWindow::StartWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);
    ui->playPushButton->setDisabled(true);
}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::on_nameLineEdit_textChanged(const QString &arg1)
{
    if ( arg1.isEmpty() )
    {
        ui->playPushButton->setDisabled(true);
        ui->informationLabel->setText("Name can't be empty");
    }
    else
    {
        ui->playPushButton->setDisabled(false);
        ui->informationLabel->setText("Nice to meet you, " + arg1 + " ! :)");
    }
    name = arg1;
}

void StartWindow::on_playPushButton_clicked()
{
        emit wantstoPlay(name);
}
