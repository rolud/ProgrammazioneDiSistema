#ifndef MESSAGE_H
#define MESSAGE_H

#include <QByteArray>
#include <QJsonObject>


// Classe base da cui ereditano tutti i vari tipi di messaggi.
// Contiene solo la definizione dell'enumerazione che serve per distinguere
// tra i vari tipi di messaggi quando arrivano dalla rete in formato json.
class Message
{

    public:
        enum MessageType{ CHARACTER, USER, AUTHENTICATION, CONFIGURATION, DOCUMENT, ERROR, EDIT};

        Message( const int clientId, const MessageType messageType );
        virtual ~Message();

        int getClientId() const;
        MessageType getMessageType() const;

        void toBinaryJsonMessage( QByteArray& data ) const;


    protected:
        virtual void toJsonObject( QJsonObject& jsonObject ) const = 0;


    private:
        int clientId;
        MessageType messageType;

};

#endif // MESSAGE_H
