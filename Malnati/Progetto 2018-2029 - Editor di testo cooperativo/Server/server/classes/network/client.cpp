#include "include/client.h"
#include "include/usermessage.h"
#include "include/charactermessage.h"
#include "include/errormessage.h"
#include "include/editmessage.h"
#include "include/authenticationmessage.h"
#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QDebug>
#include <QHostAddress>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <iterator>
#include <iostream>
#include <thread>



Client::Client( const qintptr socketDescriptor, QObject* parent ) : QTcpSocket( parent ), socketDescriptor( socketDescriptor ), clientId( socketDescriptor ), userName(), authenticated( false ), userId(-1), icon( -1 )
{

    // Connessione di tutti i segnali standard di QTcpSocket con gli slot di questa classe,
    // in modo da tenere traccia dello stato del socket
    QObject::connect( this, &QTcpSocket::readyRead, this, &Client::onReadyRead, Qt::DirectConnection );
    QObject::connect( this, &QTcpSocket::connected, this, &Client::onConnect );
    QObject::connect( this, &QTcpSocket::disconnected, this, &Client::onDisconnect );
    // QObject::connect( this, &QTcpSocket::stateChanged, this, &Client::onStateChange );
    QObject::connect( this, QOverload< QTcpSocket::SocketError >::of( &QAbstractSocket::error ), this, &Client::onError );
    // QObject::connect( this, &QTcpSocket::bytesWritten, this, &Client::onBytesWritten );

    // Setta il socketDescriptor; questa operazione è necessaria per permettere al socket
    // di leggere e di scrivere. Se l'operazione fallisce per qualche motivo
    // emette il segnale di errore e marca l'oggetto per essere distrutto automaticamente da Qt.
    bool result = this->setSocketDescriptor( socketDescriptor );

    if ( result == false )
    {
        emit error( this->error() );
        this->deleteLater();
    }
    else
    {
        emit this->connected();
    }

}

Client::~Client()
{
    qDebug() << this->socketDescriptor  << "Socket destroyed." "\n";
}


int Client::getClientId() const
{
    return this->clientId;
}

QString Client::getUserName() const
{
    return this->userName;
}

int Client::getUserId() const {
    return this->userId;
}

int Client::getIcon() const
{
    return this->icon;
}


bool Client::isAuthenticated() const
{
    return this->authenticated;
}

void Client::setUserId(const int id)
{
    this->userId = id;
}

void Client::setAuthenticated( const bool isAuthenticated )
{
    this->authenticated = isAuthenticated;
}

void Client::setUserName( const QString& userName )
{
    this->userName = userName;
}

void Client::setIcon(const int icon)
{
    this->icon = icon;
}


// Invia al client connesso a questo socket il suo clientId.
// Questo id servirà per identificare i caratteri scritti da ciascun client.
// Questo metodo viene chiamato dopo l'assegnazione del socketDescriptor
// quando viene emesso il segnale connected().
void Client::sendClientId()
{
    qint32 clientId = ( qint32 )this->socketDescriptor;

    char bytesBuffer[ 4 ];
    memcpy( bytesBuffer, &clientId, 4 );

    this->write( bytesBuffer, 4 );
    this->flush();
}



/** SLOTS **/


