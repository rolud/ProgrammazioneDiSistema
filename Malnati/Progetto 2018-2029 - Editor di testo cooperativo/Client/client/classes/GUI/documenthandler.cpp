#include "include/documenthandler.h"
#include "ui_documenthandler.h"


DocumentHandler::DocumentHandler(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DocumentHandler)
{
    ui->setupUi(this);

    QPixmap pixmap(":/icons/icons/account.png");
    QIcon ButtonIcon(pixmap);
    ui->UpdateProfilePushButton->setIcon(ButtonIcon);
    ui->UpdateProfilePushButton->setIconSize(pixmap.rect().size());

}

DocumentHandler::~DocumentHandler()
{
    delete ui;
}


void DocumentHandler::on_CreateNewDocument_returnPressed()
{
    this->title=ui->CreateNewDocument->text();

    if ( this->title != nullptr && !this->title.isEmpty() )
    {
        emit createNewDocument(title);
    }

}



void DocumentHandler::on_SelectByURL_returnPressed()
{
    this->URL=ui->SelectByURL->text();

    if ( this->URL != nullptr && !this->URL.isEmpty() )
    {
        emit selectByURL(URL);
    }

}


void DocumentHandler::onDocumentsListReceived( const QList< QString > documentsList )
{
    QString userDocument;
    QString titleDocument;

    for (int i = 0; i < documentsList.size(); i++){

        userDocument = documentsList.at(i).split( "/" )[ 0 ];
        titleDocument = documentsList.at(i).split( "/" )[ 1 ];

        if (userDocument!=nullptr && titleDocument!=nullptr){

            documentListModel->appendRow(new QStandardItem(QIcon(":/icons/icons/document_white.png"), titleDocument + " by " + userDocument));
        }

    }


   ui->DocumentList->setModel(documentListModel);
   ui->DocumentList->setEditTriggers(QAbstractItemView::NoEditTriggers);

   ui->label_3->setText("Welcome, " + this->user);

   this->show();

}



void DocumentHandler::on_DocumentList_doubleClicked(const QModelIndex &index)
{
    QString user;
    QString document;

    QStandardItem *item = documentListModel->itemFromIndex(index);

    if (item!=nullptr){
        user = item->text().split( " by " )[ 1 ];
        document = item->text().split( " by " )[ 0 ];

        QString path= user + "/" + document;

        qDebug() << "Document" << path << "selected";

        emit openDocument(path);

    }
}


void DocumentHandler::setUsername(const QString& username){

    this->user=username;
    this->ui->label_3->setText("Welcome, " + this->user);
}


void DocumentHandler::on_EditProfilePushButton_clicked()
{

}

void DocumentHandler::clear(){

    title="";
    URL="";

    ui->SelectByURL->clear();
    ui->CreateNewDocument->clear();

    documentListModel->clear();
    ui->DocumentList->reset();

}





void DocumentHandler::on_UpdateProfilePushButton_clicked()
{
    emit updateProfile ("Update Profile");
}
