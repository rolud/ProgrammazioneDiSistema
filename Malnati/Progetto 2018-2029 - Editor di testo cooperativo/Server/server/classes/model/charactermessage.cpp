#include "include/charactermessage.h"
#include <stdexcept>
#include <QJsonObject>


CharacterMessage::CharacterMessage( const int clientId, const Symbol& symbol, const CharacterMessage::OperationType operationType ) : Message( clientId, MessageType::CHARACTER ), symbol( symbol ), operationType( operationType )
{

}


CharacterMessage CharacterMessage::fromJsonObject( const QJsonObject& jsonObject )
{
    // Controlli che l'oggetto jsonObject contenga dati validi
    if ( !jsonObject.contains( "client_id" ) || !jsonObject[ "client_id" ].isDouble() )
    {
        throw std::invalid_argument{ "QJsonObject does not contain 'client_id'." };
    }

    if ( !jsonObject.contains( "operation_type" ) || !jsonObject[ "operation_type" ].isDouble() )
    {
        throw std::invalid_argument{ "QJsonObject does not contain 'operation_type'." };
    }

    if ( !jsonObject.contains( "symbol" ) || !jsonObject[ "symbol" ].isObject() )
    {
        throw std::invalid_argument{ "QJsonObject does not contain 'symbol'." };
    }


    const int clientId = jsonObject[ "client_id" ].toInt();
    const Symbol symbol = Symbol::fromJsonObject( jsonObject[ "symbol" ].toObject() );
    const CharacterMessage::OperationType operationType = CharacterMessage::OperationType( jsonObject[ "operation_type" ].toInt() );

    return CharacterMessage{ clientId, symbol, operationType };
}


Symbol CharacterMessage::getSymbol() const
{
    return this->symbol;
}

CharacterMessage::OperationType CharacterMessage::getOperationType() const
{
    return this->operationType;
}


void CharacterMessage::toJsonObject( QJsonObject& jsonObject ) const
{
    jsonObject[ "client_id" ] = this->getClientId();

    QJsonObject symbolJsonObject{};
    this->getSymbol().toJsonObject( symbolJsonObject );
    jsonObject[ "symbol" ] = symbolJsonObject;

    jsonObject[ "operation_type" ] = this->getOperationType();

    return;
}
