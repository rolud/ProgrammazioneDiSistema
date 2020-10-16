#ifndef CONFIGURATIONMESSAGE_H
#define CONFIGURATIONMESSAGE_H

#include "include/message.h"
#include <QJsonObject>
#include <QString>
/**
 * messaggio per configurazione iniziale
 * al momento pensato per segnalare al client la fine della configurazione iniziale
 * dopo aver inviaro il contenuto del documento dopo la connesione
 * può essere usato anche per scelta documento o altri tipi di impostazioni
 *
 * @author rolud
 * @date   14/05/2020
 */
class ConfigurationMessage : public Message {
public:
    enum ConfigStatus {SUCCESS, FAILURE};

    ConfigurationMessage(const int clientId, const ConfigStatus configStatus);

    ConfigStatus getConfigStatus() const;

    static ConfigurationMessage fromJsonObject( const QJsonObject& jsonObject);

protected:
    void toJsonObject(QJsonObject& jsonObject) const override;

private:
    ConfigStatus configStatus;

};

#endif // CONFIGURATIONMESSAGE_H
