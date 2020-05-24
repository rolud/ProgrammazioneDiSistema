//
// Created by rolud on 25/09/2019.
//

#include <string>
#include <cmath>
#include <random>
#include "SharedEditor.h"
#include "NetworkServer.h"
#include "Message.h"

SharedEditor::SharedEditor(NetworkServer &server) : server_(server) {
    siteId_ = server_.connect(this);
    counter_ = 100;
    boundery_ = 10;
    base_ = 32;
}

int SharedEditor::get_siteId() const {
    return siteId_;
}

const std::vector<Symbol> &SharedEditor::get_symbols() const {
    return symbols_;
}

/**
 * inserisce il simbolo nel vettore visualizzato in locale
 * crea e invia al server il messaggio con l'azione di inserimento
 *
 * @param index
 * @param value
 */
void SharedEditor::localInsert(int index, char value) {
    const Symbol sym = generateSymbol(index, value);
    if (index == symbols_.size())
        symbols_.push_back(sym);
    else
        symbols_.insert(symbols_.begin() + index, sym);
    Message msg(sym, Message::Action::INSERT, siteId_);
    server_.send(msg);
}

/**
 * crea il simbolo e aumenta il contatore per l'identificativo
 *
 * @param index
 * @param value
 * @return
 */
Symbol SharedEditor::generateSymbol(int index, char value) {
    const std::vector<int> posBefore = findPosBefore(index);
    const std::vector<int> posAfter  = findPosAfter(index);
    const std::vector<int> newPos    = generatePosBetween(posBefore, posAfter);
    counter_++;
    Symbol newSym(std::to_string(siteId_) + std::to_string(counter_), value, newPos);
    return newSym;
}

std::vector<int> SharedEditor::findPosBefore(int index) {
    if (index == 0)
        return {};
    else if (index == symbols_.size())
        return  symbols_[index - 1].get_position();
    else
        return symbols_[index - 1].get_position();
}

std::vector<int> SharedEditor::findPosAfter(int index) {
    if (index == symbols_.size())
        return {};
    else
        return symbols_[index].get_position();
}

/**
 * funzione ricorsiva per ricercare in quale posizione inserire il nuovo simbolo
 * analizza le due posizioni tra le quali il simbolo deve essere inserito, se c'è spazio tra le due posizioni
 * inserisce subito, altrimenti scende di livello nell'albero
 *
 * @param posBefore
 * @param posAfter
 * @param newPos
 * @param level
 * @return il nuovo vettore posizione
 */
std::vector<int> SharedEditor::generatePosBetween(std::vector<int> posBefore, std::vector<int> posAfter, std::vector<int> newPos, int level) {
    int pos1, pos2;
    int base = static_cast<int>(pow(2, level) * base_);
    Boundary boundary_strategy = choiceBoundaryStrategy(level);

    if (level <= posBefore.size() && !posBefore.empty())
        pos1 = posBefore[level];
    else
        pos1 = 0;

    if (!posAfter.empty())
        pos2 = posAfter[0];
    else
        pos2 = base;

    if (pos2 - pos1 > 1) {
        // we can insert
        int pos = generatePos(pos1, pos2, boundary_strategy);
        newPos.push_back(pos);
        return newPos;
    } else if (pos2 - pos1 == 1) {
        // explore next level
        newPos.push_back(pos1);
        return generatePosBetween(posBefore, {}, newPos, level + 1);
    } // todo handle pos2==pos1
}

/**
 * genera casualmente la posizione in base alla strategia:
 * boundary+ : posizione casuale vicina al limite inferiore
 * boundary- : posizione casuale vicina al limite superiore
 *
 * @param min : limite inferiore
 * @param max : limite superiore
 * @param boundaryStrategy : + per boundary+, - per boundary-
 * @return posizione selezionata
 */
int SharedEditor::generatePos(int min, int max, Boundary boundaryStrategy) {
    if ((max - min) < boundery_) {
        min = min + 1;
    } else {
        if (boundaryStrategy == Boundary::PLUS) {
            min = max - boundery_;
        } else if (boundaryStrategy == Boundary::MINUS){
            min = min + 1;
            max = min + boundery_;
        }
    }
    std::random_device rd;
    std::mt19937 e{rd()};
    std::uniform_int_distribution<int> dist{min, max};
    return dist(e);
}

/**
 * determina la strategia di confinamento
 * al momento alterna boundary+ e boundary- con la profondità del livello
 * è possibile implementare altre strategie
 *
 * @param level
 * @return
 */
SharedEditor::Boundary SharedEditor::choiceBoundaryStrategy(int level) {
    return (level % 2) == 0 ? Boundary::PLUS : Boundary::MINUS; // todo we can implement other strategy
}

/**
 * elimina il symbolo dal vettore locale
 * crea e invia al server il messaggio di eliminazione
 *
 * @param index
 */
void SharedEditor::localErase(int index) {
    Symbol sym = symbols_[index];
    symbols_.erase(symbols_.begin() + index);
    Message msg(sym, Message::Action::DELETE, siteId_);
    server_.send(msg);
}

/**
 * ricevuto il messaggio dal server, compie l'azione contenuta
 *
 * @param msg
 */
void SharedEditor::process(const Message &msg) {
    switch (msg.get_action()) {
        case Message::Action::INSERT:
            remoteInsert(msg.get_symbol());
            break;
        case Message::Action::DELETE:
            remoteErase(msg.get_symbol());
            break;
    }
}

/**
 * inserisce un simbolo creato da un altro editor
 *
 * @param symbol
 */
void SharedEditor::remoteInsert(const Symbol &symbol) {
    int index = findInsertPosition(symbol);
    symbols_.insert(symbols_.begin() + index, symbol);
}

int SharedEditor::findInsertPosition(const Symbol &symbol) {

    if (symbols_.empty())
        return 0;

    if (symbols_[symbols_.size() - 1] < symbol)
        return symbols_.size();

    for (int i = 0; i < symbols_.size(); i++) {
        if (symbols_[i] > symbol) {
            return i;
        }
    }
}

/**
 * elimina un simbolo cancellato da un altro editor
 *
 * @param symbol
 */
void SharedEditor::remoteErase(const Symbol &symbol) {
    for (int i = 0; i < symbols_.size(); i++) {
        if (symbols_[i].get_id() == symbol.get_id())
            symbols_.erase(symbols_.begin() + i);
    }
}

/**
 * converte il vettore di simboli in stringa
 *
 * @return
 */
std::string SharedEditor::to_string() {
    std::string string;
    for (const Symbol &s : symbols_) {
        string += s.get_value();
    }
    return string;
}







