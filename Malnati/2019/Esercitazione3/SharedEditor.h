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



    NetworkServer& server_;
    int siteId_;
    std::vector<Symbol> symbols_;
    int counter_;

    int base_;
    int boundery_;
    std::string strategy_;

    enum Boundary {PLUS, MINUS};

    std::vector<int> findPosBefore(int index);
    std::vector<int> findPosAfter(int index);
    Symbol generateSymbol(int index, char value);
    std::vector<int> generatePosBetween(std::vector<int> posBefore, std::vector<int> posAfter, std::vector<int> newPos = {}, int level = 0);
    int generatePos(int min, int max, SharedEditor::Boundary boundaryStrategy);
    SharedEditor::Boundary choiceBoundaryStrategy(int level);
    void remoteInsert(const Symbol &symbol);
    void remoteErase(const Symbol &symbol);

    int findInsertPosition(const Symbol &symbol);

};


#endif //ESERCITAZIONE3_SHAREDEDITOR_H
