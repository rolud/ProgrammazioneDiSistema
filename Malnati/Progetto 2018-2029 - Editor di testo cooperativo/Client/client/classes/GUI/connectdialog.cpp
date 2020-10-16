#include "include/connectdialog.h"
#include "ui_connectdialog.h"

#include <QApplication>

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);

}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

void ConnectDialog::on_TryAgain_Button_clicked()
{
    this->hide();
    emit this->tryAgain();

}

void ConnectDialog::on_OKButton_2_clicked()
{
    QApplication::quit();
}
