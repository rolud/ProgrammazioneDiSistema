#ifndef CHARACTERMESSAGE_H
#define CHARACTERMESSAGE_H

#include "include/message.h"
#include "include/symbol.h"
#include <QJsonObject>


class CharacterMessage : public Message
{

    public:
        enum OperationType{ INSERT, DELETE };

        CharacterMessage( const int clientId, const Symbol& symbol, const OperationType operationType );

        Symbol getSymbol() const;
        OperationType getOperationType() const;

        static CharacterMessage fromJsonObject( const QJsonObject& jsonObject );

    protected:
        void toJsonObject( QJsonObject& jsonObject ) const override;

    private:
        Symbol symbol;
        OperationType operationType;
};

#endif // CHARACTERMESSAGE_H
