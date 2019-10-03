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

    Symbol genereteSymbol(int index, char value);
    std::vector<int> generatePosBetween(std::vector<int> posBefore, std::vector<int> posAfter, std::vector<int> newPos = {}, int level = 0);
    int generatePos(int min, int max, std::string boundaryStrategy);
    std::string choiceBoundaryStrategy(int level);

    NetworkServer& _server;
    int _siteId;
    std::vector<Symbol> _symbols;
    int _counter;

    int _base;
    int _boundery;
    std::string _strategy;
};


#endif //ESERCITAZIONE3_SHAREDEDITOR_H
