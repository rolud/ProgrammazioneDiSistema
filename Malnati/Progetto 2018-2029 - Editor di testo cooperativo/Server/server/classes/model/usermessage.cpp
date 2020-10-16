#include "include/usermessage.h"
#include <QString>
#include <stdexcept>


UserMessage::UserMessage( const int clientId, const QString& userName, const UserMessage::EventType eventType, const int icon ) : Message( clientId, MessageType::USER ), userName( userName ), eventType( eventType ), icon( icon )
{

}


UserMessage::EventType UserMessage::getEventType() const
{
    return this->eventType;
}

QString UserMessage::getUserName() const
{
    return this->userName;
}

int UserMessage::getIcon() const
{
    return this->icon;
}


UserMessage UserMessage::fromJsonObject( const QJsonObject& jsonObject )
{
    // Controlli che l'oggetto jsonObject contenga dati validi
    if ( !jsonObject.contains( "client_id" ) || !jsonObject[ "client_id" ].isDouble() )
    {
        throw std::invalid_argument{ "UserMessage: QJsonObject does not contain 'client_id'." };
    }

    if ( !jsonObject.contains( "user_name" ) || !jsonObject[ "user_name" ].isString() )
    {
        throw std::invalid_argument{ "UserMessage: QJsonObject does not contain 'user_name'." };
    }

    if ( !jsonObject.contains( "event_type" ) || !jsonObject[ "event_type" ].isDouble() )
    {
        throw std::invalid_argument{ "UserMessage: QJsonObject does not contain 'event_type'." };
    }

    if ( !jsonObject.contains( "icon" ) || !jsonObject[ "icon" ].isDouble() )
    {
        throw std::invalid_argument{ "UserMessage: QJsonObject does not contain 'icon'." };
    }



    const int clientId = jsonObject[ "client_id" ].toInt();
    const QString userName = jsonObject[ "user_name" ].toString();
    const UserMessage::EventType eventType = UserMessage::EventType( jsonObject[ "event_type" ].toInt() );
    const int icon = jsonObject[ "icon" ].toInt();

    return UserMessage{ clientId, userName, eventType, icon };
}

void UserMessage::toJsonObject( QJsonObject& jsonObject ) const
{
    jsonObject[ "client_id" ] = this->getClientId();
    jsonObject[ "user_name" ] = this->getUserName();
    jsonObject[ "event_type" ] = this->getEventType();
    jsonObject[ "icon" ] = this->getIcon();
    jsonObject[ "message_type" ] = this->getMessageType();

    return;
}
