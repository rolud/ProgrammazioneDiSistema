#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QByteArray>
#include <QHostAddress>
#include <QList>
#include "include/client.h"
#include "include/message.h"
#include "include/usermessage.h"
#include "include/authenticationmessage.h"
#include "include/documentmessage.h"
#include "include/document.h"
#include <QThread>
#include <thread>
#include "include/dbinterface.h"


class Server : public QTcpServer
{
    Q_OBJECT

    public:
        explicit Server( QObject* parent = nullptr );
        ~Server();

        Server( const Server& ) = delete; // Elimina il costruttore di copia
        Server& operator=( const Server& ) = delete; // Elimina l'operatore di assegnazione

        void start(); // Mette in ascolto il server sui suoi attibuti privati port e address


    protected:
        // Metodo chiamato tutte le volte che un client si connette al server
        // Il socketDescriptor identifica la socket aperta alla connessione del client
        void incomingConnection( const qintptr socketDescriptor ) override;

        void authenticateUser( Client* const client, const QString& userName, const QString& password );
        void registerNewUser( Client* const client, const QString& userName, const QString& password, const int icon );
        void updateUserData( Client* const client, const QString& userName, const QString& password, const int icon );
        void logoutUser( Client* const client );

        void sendDocumentsListToClient( Client* const client );

        void openDocument( const QString& documentName );
        void selectDocument( Client* const client, const QString& documentName );

    private:
        const quint16 port = 8080; // Porta su cui si mette in ascolto il server
        const QHostAddress address = QHostAddress::LocalHost; // Indirizzo su cui si mette in ascolto il server
        QMap< QString, Document* > openDocumentsMap;
        DBinterface db;

        const int numThreads = QThread::idealThreadCount(); // Numero di thread disponibili
        int nextThread = 0; // Indice del thread a cui verrà assegnato il prossimo documento
        QVector< QThread* > threads; // Thread utilizzati per gestire i documenti e gli scambi di messaggi tra client nello stesso documento

    signals:
        // void connectClientToDocument( Client* const client );


    public slots:
        void onAuthenticationMessageReceived( Client* const client, const AuthenticationMessage& message );
        void onDocumentMessageReceived( Client* const client, const DocumentMessage& message );
        void onCloseDocument( Document* const document ); // Slot richiamato quando il numero di client connessi al documento è 0 e quindi il documento va chiuso e distrutto


};

#endif // SERVER_H
