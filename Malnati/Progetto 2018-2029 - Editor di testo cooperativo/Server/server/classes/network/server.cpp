#include "include/server.h"
#include "include/client.h"
#include "include/message.h"
#include "include/usermessage.h"
#include "include/authenticationmessage.h"
#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QByteArray>
#include <QHostAddress>
#include <QDebug>
#include <QList>
#include <QFile>
#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include "include/document.h"
#include "include/dbinterface.h"
#include <iostream>
#include <QThread>
#include <QTextBoundaryFinder>


Server::Server( QObject* parent ) : QTcpServer( parent ), openDocumentsMap(), db(), threads( this->numThreads )
{
    this->db.openDBConnection();

    std::cout << "Number of secondary threads: " << this->numThreads << std::endl;

    for ( int i = 0; i < this->numThreads; i++ )
    {
        QThread* thread = new QThread{ this };
        QObject::connect( thread, &QThread::finished, thread, &QThread::deleteLater );
        thread->start();
        this->threads[ i ] = thread;
    }

}

Server::~Server()
{
    for ( QThread* threadPtr: this->threads )
    {
        threadPtr->quit();
        threadPtr->deleteLater();
    }
}


// Mette in ascolto il server sui suoi attibuti privati port e address
void Server::start()
{
    if ( !this->listen( this->address, this->port ) )
    {
        std::cout << "Server could not start listening: " << this->errorString().toStdString() << std::endl;
    }

    else
    {
        std::cout << "Server listening at addr: "
                  << this->serverAddress().toString().toStdString()
                  << " port: "
                  << this->serverPort() << std::endl;
    }
}


// Metodo chiamato tutte le volte che un client si connette al server,
// il socketDescriptor identifica la socket aperta alla connessione del client
void Server::incomingConnection( const qintptr socketDescriptor )
{
    // Crea un nuovo socket a cui viene assegnato il socketDescriptor
    Client* socket = new Client{ socketDescriptor };

    QObject::connect( socket, &Client::authenticationMessageReceived, this, &Server::onAuthenticationMessageReceived, Qt::DirectConnection );
    QObject::connect( socket, &Client::documentMessageReceived, this, &Server::onDocumentMessageReceived, Qt::DirectConnection );

    // Quando il socket si disconnette dal server Qt provvede a distruggere
    // automaticamete e in modo sicuro il Socket allocato
    QObject::connect( socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater );
}



void Server::onAuthenticationMessageReceived( Client* const client, const AuthenticationMessage& message )
{
    switch ( message.getEventType() )
    {
        case AuthenticationMessage::EventType::LOGIN:
        {
            authenticateUser( client, message.getUserName(), message.getPassword() );
            break;
        }

        case AuthenticationMessage::EventType::REGISTER:
        {
            registerNewUser(client, message.getUserName(), message.getPassword(), message.getIcon() );
            break;
        }

        case AuthenticationMessage::EventType::UPDATE:
        {
            updateUserData( client, message.getUserName(), message.getPassword(), message.getIcon() );
            break;
        }

        case AuthenticationMessage::EventType::LOGOUT:
        {
            logoutUser( client );
            break;
        }

        default:
        {
            throw std::invalid_argument( "Unknown authentication message received." );
        }
    }
}


void Server::authenticateUser( Client* const client, const QString &userName, const QString &password )
{
    QByteArray data{};

    try
    {
        db.checkCredentials(userName,password);
        int uid = db.getUID(userName);
        int i = db.getDBIcon(uid);

        //std::cout <<"uid= "<< uid << std::endl;
        //std::cout <<"icon= "<< i << std::endl;

        client->setUserId(uid);
        client->setIcon(i);
        client->setAuthenticated( true );
        client->setUserName( userName );

        AuthenticationMessage message{ client->getClientId(), userName, AuthenticationMessage::LOGIN, password, i, true, "Logged in successfully." } ;
        message.toBinaryJsonMessage( data );

        std::cout << std::endl << client->getClientId() << " User authenticated successfully with username '" << userName.toStdString() << "'" << std::endl;
    }

    catch ( const BadParamException& bpe )
    {
        QString err = "Invalid username or password";
        AuthenticationMessage message{ client->getClientId(), userName, AuthenticationMessage::LOGIN, password, 0, false, err };
        message.toBinaryJsonMessage( data );
        client->setAuthenticated( false );

        std::cout << std::endl << client->getClientId() << " Authentication error with username '" << userName.toStdString() << "': " << bpe.what() << std::endl;

    }

    catch ( const QueryException& qe )
    {
        QString err = "Error selecting existing users in database";
        AuthenticationMessage message{ client->getClientId(), userName, AuthenticationMessage::LOGIN, password, 0, false, err };
        message.toBinaryJsonMessage( data );
        client->setAuthenticated( false );

        std::cout << std::endl << client->getClientId() << " Authentication error with username '" << userName.toStdString() << "': " << qe.what() << std::endl;
    }

    client->write( data );
    client->flush();

    if ( client->isAuthenticated() )
    {
        this->sendDocumentsListToClient( client );
    }

}

