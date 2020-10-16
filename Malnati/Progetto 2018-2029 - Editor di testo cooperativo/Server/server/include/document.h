#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include "include/client.h"
#include <QString>
#include <QList>
#include <QFile>
#include <QThread>

class Document : public QObject
{
    Q_OBJECT

    public:
        Document( const QString& name, QThread* const thread, QObject* parent = nullptr );
        ~Document();

        Document( const Document& document ) = delete;
        Document& operator=( const Document& document ) = delete;

        int getNumClientsConnected() const;
        QString getName() const;

        void connectClient( Client* const client );

    protected:
        void openFile( const QFile::OpenMode openMode );
        void closeFile();

        /**
         * @brief sendFileHistory
         * Invia al client appena connesso il contenuto del documento
         */
        void sendDocHistory( Client* const client );

    private:
        QList< Client* > connectedClientsList; // Lista dei client che attualmente stanno modificando questo documento
        QString name; // Nome del documento
        QFile file;
        QThread* const thread; // Il thread a cui è associato questo documento
        int numClientsConnected;


    signals:
        // Segnale inviato per notificare tutti i client connessi
        // che ci sono nuovi dati da inviare
        void writeData( const QByteArray& data );
        void closeDocument( Document* const document );
        void clientConnected( Client* client );

    public slots:
        void onCharacterMessageReceived( const QByteArray& message );
        void onClientDisconnected( Client* const client );
        void onClientConnected( Client* client );
        void onEditMessageReceived( const QByteArray& message );


};

#endif // DOCUMENT_H
