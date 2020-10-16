#include "include/authenticationmessage.h"
#include "include/usermessage.h"
#include <QString>
#include <stdexcept>

AuthenticationMessage::AuthenticationMessage( const int clientId, const QString& userName, const EventType eventType, const QString& password, int icon, const bool operationSuccess, const QString& serverMessage ): Message( clientId, MessageType::AUTHENTICATION ), userName(userName), password(password), icon(icon), eventType(eventType), operationSuccess( operationSuccess ), serverMessage(serverMessage)
{

}


AuthenticationMessage::EventType AuthenticationMessage::getEventType() const
{
    return this->eventType;
}

QString AuthenticationMessage::getUserName() const
{
    return this->userName;
}


QString AuthenticationMessage::getPassword() const
{
    return this->password;
}

int AuthenticationMessage::getIcon() const
{
    return this->icon;
}


bool AuthenticationMessage::isOperationSuccess() const
{
    return this->operationSuccess;
}

QString AuthenticationMessage::getServerMessage() const
{
    return this->serverMessage;
}


void AuthenticationMessage::toJsonObject( QJsonObject& jsonObject ) const
{
    jsonObject[ "client_id" ] = this->getClientId();
    jsonObject[ "event_type" ] = this->getEventType();
    jsonObject[ "user_name" ] = this->getUserName();
    jsonObject["password"] = this->getPassword();
    jsonObject["icon"] = this->getIcon();
    jsonObject["operation_success"] = this->isOperationSuccess();
    jsonObject["server_message"] = this->getServerMessage();

    return;
}


AuthenticationMessage AuthenticationMessage::fromJsonObject( const QJsonObject& jsonObject )
{
    if ( !jsonObject.contains( "client_id" ) || !jsonObject[ "client_id" ].isDouble() )
    {
        throw std::invalid_argument{ "Authenticationmessage: QJsonObject does not contain 'client_id'." };
    }

    if ( !jsonObject.contains( "user_name" ) || !jsonObject[ "user_name" ].isString() )
    {
        throw std::invalid_argument{ "Authenticationmessage: QJsonObject does not contain 'user_name'." };
    }

    if ( !jsonObject.contains( "event_type" ) || !jsonObject[ "event_type" ].isDouble() )
    {
        throw std::invalid_argument{ "Authenticationmessage: QJsonObject does not contain 'event_type'." };
    }

    if ( !jsonObject.contains( "password" ) || !jsonObject[ "password" ].isString() )
    {
        throw std::invalid_argument{ "Authenticationmessage: QJsonObject does not contain 'password'." };
    }
    if ( !jsonObject.contains( "icon" ) || !jsonObject[ "icon" ].isDouble() )
    {
        throw std::invalid_argument{ "Authenticationmessage: QJsonObject does not contain 'icon'." };
    }

    if ( !jsonObject.contains( "operation_success" ) || !jsonObject[ "operation_success" ].isBool() )
    {
        throw std::invalid_argument{ "Authenticationmessage: QJsonObject does not contain 'operation_success'." };
    }
    if ( !jsonObject.contains( "server_message" ) || !jsonObject[ "server_message" ].isString() )
    {
        throw std::invalid_argument{ "Authenticationmessage: QJsonObject does not contain 'server_message'." };
    }

    const int clientId = jsonObject[ "client_id" ].toInt();
    const QString userName = jsonObject[ "user_name" ].toString();
    const AuthenticationMessage::EventType eventType = AuthenticationMessage::EventType( jsonObject[ "event_type" ].toInt() );
    const QString password = jsonObject["password"].toString();
    int icon = jsonObject["icon"].toInt();
    const bool operationSuccess = jsonObject["operation_success"].toBool();
    const QString serverMessage = jsonObject["server_message"].toString();

    return AuthenticationMessage{ clientId, userName, eventType, password, icon, operationSuccess, serverMessage };
}


