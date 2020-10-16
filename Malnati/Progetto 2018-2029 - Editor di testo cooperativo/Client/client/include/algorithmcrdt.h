#ifndef ALGORITHMCRDT_H
#define ALGORITHMCRDT_H

#include <Qt>
#include <vector>

#include "include/symbol.h"
#include "include/message.h"
#include "include/charactermessage.h"

class AlgorithmCRDT {

public:

    enum OperationSource {NONE, REMOTE, LOCAL};

    AlgorithmCRDT();

    CharacterMessage localInsert(QChar value, unsigned int index, QTextCharFormat char_format, Qt::Alignment alignment);
    CharacterMessage localErase(int index);
    unsigned int remoteInsert(const Symbol &symbol);
    unsigned int remoteErase(const Symbol &symbol);
    void setSiteID( const int siteId );
    int getSiteID() const;
    OperationSource getLastOperationSource() const;
    void setLastOperationSource(OperationSource source);

    void clear();

    std::string to_string() const;

    std::vector<int> getTextAuthors() const;

private:
    int site_id_; /** unique identifier of client */
    int counter_; /** unique identifier of symbol inserted from client */
    std::vector<Symbol> symbols_; /** vector of symbols */
    int base_; /** first level dimension, base to calculate level 1+ */
    unsigned int boundary_; /** max of range to randomly choose position */
    // std::string strategy_;
    OperationSource last_operation_source_;

    enum BoundaryType {PLUS, MINUS};

    std::vector<unsigned int> findPosBefore(unsigned int index);
    std::vector<unsigned int> findPosAfter(unsigned int index);
    Symbol generateSymbol(QChar value, unsigned int index, QTextCharFormat char_format, Qt::Alignment alignment);
    std::vector<unsigned int> generatePosBetween(std::vector<unsigned int> pos_before, std::vector<unsigned int> pos_after, std::vector<unsigned int> new_pos = {}, int level = 0);
    unsigned int generatePosIndex(unsigned int min, unsigned int max, AlgorithmCRDT::BoundaryType boundary_type);
    AlgorithmCRDT::BoundaryType choiceBoundary(int level);

    unsigned int findInsertPosition(const Symbol &symbol);
};

#endif // ALGORITHMCRDT_H
