#include "include/configurationmessage.h"
#include "include/message.h"
#include <QString>
#include <stdexcept>

ConfigurationMessage::ConfigurationMessage(const int clientId, const ConfigStatus configStatus) : Message(clientId, MessageType::CONFIGURATION), configStatus(configStatus) {}

ConfigurationMessage::ConfigStatus ConfigurationMessage::getConfigStatus() const {
    return this->configStatus;
}

void ConfigurationMessage::toJsonObject(QJsonObject &jsonObject) const {
    jsonObject[ "client_id" ] = this->getClientId();
    jsonObject["config_status"] = this->getConfigStatus();
}

ConfigurationMessage ConfigurationMessage::fromJsonObject(const QJsonObject &jsonObject) {
    if ( !jsonObject.contains( "client_id" ) || !jsonObject[ "client_id" ].isDouble() ) {
        // todo handle **SERVER EXCEPTION**
        throw std::invalid_argument{ "Authenticationmessage: QJsonObject does not contain 'client_id'." };
    }

    if ( !jsonObject.contains( "config_status" ) || !jsonObject[ "config_status" ].isDouble() ) {
        // todo handle **SERVER EXCEPTION**
        throw std::invalid_argument{ "Authenticationmessage: QJsonObject does not contain 'config_status'." };
    }

    const int clientId = jsonObject["client_id"].toInt();
    const ConfigurationMessage::ConfigStatus configStatus = ConfigurationMessage::ConfigStatus(jsonObject["config_status"].toInt());

    return ConfigurationMessage(clientId, configStatus);
}

