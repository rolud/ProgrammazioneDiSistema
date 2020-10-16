#ifndef DOCUMENTMESSAGE_H
#define DOCUMENTMESSAGE_H

#include "include/message.h"
#include <QString>
#include <QList>


class DocumentMessage : public Message
{
    public:
        enum EventType{ SELECTED_DOCUMENT, NEW_DOCUMENT, URL_DOCUMENT, SERVER_RESPONSE };


        DocumentMessage( const int clientId, const EventType eventType, const QList< QString >& documentsList, const QString& selectedDocumentName, const QString& newDocumentName, const QString& urlDocument, const bool operationSuccess, const QString& serverMessage );


        EventType getEventType() const;
        QList< QString > getDocumentsList() const;
        QString getSelectedDocumentName() const;
        QString getNewDocumentName() const;
        QString getUrlDocument() const;
        bool isOperationSuccess() const;
        QString getServerMessage() const;

        static DocumentMessage fromJsonObject( const QJsonObject& jsonObject );


    protected:
        void toJsonObject( QJsonObject& jsonObject ) const override;


    private:
        QList< QString > documentsList;
        QString selectedDocumentName;
        QString newDocumentName;
        QString urlDocument;
        EventType eventType;

        // Attributi per il messaggio SERVER_RESPONSE
        // servono per comunicare al client se l'operazione
        // ha avuto successo o se è fallita; se è fallita l'errore è specificato
        // in serverMessage
        bool operationSuccess;
        QString serverMessage;
};

#endif // DOCUMENTMESSAGE_H
