#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QHostAddress>

#include "include/algorithmcrdt.h"
#include "include/message.h"
#include "include/authenticationmessage.h"
#include "include/configurationmessage.h"
#include "include/charactermessage.h"
#include "include/usermessage.h"


class Client : public QObject
{
    Q_OBJECT

    public:
        explicit Client( QObject* parent = nullptr );

        Client( const Client& client ) = delete; // Elimina il costruttore di copia
        Client& operator=( const Client& client ) = delete; // Elimina l'operatore di assegnazione

        // Il clientId viene fornito dal server al momento della connessione
        qint32 getClientId() const;
        void setClientId( const qint32 clientId );
        QString getUserName() const;
        int getIcon() const;
        QString getPassword() const;
        bool isConfigurationFinished();


    protected:
        // Legge il clientId dal socket al momento della connessione
        void readClientId();


    private:
        QTcpSocket* const socket; // Socket utilizzato per comunicare con il server.
        const QHostAddress serverAddress = QHostAddress::LocalHost;
        const quint16 serverPort = 8080;
        qint32 clientId = 0;

        bool configurationFinished = false;

        // Dati dell'utente quando autenticato
        bool authenticated = false;
        QString username;
        QString password;
        int icon;


    signals:
        void clientConnected();
        void clientDisconnected();

        void connectionError();

        void characterMessageReceived( const CharacterMessage& characterMessage );
        void userMessageReceived( const UserMessage& userMessage );
        void authenticationMessageReceived( const AuthenticationMessage& authenticationMessage );

        void userAuthenticatedSuccessfully( const QString& username, const int icon );
        void userLoggedoutSuccesfully();
        void userAuthenticationError( const QString& serverMessage );
        void userRegisteredSuccessfully( const QString& username, const int icon );
        void userRegistrationError( const QString& serverMessage );
        void userUpdatedSuccessfully();
        void userUpdateError( const QString& serverMessage );

        void documentsListReceived( const QList< QString > documentsList );
        void documentSelectedSuccessfully( const QString& documentName );
        void documentSelectionError( const QString& serverMessage );

        void configurationEnded();


    public slots:
        // Connette il client al server all'indirizzo e porta specificati negli attributi privati.
        void connectToServer();
        void disconnectFromServer();


        // Slots per intercettare i segnali del socket
        void onConnect();
        void onDisconnect();
        void onError( const QAbstractSocket::SocketError socketError );
        void onStateChange( const QAbstractSocket::SocketState socketState );
        void onReadyRead();
        void onBytesWritten( const quint64 bytes );

        // Slot per intercettare nuovi messaggi dall'editor ed inviarli al server convertiti in Json
        void onNewCharacterMessage( const CharacterMessage& message );

        // Slot per intercettare i segnali inviati dall'interfaccia di login
        // creano il messaggio di autenticazione corrispondente e lo inviano al server
        void sendLoginRequest( const QString& username, const QString& password );
        void sendRegisterRequest( const QString& username, const QString& password, int icon );
        void sendLogoutRequest( const QString& username );
        void sendUpdateRequest( const QString& username, const QString& password, int icon );

        // Slot per intercettare i segnali inviati dall'interfaccia di selezione del documento
        // creano il messaggio Document corrispondente e lo inviano al server
        void sendNewDocumentRequest( const QString& newDocumentName );
        void sendUrlDocumentRequest( const QString& urlDocument );
        void sendSelectedDocumentRequest( const QString& documentName );
};

#endif // CLIENT_H
