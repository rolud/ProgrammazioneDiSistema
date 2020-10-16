#include <iostream>
#include <stdexcept>
#include <string>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QObject>
#include <QtNetwork/QHostAddress>
#include <QByteArray>
#include <QDebug>
#include <QString>
#include <QJsonDocument>
#include <QJsonParseError>
#include <thread>

#include "include/client.h"
#include "include/message.h"
#include "include/charactermessage.h"
#include "include/usermessage.h"
#include "include/authenticationmessage.h"
#include "include/documentmessage.h"


Client::Client( QObject* parent ) : QObject( parent ), socket( new QTcpSocket{ this } ), clientId( 0 )
{
    // Connette gli slot a tutti i segnali che può scatenare il socket
    QObject::connect( this->socket, &QTcpSocket::connected, this, &Client::onConnect );
    QObject::connect( this->socket, &QTcpSocket::disconnected, this, &Client::onDisconnect );
    QObject::connect( this->socket, &QTcpSocket::stateChanged, this, &Client::onStateChange );
    QObject::connect( this->socket, QOverload< QTcpSocket::SocketError >::of( &QAbstractSocket::error ), this, &Client::onError );
    QObject::connect( this->socket, &QTcpSocket::readyRead, this, &Client::onReadyRead, Qt::QueuedConnection );
    // QObject::connect( this->socket, &QTcpSocket::bytesWritten, this, &Client::onBytesWritten );
}


qint32 Client::getClientId() const
{
    return this->clientId;
}

void Client::setClientId( const qint32 clientId )
{
    if ( clientId <= 0 )
    {
        throw std::invalid_argument{ "clientId in class Client cannot be <= 0" };
    }

    this->clientId = clientId;
    qDebug() << "ClientId:" << clientId;
}

QString Client::getUserName() const
{
    return this->username;
}

QString Client::getPassword() const
{
    return this->password;
}

int Client::getIcon() const
{
    return this->icon;
}

// Connette il client al server. Se l'operazione non riesce entro 6s
// ci riprova fino a che non si connette.
void Client::connectToServer()
{    
    if ( this->socket->state() == QAbstractSocket::ConnectedState )
    {
        return;
    }


    this->socket->connectToHost( this->serverAddress, this->serverPort );

    while ( !this->socket->waitForConnected( 1000 ) )
    {
        // qDebug() << "\n" "Retrying to connect to the server...";
        // this->socket->connectToHost( this->serverAddress, this->serverPort );
        emit this->connectionError();
        return;
    }

    while ( !this->socket->waitForReadyRead( 1000 ) )
    {
        qDebug() << "\n" "Waiting for client id...";
    }

    try
    {
        this->readClientId();
    }

    catch ( ... )
    {
        this->disconnectFromServer();
    }

}

void Client::disconnectFromServer()
{
    this->socket->disconnectFromHost();
}

bool Client::isConfigurationFinished() {
    return this->configurationFinished;
}

/** SLOTS **/

