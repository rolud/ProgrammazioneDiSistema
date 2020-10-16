#include "include/registerdialog.h"
#include "ui_registerdialog.h"

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);

}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_SelectIconPushButton_clicked()
{
 emit openIconSelection();
}


void RegisterDialog::on_RegisterPushButton_clicked()
{


  this->username=ui->lineEdit_Choose_Username->text();

  if ( this->username.isNull() || this->username.isEmpty() )
  {
      emit this->incompleteData( "Please insert username." );
      return;
  }

  this->password=ui->lineEdit_Choose_Password->text();

  if ( this->password.isNull() || this->password.isEmpty() )
  {
      emit this->incompleteData( "Please insert password." );
      return;
  }

  if ( this->icon == -1 )
  {
      emit this->incompleteData( "Please select an icon." );
      return;
  }



  emit RegisterRequest(username, password, icon);
}

void RegisterDialog::onUserRegistered()
{
    this->close();
}

void RegisterDialog::clear(){

     ui->lineEdit_Choose_Password->clear();
     ui->lineEdit_Choose_Username->clear();
     ui->IconPreview->clear();

     username="";
     password="";
     icon=-1;
     iconPath="";
     buttonTitle="";

}

void RegisterDialog::setBoxTitle ( const QString& title ){

    this->buttonTitle=title.split(" ")[0];
    ui->RegisterPushButton->setText(buttonTitle);
    ui->groupBox->setTitle(title);

}



void RegisterDialog::showIcon (int icon){


    this->icon=icon;


    switch (this->icon)
    {
        case 1:
            iconPath= ":/icons/user icons/Basket.png";
            break;
        case 2:
            iconPath= ":/icons/user icons/Engineer.png";
            break;
        case 3:
            iconPath= ":/icons/user icons/Eve.png";
            break;
        case 4:
            iconPath= ":/icons/user icons/Football.png";
            break;
        case 5:
            iconPath= ":/icons/user icons/Harley.png";
            break;
        case 6:
            iconPath= ":/icons/user icons/Love.png";
            break;
        case 7:
            iconPath= ":/icons/user icons/Motorsport.png";
            break;
        case 8:
            iconPath= ":/icons/user icons/Music.png";
            break;
        case 9:
            iconPath= ":/icons/user icons/Nature.png";
            break;
        case 10:
            iconPath= ":/icons/user icons/Navy.png";
            break;
        case 11:
            iconPath= ":/icons/user icons/Prize.png";
            break;
        case 12:
            iconPath= ":/icons/user icons/Research.png";
            break;
        case 13:
            iconPath= ":/icons/user icons/Robot.png";
            break;
        case 14:
            iconPath= ":/icons/user icons/Soccer.png";
            break;
        case 15:
            iconPath= ":/icons/user icons/Star.png";
            break;
        case 16:
            iconPath= ":/icons/user icons/Summer.png";
            break;
        case 17:
            iconPath= ":/icons/user icons/Tennis.png";
            break;
        case 18:
            iconPath= ":/icons/user icons/Trip.png";
            break;
        case 19:
            iconPath= ":/icons/user icons/Videogame.png";
            break;
        default:
            iconPath= ":/icons/user icons/Yoga.png";
            break;
    }



    QPixmap pixmap(iconPath);
    //QIcon IconPrev(pixmap);
    ui->IconPreview->setPixmap(pixmap);

}



void RegisterDialog::on_RegisterPushButton_2_clicked()
{
    emit openLoginDialog();
    this->hide();
}

void RegisterDialog::setUserData( const QString& username, const QString& password, const int icon )
{
    this->username = username;
    this->password = password;
    this->icon = icon;

    this->ui->lineEdit_Choose_Username->setText( username );
    this->ui->lineEdit_Choose_Password->setText( password );
    this->showIcon( icon );
}
