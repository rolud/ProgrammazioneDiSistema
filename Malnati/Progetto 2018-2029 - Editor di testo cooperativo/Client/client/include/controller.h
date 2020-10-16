#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QTimer>
#include "include/client.h"
#include "include/editor.h"
#include "include/logindialog.h"
#include "include/registerdialog.h"
#include "include/errordialog.h"
#include "include/documenthandler.h"
#include "include/iconselection.h"
#include "include/connectdialog.h"
#include "include/startupdialog.h"


/* Classe che agisce da intermediario tra i vari messaggi che arrivano
 * dalla rete e la visualizzazione sulle interfacce grafiche */
class Controller : public QObject
{
    Q_OBJECT

    public:
        explicit Controller( QObject* parent = nullptr );
        ~Controller();

        Controller( const Controller& controller ) = delete; // Elimina il costruttore di copia
        Controller& operator=( const Controller& controller ) = delete; // Elimina l'operatore di assegnazione


    protected:


    private:
        Client client;

        QTimer timer;

        /** GUI **/
        StartupDialog startupDialog;
        ConnectDialog connectDialog;
        LoginDialog loginDialog;
        RegisterDialog registerDialog;
        RegisterDialog updateDialog;
        IconSelection iconSelection;
        ErrorDialog errorDialog;
        DocumentHandler documentHandler;
        Editor editor;


    public slots:
        void showStartupDialog();
        void showConnectDialog();
        void showLoginDialog();
        void showDocumentHandlerDialog( const QList< QString >& documentsList );
        void showEditor( const QString& documentName );
        void showUpdateProfileDialog();

        void start();

        void onUserLogout();
        void onUserDataUpdatedSuccessfully();






    signals:

};

#endif // CONTROLLER_H