// Legge i dati dal socket in risposta al segnale readyRead().
// Legge prima la dimensione del documento Json e successivamente i
// byte relativi al documento. Infine converte il documento in un
// oggetto Message che viene segnalato all'editor.
void Client::onReadyRead()
{
    // Usato come vettore di byte per leggere la dimensione del documento Json dal socket
    static char dataSize[ 4 ];
    static qint32 dataSizeLeft = 4;

    static QByteArray incompleteData{};
    static qint32 bytesToReadLeft = 0;


    if ( socket->atEnd() )
    {
        return;
    }


    try
    {
        qint32 bytesToRead = 0;

        if ( bytesToReadLeft > 0 )
        {
            bytesToRead = bytesToReadLeft;
        }

        else
        {
            // Legge prima la dimensione del documento Json che è un intero 32bit
            qint64 bytesRead = socket->read( &dataSize[ 4 - dataSizeLeft ], dataSizeLeft );

            if ( bytesRead != dataSizeLeft )
            {
                //qDebug() << bytesRead;
                //throw std::invalid_argument{ "bytesRead != 4 while reading Json document size." };
                dataSizeLeft -= bytesRead;
                return;
            }

            // Converte i byte in intero 32bit
            memcpy( &bytesToRead, dataSize, 4 );
            dataSizeLeft = 4;

            if ( bytesToRead <= 0 )
            {
                throw std::invalid_argument{ "invalid bytesToRead value." };
            }
        }



        QByteArray data = socket->read( bytesToRead );

        if ( data.size() != bytesToRead )
        {
            //qDebug() << "\n" "in: bytesToRead:" << bytesToRead << "byte";
            //qDebug() << "in: size:" << data.size() << "byte";
            //qDebug() << "in:" << data;
            // throw std::invalid_argument{ "data size is different from bytesToRead." };

            bytesToReadLeft = bytesToRead - data.size();
            incompleteData.append( data );

            return;
        }

        else
        {
            data.prepend( incompleteData );

            bytesToReadLeft = 0;
            incompleteData.clear();
        }

        const QJsonDocument json = QJsonDocument::fromBinaryData( data );

        // Se la conversione fallisce
        if ( json.isNull() || json.isEmpty() || !json.isObject() )
        {
            throw std::invalid_argument{ "invalid QJsonDocument format." };
        }

        // qDebug() << "\n" "in: size" << bytesToRead << "byte";
        // qDebug() << "in:" << json << "\n";

        const QJsonObject jsonObject = json.object();

        if ( !jsonObject.contains( "message_type" ) || !jsonObject[ "message_type" ].isDouble() )
        {
            throw std::invalid_argument{ "QJsonObject does not contain 'message_type'." };
        }

        if ( !jsonObject.contains( "message" ) || !jsonObject[ "message" ].isObject() )
        {
            throw std::invalid_argument{ "QJsonObject does not contain 'message'." };
        }

        const Message::MessageType messageType = Message::MessageType( jsonObject[ "message_type" ].toInt() );
        const QJsonObject& messageJsonObject = jsonObject[ "message" ].toObject();

        switch ( messageType )
        {
            case Message::MessageType::CHARACTER:
            {
                CharacterMessage characterMessage = CharacterMessage::fromJsonObject( messageJsonObject );

                // Controlla che il messaggio non sia stato mandato da questo client
                if ( !this->isConfigurationFinished() || characterMessage.getClientId() != this->getClientId() )
                {
                    emit this->characterMessageReceived( characterMessage );
                }

                break;
            }

            case Message::MessageType::USER:
            {
                UserMessage userMessage = UserMessage::fromJsonObject( messageJsonObject );

                // Controlla che il messaggio non sia stato mandato da questo client
                if ( userMessage.getClientId() != this->getClientId() )
                {
                    emit this->userMessageReceived( userMessage );
                }

                break;
            }

            case Message::MessageType::AUTHENTICATION:
            {
                AuthenticationMessage authenticationMessage = AuthenticationMessage::fromJsonObject( messageJsonObject );

                if ( authenticationMessage.getClientId() == this->getClientId() )
                {
                    switch ( authenticationMessage.getEventType() )
                    {
                        case AuthenticationMessage::EventType::LOGIN:
                        {
                            if ( authenticationMessage.isOperationSuccess() )
                            {

                                this->authenticated = true;
                                this->username = authenticationMessage.getUserName();
                                this->password = authenticationMessage.getPassword();
                                this->icon = authenticationMessage.getIcon();
                                emit this->userAuthenticatedSuccessfully( this->username, this->icon );
                            }
                            else
                            {
                                emit this->userAuthenticationError( authenticationMessage.getServerMessage() );
                            }
                        }
                        break;

                        case AuthenticationMessage::EventType::REGISTER:
                        {
                            if ( authenticationMessage.isOperationSuccess() )
                            {

                                this->authenticated = true;
                                this->username = authenticationMessage.getUserName();
                                this->password = authenticationMessage.getPassword();
                                this->icon = authenticationMessage.getIcon();
                                emit this->userRegisteredSuccessfully( this->username, this->icon );
                            }
                            else
                            {
                                emit this->userRegistrationError( authenticationMessage.getServerMessage() );
                            }
                        }
                        break;

                        case AuthenticationMessage::EventType::UPDATE:
                        {
                            if ( authenticationMessage.isOperationSuccess() )
                            {

                                this->authenticated = true;
                                this->username = authenticationMessage.getUserName();
                                this->password = authenticationMessage.getPassword();
                                this->icon = authenticationMessage.getIcon();
                                emit this->userUpdatedSuccessfully();
                            }
                            else
                            {
                                emit this->userUpdateError( authenticationMessage.getServerMessage() );
                            }
                        }
                        break;

                        default:
                            throw std::invalid_argument{ "AuthenticationMessage received with invalid EventType." };
                            break;
                    }

                }

                else
                {
                    throw std::invalid_argument{ "AuthenticationMessage received from another client." };
                }

                break;
            }

            case Message::MessageType::DOCUMENT:
            {
                DocumentMessage documentMessage = DocumentMessage::fromJsonObject( messageJsonObject );

                if ( this->clientId == documentMessage.getClientId() )
                {
                    switch ( documentMessage.getEventType() )
                    {
                        case DocumentMessage::SERVER_RESPONSE:
                        {
                            if ( documentMessage.isOperationSuccess() )
                            {
                                qDebug() << "Document selected successfully";
                                emit this->documentSelectedSuccessfully( documentMessage.getSelectedDocumentName() );

                            }

                            else
                            {
                                qDebug() << "Document selection error" << documentMessage.getServerMessage();
                                emit this->documentSelectionError( documentMessage.getServerMessage() );
                            }

                            break;
                        }

                        default:
                        {
                            qDebug() << "Documents list received" << documentMessage.getDocumentsList();
                            emit this->documentsListReceived( documentMessage.getDocumentsList() );
                            break;
                        }
                    }
                }

                break;
            }

            case Message::MessageType::CONFIGURATION:
            {
                // ricezione messaggio ConfigurationMessagge con il mio client id --> configurazione finita
                ConfigurationMessage message = ConfigurationMessage::fromJsonObject(messageJsonObject);
                if (message.getClientId() == this->getClientId()) {
                    qDebug() << " --- configuration finished";
                    switch (message.getConfigStatus()) {
                        case ConfigurationMessage::ConfigStatus::SUCCESS:
                            this->configurationFinished = true;
                            break;
                        case ConfigurationMessage::ConfigStatus::FAILURE:
                            break;
                    }
                    emit configurationEnded();
                }
                break;
            }

            default:
            {
                throw std::invalid_argument{ "Invalid messageType." };
            }

        }


        // Se ci sono altri dati da leggere sul socket rilancia il segnale.
        if ( !this->socket->atEnd() )
        {
            emit this->socket->readyRead();
        }
    }

    catch ( std::exception& e )
    {
        qDebug() << "Exception:" << e.what();
        this->disconnectFromServer();
    }

    catch ( ... )
    {
        this->disconnectFromServer();
    }


}

