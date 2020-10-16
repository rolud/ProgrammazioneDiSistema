#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include "include/authenticationmessage.h"
#include "include/documentmessage.h"


class Client : public QTcpSocket
{
    Q_OBJECT

    public:
        Client( const qintptr socketDescriptor, QObject* parent = nullptr );
        ~Client();

        Client( const Client& ) = delete; // Elimina il costruttore di copia
        Client& operator=( const Client& ) = delete; // Elimina l'operatore di assegnazione

        int getClientId() const;
        QString getUserName() const;
        bool isAuthenticated() const;
        int getUserId() const;
        int getIcon() const;

        void setAuthenticated( const bool isAuthenticated );
        void setUserId( const int id);
        void setUserName( const QString& userName );
        void setIcon( const int icon );

    protected:
        // Invia al client connesso a questo socket il suo clientId.
        // Questo id servirà per identificare i caratteri scritti da ciascun client.
        void sendClientId();


    private:
        const qintptr socketDescriptor; // Id utilizzato per identificare il socket.
        const int clientId;
        QString userName;
        bool authenticated; // bool per mantenere lo stato del client.
        int userId;
        int icon;


    signals:
        // void clientConnected( const Client* const client );
        void clientDisconnected( Client* const client );

        void characterMessageReceived( const QByteArray& message );
        void authenticationMessageReceived( Client* const client, const AuthenticationMessage& message );
        void documentMessageReceived( Client* const client, const DocumentMessage& message );

        void disconnectClientFromDocument( Client* const client );

        void editMessageReceived( const QByteArray& message );


    public slots:
        // Slots per intercettare tutti i segnali standard di QTcpSocket
        // per tenere traccia dello stato del socket.
        void onReadyRead();
        void onWriteData( const QByteArray& data );
        void onConnect();
        void onDisconnect();
        void onError( const QAbstractSocket::SocketError socketError );
        void onStateChange( const QAbstractSocket::SocketState socketState );
        void onBytesWritten( const quint64 bytes );

};

#endif // SOCKET_H
