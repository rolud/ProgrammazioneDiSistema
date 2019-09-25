//
// Created by rolud on 25/09/2019.
//

#ifndef ESERCITAZIONE3_SHAREDEDITOR_H
#define ESERCITAZIONE3_SHAREDEDITOR_H

#include <string>
#include <vector>

#include "Message.h"
#include "Symbol.h"

class NetworkServer;

class SharedEditor {
public:
    explicit SharedEditor(NetworkServer& server);
    void localInsert(int index, char value);
    void localErase(int index);
    void process(const Message& msg);
    std::string to_string();

    int get_siteId() const;
    const std::vector<Symbol> &get_symbols() const;

private:
    NetworkServer& _server;
    int _siteId;
    std::vector<Symbol> _symbols;
    int _counter;
};


#endif //ESERCITAZIONE3_SHAREDEDITOR_H