// Legge il clientId dal socket, che deve essere un intero su 32bit.
// In caso di incongruenze scatena un'eccezione.
void Client::readClientId()
{
    // Usato come vettore di byte per leggere i 4 byte del client_id
    char bytesBuffer[ 4 ];
    qint64 bytesRead = this->socket->read( bytesBuffer, 4 );

    if ( bytesRead != 4 )
    {
        qDebug() << bytesRead;
        throw std::invalid_argument{ "bytesRead != 4 while reading client id from socket" };
    }

    qint32 clientId = 0;
    memcpy( &clientId, bytesBuffer, 4 );

    this->setClientId( clientId );
}

void Client::onConnect()
{
    qDebug() << "Connected to server addr:"
             << this->socket->peerAddress().toString()
             << "port:"
             << this->socket->peerPort();

    emit this->clientConnected();
}

void Client::onDisconnect()
{
    qDebug() << "Disconnected from server";
    this->username = "";
    this->icon = -1;
    this->authenticated = false;
    this->configurationFinished = false;
    this->socket->reset();
    emit this->clientDisconnected();
}

void Client::onError( const QAbstractSocket::SocketError socketError )
{
    if ( this->getClientId() != 0 )
    {
        qDebug() << "\n";
    }

    qDebug() << socketError << ":" << this->socket->errorString();
}

