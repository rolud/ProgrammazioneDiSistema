#include "include/document.h"
#include <QString>
#include "include/client.h"
#include <QList>
#include <QThread>
#include <QFile>
#include <QDir>
#include <QByteArray>
#include <QApplication>
#include "include/usermessage.h"
#include "include/configurationmessage.h"
#include <iostream>
#include <thread>

Document::Document( const QString& name, QThread* const thread, QObject *parent ) : QObject( parent ), connectedClientsList(), name( name ), file(), thread( thread ), numClientsConnected( 0 )
{
    QDir dir{ "./" };
    QString dirName = name.split( "/" )[ 0 ];
    QString fileName = name.split( "/" )[ 1 ];
    QString path = "documents/" + dirName;
    dir.mkpath( path );
    this->file.setFileName( path + "/" + fileName );
    this->openFile( QFile::Append );
    QObject::connect( this->thread, &QThread::finished, this, &Document::deleteLater );
    QObject::connect( this, &Document::clientConnected, this, &Document::onClientConnected );
    std::cout << "Document '" << this->name.toStdString() << "' opened." << std::endl;
}

Document::~Document()
{
    this->closeFile();

    for ( Client* const client: this->connectedClientsList )
    {
        client->disconnectFromHost();
        client->deleteLater();
    }

    std::cout << "Document '" << this->name.toStdString() << "' closed." << std::endl;
}


void Document::openFile( const QFile::OpenMode openMode )
{
    if ( this->file.isOpen() )
    {
        this->closeFile();
    }

    if ( !this->file.open( openMode ) )
    {
        throw std::invalid_argument( "Server could not open document " + this->name.toStdString() );
    }
}

void Document::closeFile()
{
    if ( this->file.isOpen() )
    {
        if ( this->file.openMode() == QFile::Append )
        {
            this->file.flush();
        }

        this->file.close();
    }
}

void Document::connectClient( Client* const client )
{
    client->moveToThread( this->thread );
    emit this->clientConnected( client );
}

void Document::sendDocHistory( Client* const client )
{
    this->openFile( QFile::ReadOnly );

    QByteArray data = file.readAll();
    client->write( data );
    client->flush();

    this->closeFile();

    // notifica il client che ha finito di inviare il contenuto del file
    ConfigurationMessage configMsg = ConfigurationMessage(client->getClientId(), ConfigurationMessage::ConfigStatus::SUCCESS);
    QByteArray configMsgJsonData{};
    configMsg.toBinaryJsonMessage(configMsgJsonData);

    client->write(configMsgJsonData);
    client->flush();
}



void Document::onCharacterMessageReceived( const QByteArray& message )
{
    if ( !this->file.isOpen() || this->file.openMode() != QFile::Append )
    {
        this->openFile( QFile::Append );
    }

    this->file.write( message );
    emit this->writeData( message );
}

void Document::onClientDisconnected( Client* const client )
{
    QObject::disconnect( client, &Client::characterMessageReceived, this, &Document::onCharacterMessageReceived );
    QObject::disconnect( this, &Document::writeData, client, &Client::onWriteData );
    QObject::disconnect( client, &Client::clientDisconnected, this, &Document::onClientDisconnected );
    QObject::disconnect( client, &Client::disconnectClientFromDocument, this, &Document::onClientDisconnected );
    QObject::disconnect( client, &Client::editMessageReceived, this, &Document::onEditMessageReceived );

    // Rimuove dalla lista dei client connessi del server il client che si è disconnesso
    this->connectedClientsList.removeAll( client );

    // Notifica tutti i client connessi che il client si è disconnesso
    UserMessage userMessage{ client->getClientId(), client->getUserName(), UserMessage::EventType::DISCONNECTED, client->getIcon() };
    QByteArray data{};
    userMessage.toBinaryJsonMessage( data );
    emit this->writeData( data );

    this->numClientsConnected--;
    std::cout << client->getClientId() << " Client disconnected from document '" << this->name.toStdString() << "'" << std::endl;

    if ( numClientsConnected == 0 )
    {
        emit this->closeDocument( this );
    }

    return;
}

void Document::onClientConnected( Client* client )
{
    std::cout << client->getClientId() << " Client connected to document '" << this->name.toStdString() << "'" <<std::endl;
    this->numClientsConnected++;

    this->sendDocHistory( client );

    QByteArray data{};

    // Invia al client appena connesso la lista dei client già connessi
    for ( const Client* const s : this->connectedClientsList )
    {
        UserMessage userMessage{ s->getClientId(), s->getUserName(), UserMessage::EventType::CONNECTED, s->getIcon() };
        userMessage.toBinaryJsonMessage( data );

        client->write( data );
        client->flush();

        data.clear();
    }

    // Aggiunge alla lista dei client connessi del server il client appena connesso
    this->connectedClientsList.push_back( client );

    // Notifica tutti i client connessi che il nuovo client si è connesso
    UserMessage userMessage{ client->getClientId(), client->getUserName(), UserMessage::EventType::CONNECTED, client->getIcon() };
    userMessage.toBinaryJsonMessage( data );
    emit this->writeData( data );

    // Connette segnali e slot del socket con il documento in modo che
    // si possano comunicare quando ci sono dati disponibile da scrivere o da leggere
    QObject::connect( client, &Client::characterMessageReceived, this, &Document::onCharacterMessageReceived );
    QObject::connect( this, &Document::writeData, client, &Client::onWriteData );

    // Per notificare tutti i client quando uno dei client si disconnette
    QObject::connect( client, &Client::clientDisconnected, this, &Document::onClientDisconnected );
    QObject::connect( client, &Client::disconnectClientFromDocument, this, &Document::onClientDisconnected );

    QObject::connect( client, &Client::editMessageReceived, this, &Document::onEditMessageReceived );

    return;
}


int Document::getNumClientsConnected() const
{
    return this->numClientsConnected;
}

QString Document::getName() const
{
    return this->name;
}




void Document::onEditMessageReceived( const QByteArray& message )
{
    emit this->writeData( message );
}
