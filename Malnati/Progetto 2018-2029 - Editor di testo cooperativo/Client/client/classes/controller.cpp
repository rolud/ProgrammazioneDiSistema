#include "include/controller.h"
#include <QObject>
#include <QApplication>
#include <QThread>
#include <QTimer>

Controller::Controller(QObject *parent) : QObject(parent), timer()
{
    QObject::connect( &timer, &QTimer::timeout, this, &Controller::start );

    QObject::connect( &client, &Client::clientDisconnected, this, &Controller::showConnectDialog );
    QObject::connect( &client, &Client::clientConnected, &startupDialog, &StartupDialog::close);
    QObject::connect( &client, &Client::clientConnected, this, &Controller::showLoginDialog );
    QObject::connect( &client, &Client::connectionError, this, &Controller::showConnectDialog );
    QObject::connect( &connectDialog, &ConnectDialog::tryAgain, &client, &Client::connectToServer );

    QObject::connect( &loginDialog, &LoginDialog::loginRequest, &client, &Client::sendLoginRequest );
    QObject::connect( &loginDialog, &LoginDialog::openRegisterDialog, &registerDialog, &RegisterDialog::setBoxTitle);
    QObject::connect( &loginDialog, &LoginDialog::openRegisterDialog, &registerDialog, &RegisterDialog::show );
    QObject::connect( &loginDialog, &LoginDialog::incompleteData, &errorDialog, &ErrorDialog::displayError );


    QObject::connect( &registerDialog, &RegisterDialog::RegisterRequest, &client, &Client::sendRegisterRequest );
    QObject::connect( &registerDialog, &RegisterDialog::incompleteData, &errorDialog, &ErrorDialog::displayError );
    QObject::connect( &registerDialog, &RegisterDialog::openIconSelection, &iconSelection, &IconSelection::show );
    QObject::connect( &iconSelection, &IconSelection::iconSelected, &registerDialog, &RegisterDialog::showIcon );


    QObject::connect( &registerDialog, &RegisterDialog::openLoginDialog, &loginDialog, &LoginDialog::show );

    QObject::connect( &client, &Client::userAuthenticationError, &errorDialog, &ErrorDialog::displayError);
    QObject::connect( &client, &Client::userRegistrationError, &errorDialog, &ErrorDialog::displayError);

    QObject::connect( &client, &Client::documentSelectionError, &errorDialog, &ErrorDialog::displayError );
    QObject::connect( &client, &Client::documentsListReceived, this, &Controller::showDocumentHandlerDialog );
    QObject::connect( &documentHandler, &DocumentHandler::createNewDocument, &client, &Client::sendNewDocumentRequest );
    QObject::connect( &documentHandler, &DocumentHandler::selectByURL, &client, &Client::sendUrlDocumentRequest );
    QObject::connect( &documentHandler, &DocumentHandler::openDocument, &client, &Client::sendSelectedDocumentRequest );

    QObject::connect( &documentHandler, &DocumentHandler::updateProfile, this, &Controller::showUpdateProfileDialog );
    QObject::connect( &updateDialog, &RegisterDialog::openIconSelection, &iconSelection, &IconSelection::show );
    QObject::connect( &iconSelection, &IconSelection::iconSelected, &updateDialog, &RegisterDialog::showIcon );
    QObject::connect( &updateDialog, &RegisterDialog::RegisterRequest, &client, &Client::sendUpdateRequest );
    QObject::connect( &client, &Client::userUpdateError, &errorDialog, &ErrorDialog::displayError );
    QObject::connect( &client, &Client::userUpdatedSuccessfully, this, &Controller::onUserDataUpdatedSuccessfully );
    QObject::connect( &updateDialog, &RegisterDialog::incompleteData, &errorDialog, &ErrorDialog::displayError );

    QObject::connect( &client, &Client::documentSelectedSuccessfully, this, &Controller::showEditor );
    QObject::connect( &editor, &Editor::newCharacterMessage, &client, &Client::onNewCharacterMessage );
    QObject::connect( &client, &Client::characterMessageReceived, &editor, &Editor::onCharacterMessageReceived );
    QObject::connect( &client, &Client::userMessageReceived, &editor, &Editor::onUserMessageReceived );
    QObject::connect( &client, &Client::configurationEnded, &editor, &Editor::onConfigurationEnded );
    QObject::connect( &editor, &Editor::displayURL, &errorDialog, &ErrorDialog::displayError);

    QObject::connect( &editor, &Editor::requestLogout, &client, &Client::sendLogoutRequest);
    QObject::connect( &client, &Client::userLoggedoutSuccesfully, this, &Controller::onUserLogout);
}

Controller::~Controller()
{
}

void Controller::showStartupDialog()
{
    QApplication::closeAllWindows();
    this->startupDialog.show();
    this->timer.start( 2000 );
}

void Controller::start()
{
    this->timer.stop();
    this->client.connectToServer();
}

void Controller::onUserLogout()
{
    QApplication::closeAllWindows();

    this->showLoginDialog();
}

void Controller::showLoginDialog()
{
    this->loginDialog.clear();
    this->registerDialog.clear();

    QApplication::closeAllWindows();

    this->loginDialog.show();
}

void Controller::showDocumentHandlerDialog( const QList< QString >& documentsList )
{
    QApplication::closeAllWindows();

    this->documentHandler.clear();
    this->documentHandler.setUsername( this->client.getUserName() );
    this->documentHandler.onDocumentsListReceived( documentsList );
}

void Controller::showEditor( const QString& documentName )
{
    this->editor.clear();
    this->editor.setClientId( this->client.getClientId() );
    this->editor.setUsername( client.getUserName() );
    this->editor.setIcon( this->client.getIcon() );

    QApplication::closeAllWindows();

    this->editor.onDocumentSelectedSuccessfully( documentName );
}

void Controller::showConnectDialog()
{
    QApplication::closeAllWindows();
    this->connectDialog.show();
}

void Controller::showUpdateProfileDialog()
{
    this->updateDialog.clear();
    this->updateDialog.setUserData( this->client.getUserName(), this->client.getPassword(), this->client.getIcon() );
    this->updateDialog.setBoxTitle( "Update Profile" );
    this->updateDialog.show();
}

void Controller::onUserDataUpdatedSuccessfully()
{
    this->documentHandler.setUsername( client.getUserName() );
    this->updateDialog.close();
    this->errorDialog.displayError( "User data updated succesfully." );
}