void Client::onStateChange( const QAbstractSocket::SocketState socketState )
{
    switch ( socketState )
    {
        case QAbstractSocket::SocketState::UnconnectedState:
        {
            qDebug() << "Socket state changed: The socket is not connected.";
            break;
        }

        case QAbstractSocket::SocketState::ConnectingState:
        {
            qDebug() << "Socket state changed: The socket has started establishing a connection.";
            break;
        }

        case QAbstractSocket::SocketState::BoundState:
        {
            qDebug() << "Socket state changed: The socket is bound to addr: "
                     << this->socket->localAddress().toString()
                     << " port: "
                     << this->socket->localPort();
            break;
        }

        case QAbstractSocket::SocketState::ClosingState:
        {
            qDebug() << "Socket state changed: The socket is about to close.";
            break;
        }

        case QAbstractSocket::SocketState::ConnectedState:
        {
            qDebug() << "Socket state changed: The connection is established.";
            break;
        }

        case QAbstractSocket::SocketState::HostLookupState:
        {
            qDebug() << "Socket state changed: The socket is performing a host name lookup.";
            break;
        }

        case QAbstractSocket::SocketState::ListeningState:
        {
            qDebug() << "Socket state changed: The socket is in listening state.";
            break;
        }

    }

}

void Client::onBytesWritten( const quint64 bytes )
{
    ( void )bytes;
}

// Slot per intercettare il messaggio newCharacterMessage() generato dall'editor
// quando l'utente compie qualche azione.
// L'oggetto Message viene quindi convertito in Json ed inviato al server.
void Client::onNewCharacterMessage( const CharacterMessage& message )
{

    /*
    if (message.getOperationType() == CharacterMessage::OperationType::INSERT)
        qDebug() << "sending msg -> operation INSERT";
    else
        qDebug() << "sending msg -> operation DELETE";
    */

    if (this->configurationFinished) {
        QByteArray data{};
        message.toBinaryJsonMessage( data );

        this->socket->write( data );
        this->socket->flush();
    }
}


void Client::sendLoginRequest( const QString& username, const QString& password )
{
    AuthenticationMessage message{ this->getClientId(), username, AuthenticationMessage::LOGIN, password, 0, false, "" };

    QByteArray data{};
    message.toBinaryJsonMessage( data );

    this->socket->write( data );
    this->socket->flush();
}

void Client::sendRegisterRequest(const QString& username, const QString& password, int icon )
{
    AuthenticationMessage message{ this->getClientId(), username, AuthenticationMessage::REGISTER, password, icon, false, "" };

    QByteArray data{};
    message.toBinaryJsonMessage( data );

    this->socket->write( data );
    this->socket->flush();
}

void Client::sendLogoutRequest( const QString& username )
{
    AuthenticationMessage message{ this->getClientId(), username, AuthenticationMessage::LOGOUT, "", 0, false, "" };

    QByteArray data{};
    message.toBinaryJsonMessage( data );

    this->socket->write( data );
    this->socket->flush();

    this->authenticated = false;
    this->username = "";
    this->password = "";
    this->icon = -1;
    this->configurationFinished = false;

    emit userLoggedoutSuccesfully();
}

void Client::sendUpdateRequest(const QString& username, const QString& password, int icon )
{
    AuthenticationMessage message{ this->getClientId(), username, AuthenticationMessage::UPDATE, password, icon, false, "" };

    QByteArray data{};
    message.toBinaryJsonMessage( data );

    this->socket->write( data );
    this->socket->flush();
}


void Client::sendNewDocumentRequest( const QString& newDocumentName )
{
    const QString documentName = this->getUserName() + "/" + newDocumentName;
    DocumentMessage message{ this->getClientId(), DocumentMessage::NEW_DOCUMENT, QList< QString >{}, "", documentName, "", false, "" };

    QByteArray data{};
    message.toBinaryJsonMessage( data );

    this->socket->write( data );
    this->socket->flush();
}

void Client::sendUrlDocumentRequest( const QString& urlDocument )
{
    DocumentMessage message{ this->getClientId(), DocumentMessage::URL_DOCUMENT, QList< QString >{}, "", "", urlDocument, false, "" };

    QByteArray data{};
    message.toBinaryJsonMessage( data );

    this->socket->write( data );
    this->socket->flush();
}

void Client::sendSelectedDocumentRequest( const QString& selectedDocumentName )
{
    DocumentMessage message{ this->getClientId(), DocumentMessage::SELECTED_DOCUMENT, QList< QString >{}, selectedDocumentName, "", "", false, "" };

    QByteArray data{};
    message.toBinaryJsonMessage( data );

    this->socket->write( data );
    this->socket->flush();
}


