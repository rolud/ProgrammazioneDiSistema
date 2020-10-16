#include "include/logindialog.h"
#include "ui_logindialog.h"



LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

}

LoginDialog::~LoginDialog()
{
    delete ui;
}




void LoginDialog::on_LoginPushButton_clicked()
{

    this->username=ui->lineEdit_UserName->text();
    this->password=ui->lineEdit_Password->text();

    if ( this->username.isNull() || this->username.isEmpty() )
    {
        emit this->incompleteData( "Please insert username." );
        return;
    }

    if ( this->password.isNull() || this->password.isEmpty() )
    {
        emit this->incompleteData( "Please insert password." );
        return;
    }



    emit loginRequest( username, password );

}

void LoginDialog::on_RegisterPushButton_clicked()
{

    this->hide();
    emit openRegisterDialog("Create Profile");


}

void LoginDialog::onUserAuthenticated()
{
    this->close();
}


void LoginDialog::clear(){

     ui->lineEdit_Password->clear();
     ui->lineEdit_UserName->clear();
     username="";
     password="";

}
