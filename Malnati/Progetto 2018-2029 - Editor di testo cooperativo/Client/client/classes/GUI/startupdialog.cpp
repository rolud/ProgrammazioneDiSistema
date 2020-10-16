#include "include/startupdialog.h"
#include "ui_startupdialog.h"

StartupDialog::StartupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartupDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::CustomizeWindowHint);

}

StartupDialog::~StartupDialog()
{
    delete ui;
}