// Slot per intercettare il segnale readyRead() di QTcpSocket che notifica che ci sono
// nuovi dati disponibili da leggere.
// Prima legge i 4byte che indicano la dimensione in byte del messaggio,
// poi legge il messaggio e lo converte nell'oggetto Message corrispondente
// ed infine emette un segnale per notificare l'arrivo di un certo tipo di messaggio
void Client::onReadyRead()
{
    // Usato come vettore di byte per leggere la dimensione del documento Json dal socket
    static char dataSize[ 4 ];
    static qint32 dataSizeLeft = 4;

    static QByteArray incompleteData{};
    static qint32 bytesToReadLeft = 0;


    if ( this->atEnd() )
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
            qint64 bytesRead = this->read( &dataSize[ 4 - dataSizeLeft ], dataSizeLeft );

            if ( bytesRead != dataSizeLeft )
            {
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



        QByteArray data = this->read( bytesToRead );

        if ( data.size() != bytesToRead )
        {
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

        // std::cout << this->clientId << " in " << bytesToRead << " bytes read by thread number " << std::this_thread::get_id() << std::endl;

        switch ( messageType )
        {
            case Message::MessageType::CHARACTER:
            {
                data.prepend( dataSize, 4 );
                emit this->characterMessageReceived( data );

                break;
            }

            case Message::MessageType::AUTHENTICATION:
            {
                qDebug() << this->clientId << "in AuthenticationMessage:" << messageJsonObject;
                const AuthenticationMessage message = AuthenticationMessage::fromJsonObject( messageJsonObject );
                emit this->authenticationMessageReceived( this, message );

                break;
            }

            case Message::MessageType::DOCUMENT:
            {
                qDebug() << this->clientId << "in DocumentMessage:" << messageJsonObject;
                const DocumentMessage message = DocumentMessage::fromJsonObject( messageJsonObject );
                emit this->documentMessageReceived( this, message );

                break;
            }

            case Message::MessageType::EDIT:
            {
//                const EditMessage message = EditMessage::fromJsonObject( messageJsonObject );
//                emit this->editMessageReceived(message );
                EditMessage message = EditMessage::fromJsonObject( messageJsonObject );
                message.setClientId(this->clientId);
                QByteArray data{};
                message.toBinaryJsonMessage( data );

                emit this->editMessageReceived( data );
                break;
            }
            default:
            {
                throw std::invalid_argument{ "Invalid messageType." };
            }
        }

        if ( !this->atEnd() )
        {
            emit this->readyRead();
        }
    }

    catch( const std::exception& e )
    {
        std::cout << std::endl
                  << this->getClientId()
                  << " Error while reading data: "
                  << e.what()
                  << std::endl;

        const ErrorMessage errorMessage{ clientId, e.what() };
        QByteArray data{};
        errorMessage.toBinaryJsonMessage(data);
        this->write(data);
        this->flush();

        this->disconnectFromHost();
    }


}


// Slot per intercettare il segnale writeData( QByteArray data ) del server.
// Scrive quindi i dati sul socket.
void Client::onWriteData( const QByteArray& data )
{
    this->write( data );
    this->flush();
}


/** Gli slot seguenti servono per intercettare i segnali standard
 *  di QTcpSocket. Per il momento vengono utilizzati semplicemente
 *  a scopo di debug per stampare a video i vari stati in cui si
 *  trova il socket **/

void Client::onConnect()
{
    std::cout << std::endl
              << this->clientId
              << " Client connected with addr: "
              << this->peerAddress().toString().toStdString()
              << " port: "
              << this->peerPort() << std::endl;


    this->sendClientId();
}

void Client::onDisconnect()
{
    std::cout << std::endl << this->clientId << " Client disconnected from server" << std::endl;

    emit this->clientDisconnected( this );
}

void Client::onError( const QAbstractSocket::SocketError socketError )
{
    ( void )socketError;

    std::cout << this->clientId
              << " Socket error: "
              << this->errorString().toStdString() << std::endl;
}

void Client::onStateChange( const QAbstractSocket::SocketState socketState )
{

    switch ( socketState )
    {
        case QAbstractSocket::SocketState::UnconnectedState:
        {
            qDebug() << this->socketDescriptor << "Socket state changed: The socket is not connected.";
            break;
        }

        case QAbstractSocket::SocketState::ConnectingState:
        {
            qDebug() << this->socketDescriptor << "Socket state changed: The socket has started establishing a connection.";
            break;
        }

        case QAbstractSocket::SocketState::BoundState:
        {
           qDebug() << this->socketDescriptor
                    << "Socket state changed: The socket is bound to addr:"
                    << this->localAddress().toString()
                    << " port: "
                    << this->localPort();
            break;
        }

        case QAbstractSocket::SocketState::ClosingState:
        {
            qDebug() << this->socketDescriptor << "Socket state changed: The socket is about to close.";
            break;
        }

        case QAbstractSocket::SocketState::ConnectedState:
        {
            qDebug() << this->socketDescriptor << "Socket state changed: The connection is established.";
            break;
        }

        case QAbstractSocket::SocketState::HostLookupState:
        {
            qDebug() << this->socketDescriptor << "Socket state changed: The socket is performing a host name lookup.";
            break;
        }

        case QAbstractSocket::SocketState::ListeningState:
        {
            qDebug() << this->socketDescriptor << "Socket state changed: The socket is in listening state.";
            break;
        }
    }
}

void Client::onBytesWritten( const quint64 bytes )
{
    std::cout << this->socketDescriptor << " out " << bytes << " bytes written successfully by thread number " << std::this_thread::get_id() << std::endl;
}
