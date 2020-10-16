#include "include/message.h"
#include "include/usermessage.h"
#include <QJsonDocument>


Message::Message( const int clientId, const MessageType messageType ) : clientId( clientId ), messageType( messageType )
{

}

Message::~Message()
{

}


int Message::getClientId() const
{
    return this->clientId;
}

Message::MessageType Message::getMessageType() const
{
    return this->messageType;
}


void Message::toBinaryJsonMessage( QByteArray& data ) const
{
    data.clear();

    QJsonObject messageJsonObject{};
    this->toJsonObject( messageJsonObject );

    QJsonObject jsonObject{};
    jsonObject[ "message_type" ] = this->getMessageType();
    jsonObject[ "message" ] = messageJsonObject;

    QJsonDocument jsonDocument{};
    jsonDocument.setObject( jsonObject );

    data.prepend( jsonDocument.toBinaryData() );

    // Calcola la dimensione del documento json,
    // la converte in array di byte in bytesBuffer
    // e inserisce i byte all'inizio del documento
    char bytesBuffer[ 4 ];
    qint32 size = data.size();
    memcpy( bytesBuffer, &size, 4 );
    data.prepend( bytesBuffer, 4 );

    return;
}









