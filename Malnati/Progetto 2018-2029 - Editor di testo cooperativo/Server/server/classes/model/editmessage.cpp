#include "include/editmessage.h"
#include <stdexcept>


EditMessage::EditMessage( const int clientId, const int pos ) : Message( clientId, MessageType::EDIT ), position(pos){}



EditMessage EditMessage::fromJsonObject( const QJsonObject& jsonObject )
{
    // Controlli che l'oggetto jsonObject contenga dati validi
    if ( !jsonObject.contains( "client_id" ) || !jsonObject[ "client_id" ].isDouble() )
    {
        throw std::invalid_argument{ "QJsonObject does not contain 'client_id'." };
    }

    if ( !jsonObject.contains( "pos" ) || !jsonObject[ "pos" ].isDouble() )
    {
        throw std::invalid_argument{ "QJsonObject does not contain 'pos'." };
    }

    const int clientId = jsonObject[ "client_id" ].toInt();
    const int pos = jsonObject[ "pos" ].toInt();

    return EditMessage{ clientId, pos};
}

int EditMessage::getPosition() const
{
    return this->position;
}

int EditMessage::getClientId() const{
    return this->clientId;
}


void EditMessage::setClientId(int id) {
    this->clientId = id;
}


void EditMessage::toJsonObject( QJsonObject& jsonObject ) const
{
    jsonObject[ "client_id" ] = this->getClientId();
    jsonObject[ "pos" ] = this->getPosition();

    return;
}