void Server::registerNewUser( Client* const client, const QString &userName, const QString &password, const int icon )
{

    QByteArray data{};

    try
    {
        uint uid = db.createUser(userName,password, static_cast<uint>(icon));
        client->setUserId(static_cast<int>(uid));
        client->setAuthenticated( true );
        client->setUserName( userName );
        client->setIcon( icon );

        AuthenticationMessage message{ client->getClientId(), userName, AuthenticationMessage::REGISTER, password, icon, true, "User registered correctly" };
        message.toBinaryJsonMessage( data );

        std::cout << std::endl << client->getClientId() << " User registered successfully with username '" << userName.toStdString() << "'" << std::endl;
    }

    catch ( const BadParamException& bpe )
    {
        QString err = bpe.what();
        AuthenticationMessage message{ client->getClientId(), userName, AuthenticationMessage::REGISTER, password, 0, false, "A user with the selected username already exists." };
        message.toBinaryJsonMessage( data );
        client->setAuthenticated( false );

        std::cout << std::endl << client->getClientId() << " Registration error with username '" << userName.toStdString() << "': " << bpe.what() << std::endl;
    }

    catch( const QueryException& qe )
    {
        QString err = qe.what();
        AuthenticationMessage message{ client->getClientId(), userName, AuthenticationMessage::REGISTER, password, 0, false, "Error registering new user. Please, try again later." };
        message.toBinaryJsonMessage( data );
        client->setAuthenticated( false );

        std::cout << std::endl << client->getClientId() << " Registration error with username '" << userName.toStdString() << "': " << qe.what() << std::endl;
    }

    client->write( data );
    client->flush();

    if ( client->isAuthenticated() )
    {
        this->sendDocumentsListToClient( client );
    }
}

void Server::updateUserData( Client* const client, const QString &userName, const QString &password, const int icon )
{
    QByteArray data{};

    try {
        db.updateAllProfile(client->getUserId(),userName, password, static_cast<uint>(icon));
        client->setIcon(icon);
        client->setUserName( userName );

        AuthenticationMessage message{client->getClientId(), userName, AuthenticationMessage::UPDATE, password, icon, true, "User profile updated correctly"};
        message.toBinaryJsonMessage( data );

        std::cout << std::endl << client->getClientId() << " User updated successfully with username '" << userName.toStdString() << "'" << " and iconNumber= "<< icon << std::endl;

    } catch ( const BadParamException& bpe ) {
        QString err = bpe.what();
        AuthenticationMessage message{ client->getClientId(), userName, AuthenticationMessage::UPDATE, password, 0, false, err };
        message.toBinaryJsonMessage( data );

        std::cout << std::endl << client->getClientId() << " update error for client with username '" << userName.toStdString() << "': " << "Error while updating user data. Please, try again later." << std::endl;
    }
    catch( const QueryException& qe ) {
        QString err = qe.what();
        AuthenticationMessage message{ client->getClientId(), userName, AuthenticationMessage::UPDATE, password, 0, false, err };
        message.toBinaryJsonMessage( data );

        std::cout << std::endl << client->getClientId() << " update error for client with username '" << userName.toStdString() << "': " << "The username inserted is already used. Please, insert another one." << std::endl;
    }

    client->write( data );
    client->flush();
}

void Server::logoutUser( Client* const client )
{
    client->disconnectClientFromDocument( client );
    client->moveToThread( QApplication::instance()->thread() );
    client->setUserId( -1 );
    client->setUserName( "" );
    client->setAuthenticated( false );
    client->setIcon( -1 );
}


