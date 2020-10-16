#include "include/errormessage.h"
#include "include/message.h"

ErrorMessage::ErrorMessage( const int clientId, const QString& errorMessage ): Message( clientId, MessageType::ERROR ), errorMessage( errorMessage )
{

}

QString ErrorMessage::getErrorMessage() const
{
    return this->errorMessage;
}

void ErrorMessage::toJsonObject( QJsonObject& jsonObject ) const
{
    jsonObject[ "client_id" ] = this->getClientId();
    jsonObject[ "error_message" ] = this->getErrorMessage();

    return;
}

ErrorMessage ErrorMessage::fromJsonObject( const QJsonObject& jsonObject )
{
    if ( !jsonObject.contains( "client_id" ) || !jsonObject[ "client_id" ].isDouble() )
    {
        throw std::invalid_argument{ "ErrorMessage: QJsonObject does not contain 'client_id'." };
    }

    if ( !jsonObject.contains( "error_message" ) || !jsonObject[ "error_message" ].isString() )
    {
        throw std::invalid_argument{ "ErrorMessage: QJsonObject does not contain 'error_message'." };
    }


    const int clientId = jsonObject[ "client_id" ].toInt();
    const QString errorMessage = jsonObject["error_message"].toString();


    return ErrorMessage( clientId, errorMessage );
}
