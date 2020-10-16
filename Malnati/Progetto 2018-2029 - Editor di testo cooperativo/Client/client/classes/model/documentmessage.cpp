#include "include/documentmessage.h"
#include "include/message.h"
#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>


DocumentMessage::DocumentMessage( const int clientId, const EventType eventType, const QList< QString >& documentsList, const QString& selectedDocumentName, const QString& newDocumentName, const QString& urlDocument, const bool operationSuccess, const QString& serverMessage ) : Message( clientId, MessageType::DOCUMENT ), documentsList( documentsList ), selectedDocumentName( selectedDocumentName ), newDocumentName( newDocumentName ), urlDocument( urlDocument ), eventType( eventType ), operationSuccess( operationSuccess ), serverMessage( serverMessage )
{

}


QList< QString > DocumentMessage::getDocumentsList() const
{
    return this->documentsList;
}

QString DocumentMessage::getSelectedDocumentName() const
{
    return this->selectedDocumentName;
}

QString DocumentMessage::getNewDocumentName() const
{
    return this->newDocumentName;
}

QString DocumentMessage::getUrlDocument() const
{
    return this->urlDocument;
}

bool DocumentMessage::isOperationSuccess() const
{
    return this->operationSuccess;
}

QString DocumentMessage::getServerMessage() const
{
    return this->serverMessage;
}

DocumentMessage::EventType DocumentMessage::getEventType() const
{
    return this->eventType;
}


void DocumentMessage::toJsonObject( QJsonObject& jsonObject ) const
{
    jsonObject[ "client_id" ] = this->getClientId();
    jsonObject[ "event_type" ] = this->getEventType();
    jsonObject[ "selected_document_name" ] = this->getSelectedDocumentName();
    jsonObject[ "new_document_name" ] = this->getNewDocumentName();
    jsonObject[ "url_document" ] = this->getUrlDocument();
    jsonObject[ "operation_success" ] = this->isOperationSuccess();
    jsonObject[ "server_message" ] = this->getServerMessage();

    QJsonArray jsonArray{};

    for ( const QString& documentName : this->getDocumentsList() )
    {
        jsonArray.append( documentName );
    }

    jsonObject[ "documents_list" ] = jsonArray;


    return;
}


DocumentMessage DocumentMessage::fromJsonObject( const QJsonObject& jsonObject )
{
    if ( !jsonObject.contains( "client_id" ) || !jsonObject[ "client_id" ].isDouble() )
    {
        throw std::invalid_argument{ "DocumentMessage: QJsonObject does not contain 'client_id'." };
    }

    if ( !jsonObject.contains( "event_type" ) || !jsonObject[ "event_type" ].isDouble() )
    {
        throw std::invalid_argument{ "DocumentMessage: QJsonObject does not contain 'event_type'." };
    }

    if ( !jsonObject.contains( "documents_list" ) || !jsonObject[ "documents_list" ].isArray() )
    {
        throw std::invalid_argument{ "DocumentMessage: QJsonObject does not contain 'documents_list'." };
    }

    if ( !jsonObject.contains( "selected_document_name" ) || !jsonObject[ "selected_document_name" ].isString() )
    {
        throw std::invalid_argument{ "DocumentMessage: QJsonObject does not contain 'selected_document_name'." };
    }

    if ( !jsonObject.contains( "new_document_name" ) || !jsonObject[ "new_document_name" ].isString() )
    {
        throw std::invalid_argument{ "DocumentMessage: QJsonObject does not contain 'new_document_name'." };
    }

    if ( !jsonObject.contains( "url_document" ) || !jsonObject[ "url_document" ].isString() )
    {
        throw std::invalid_argument{ "DocumentMessage: QJsonObject does not contain 'url_document'." };
    }

    if ( !jsonObject.contains( "operation_success" ) || !jsonObject[ "operation_success" ].isBool() )
    {
        throw std::invalid_argument{ "DocumentMessage: QJsonObject does not contain 'operation_success'." };
    }

    if ( !jsonObject.contains( "server_message" ) || !jsonObject[ "server_message" ].isString() )
    {
        throw std::invalid_argument{ "DocumentMessage: QJsonObject does not contain 'server_message'." };
    }




    const int clientId = jsonObject[ "client_id" ].toInt();
    const DocumentMessage::EventType eventType = DocumentMessage::EventType( jsonObject[ "event_type" ].toInt() );

    QList< QString > documentsList{};
    QJsonArray documentsArray = jsonObject[ "documents_list" ].toArray();

    for ( const QJsonValueRef& d : documentsArray )
    {
        documentsList.append( d.toString() );
    }

    const QString selectedDocumentName = jsonObject[ "selected_document_name" ].toString();
    const QString newDocumentName = jsonObject[ "new_document_name" ].toString();
    const QString urlDocument = jsonObject[ "url_document" ].toString();
    const bool operationSuccess = jsonObject["operation_success"].toBool();
    const QString serverMessage = jsonObject["server_message"].toString();


    return DocumentMessage( clientId, eventType, documentsList, selectedDocumentName, newDocumentName, urlDocument, operationSuccess, serverMessage );
}