void Server::selectDocument( Client* const client, const QString& documentName )
{    
    this->openDocument( documentName );
    Document* selectedDocument = this->openDocumentsMap.value( documentName );

    DocumentMessage message{ client->getClientId(), DocumentMessage::SERVER_RESPONSE, QList< QString >{}, documentName, "", "", true, "" };
    QByteArray data{};
    message.toBinaryJsonMessage( data );
    client->write( data );
    client->flush();

    selectedDocument->connectClient( client );
}


void Server::onDocumentMessageReceived( Client* const client, const DocumentMessage& message )
{
    try
    {
        switch ( message.getEventType() )
        {
            case DocumentMessage::SELECTED_DOCUMENT:
            {
                const QString documentName = message.getSelectedDocumentName();
                std::cout << client->getClientId() << " Client '" << client->getUserName().toStdString() << "' selected document '" << documentName.toStdString() << "'" << std::endl;
                this->selectDocument( client, documentName );

                break;
            }

            case DocumentMessage::NEW_DOCUMENT:
            {
                const QString newDocumentName = message.getNewDocumentName();
                db.insertDocument(newDocumentName);
                db.addDocumentAccess( client->getUserName(), newDocumentName );
                std::cout << client->getClientId() << " Client '" << client->getUserName().toStdString() << "' created new document '" << newDocumentName.toStdString() << "'" << std::endl;
                this->selectDocument( client, newDocumentName );

                break;
            }

            case DocumentMessage::URL_DOCUMENT:
            {
                const QString urlDocument = message.getUrlDocument();

                if ( !urlDocument.startsWith( "url://" ) || urlDocument.length() <= 8 )
                {
                    throw std::invalid_argument{ "The url inserted is not valid." };
                }
                const QString documentName = urlDocument.mid( 6 );
                /*qDebug() << documentName;
                bool res = db.getDocument(documentName);
                qDebug() <<"ritornato: "<< res;
                */
                if(db.getDocument(documentName)) {
                    db.addDocumentAccess( client->getUserName(), documentName );
                    std::cout << client->getClientId() << " Client '" << client->getUserName().toStdString() << "' requested document by url '" << urlDocument.toStdString() << "'" << std::endl;
                    this->selectDocument( client, documentName );
                }
                break;
            }

            default:
            {
                qDebug() << "Unknown document message received.";
            }
        }
    }

    catch ( const std::exception& e )
    {
        DocumentMessage documentMessage{ client->getClientId(), DocumentMessage::SERVER_RESPONSE, QList< QString >(), "", "", "", false, e.what() };
        QByteArray data{};
        documentMessage.toBinaryJsonMessage( data );
        client->write(data);
        client->flush();
    }

    catch ( ... )
    {
        DocumentMessage documentMessage{ client->getClientId(), DocumentMessage::SERVER_RESPONSE, QList< QString >(), "", "", "", false, "Error while accessing document. Please, try again later." };
        QByteArray data{};
        documentMessage.toBinaryJsonMessage( data );
        client->write(data);
        client->flush();
    }


}


void Server::openDocument( const QString& documentName )
{
    if ( !this->openDocumentsMap.contains( documentName ) )
    {
        int threadIndex = this->nextThread++;

        if ( this->nextThread == this->numThreads )
        {
            this->nextThread = 0;
        }

        Document* documentPtr = new Document( documentName, threads[ threadIndex ] );
        QObject::connect( documentPtr, &Document::closeDocument, this, &Server::onCloseDocument, Qt::QueuedConnection );
        documentPtr->moveToThread( threads[ threadIndex ] );
        this->openDocumentsMap.insert( documentName, documentPtr );
    }
}

void Server::onCloseDocument( Document* const document )
{
    if ( document->getNumClientsConnected() == 0 )
    {
        this->openDocumentsMap.remove( document->getName() );
        document->deleteLater();
    }
}


void Server::sendDocumentsListToClient( Client* const client )
{
    QList< QString > documentsList{};

    try
    {
        documentsList = db.getAccessibileDocuments( client->getUserName() );
    }
    catch (...)
    {
        qDebug() << "Error reading documentsList of client" << client->getUserName() << "\n";
        documentsList.clear();
    }

    QByteArray data{};
    DocumentMessage message{ client->getClientId(), DocumentMessage::SELECTED_DOCUMENT, documentsList, "", "", "", false, "" };
    message.toBinaryJsonMessage( data );

    client->write( data );
    client->flush();
}
