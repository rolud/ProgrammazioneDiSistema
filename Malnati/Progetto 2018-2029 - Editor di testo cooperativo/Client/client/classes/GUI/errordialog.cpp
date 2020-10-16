#include "ui_errordialog.h"
#include "include/errordialog.h"


ErrorDialog::ErrorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorDialog)
{
    ui->setupUi(this);



}

ErrorDialog::~ErrorDialog()
{
    delete ui;
}


void ErrorDialog::displayError(const QString& errorMessage ){

ui->label->setText(errorMessage);

ui->label->setTextInteractionFlags(Qt::TextSelectableByMouse);

ui->label->setAlignment(Qt::AlignCenter);

this->show();


}

void ErrorDialog::on_OKButton_clicked()
{
    this->hide();
}
