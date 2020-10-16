#include <iostream>
#include <cmath>
#include <random>
#include "include/algorithmcrdt.h"

AlgorithmCRDT::AlgorithmCRDT() {
    this->site_id_ = -1;
    this->counter_ = 100;
    this->boundary_ = 1024;
    this->base_ = 2048;

    this->last_operation_source_ = OperationSource::NONE;
}

int AlgorithmCRDT::getSiteID() const {
    return site_id_;
}

void AlgorithmCRDT::setSiteID( const int siteId )
{
    this->site_id_ = siteId;
}

AlgorithmCRDT::OperationSource AlgorithmCRDT::getLastOperationSource() const {
    return this->last_operation_source_;
}

void AlgorithmCRDT::setLastOperationSource(AlgorithmCRDT::OperationSource source) {
    this->last_operation_source_ = source;
}

CharacterMessage AlgorithmCRDT::localInsert(QChar value, unsigned int index, QTextCharFormat char_format, Qt::Alignment alignment) {
    this->last_operation_source_ = OperationSource::LOCAL;
    const Symbol sym = this->generateSymbol(value, index, char_format, alignment);
    if (index == this->symbols_.size())
        this->symbols_.push_back(sym);
    else
      this->symbols_.insert(symbols_.begin() + static_cast<int>(index), sym);

    return CharacterMessage(this->site_id_, sym, CharacterMessage::OperationType::INSERT);
}

Symbol AlgorithmCRDT::generateSymbol(QChar value, unsigned int index, QTextCharFormat char_format, Qt::Alignment alignment) {
    const std::vector<unsigned int> pos_before = findPosBefore(index);
    const std::vector<unsigned int> pos_after  = findPosAfter(index);
    const std::vector<unsigned int> new_pos    = generatePosBetween(pos_before, pos_after);
    this->counter_++;
    Symbol sym(std::to_string(this->site_id_) + std::to_string(this->counter_), this->site_id_, value, new_pos);
    sym.setStyle(char_format, alignment);
    return sym;
}


std::vector<unsigned int> AlgorithmCRDT::findPosBefore(unsigned int index) {
    if (index == 0)
        return {};
    else if (index == symbols_.size())
        return symbols_[index - 1].getPosition();
    else
        return symbols_[index - 1].getPosition();
}

std::vector<unsigned int> AlgorithmCRDT::findPosAfter(unsigned int index) {
    auto ind = static_cast<unsigned int>(index);
    if (ind == symbols_.size())
        return {};
    else
        return symbols_[ind].getPosition();
}

std::vector<unsigned int> AlgorithmCRDT::generatePosBetween(std::vector<unsigned int> pos_before, std::vector<unsigned int> pos_after, std::vector<unsigned int> new_pos, int level) {
    unsigned int pos1, pos2;
    unsigned int base = static_cast<unsigned int>(pow(2, level) * this->base_);
    BoundaryType boundary_type = this->choiceBoundary(level);

    if (!pos_before.empty() && static_cast<unsigned int>(level) < pos_before.size())
        pos1 = pos_before[static_cast<unsigned int>(level)];
    else
        pos1 = 0;

    if (!pos_after.empty() && static_cast<unsigned int>(level) < pos_after.size())
        pos2 = pos_after[static_cast<unsigned int>(level)];
    else
        pos2 = base;

    if (pos2 - pos1 > 1) { // we can insert
        unsigned int pos = generatePosIndex(pos1, pos2, boundary_type);
        new_pos.push_back(pos);
        return new_pos;
    } else if (pos2 - pos1 == 1) { // explore next level
        new_pos.push_back(pos1);
        return generatePosBetween(pos_before, {}, new_pos, level + 1);
    } else if (pos1 == pos2) { // explore next level
        new_pos.push_back(pos1);
        return generatePosBetween(pos_before, pos_after, new_pos, level + 1);
    } else {
        return {};
    }

}

unsigned int AlgorithmCRDT::generatePosIndex(unsigned int min, unsigned int max, AlgorithmCRDT::BoundaryType boundary_type){
    if ((max - min) < static_cast<unsigned int>(this->boundary_)) {
        min++;
        max--;
    } else {
        if (boundary_type == BoundaryType::MINUS) {
            min++;
            max = min + static_cast<unsigned int>(this->boundary_);
        }
    }
    std::random_device rd;
    std::mt19937 e{rd()};
    std::uniform_int_distribution<unsigned int> dist{min, max};
    return static_cast<unsigned int>(dist(e));
}

AlgorithmCRDT::BoundaryType AlgorithmCRDT::choiceBoundary(int level) {
    return (level % 2) == 0 ? BoundaryType::PLUS : BoundaryType::MINUS;
}

CharacterMessage AlgorithmCRDT::localErase(int index) {
    this->last_operation_source_ = OperationSource::LOCAL;
    if (symbols_.size() <= static_cast<unsigned int>(index)) {
        throw std::invalid_argument("Posizione inesistente");
    }
    Symbol sym = symbols_[static_cast<unsigned int>(index)];
    symbols_.erase(symbols_.begin() + index);

    return CharacterMessage(this->site_id_, sym, CharacterMessage::OperationType::DELETE);
}

unsigned int AlgorithmCRDT::remoteInsert(const Symbol &symbol) {
    this->last_operation_source_ = OperationSource::REMOTE;
    unsigned int index = findInsertPosition(symbol);
    symbols_.insert(symbols_.begin() + static_cast<int>(index), symbol);
    return index;
}

unsigned int AlgorithmCRDT::remoteErase(const Symbol &symbol) {
    this->last_operation_source_ = OperationSource::REMOTE;
    unsigned int index = 0;
    bool erased = false;
    for (unsigned int i = 0; i < symbols_.size(); i++) {
        if (symbols_[i].getID() == symbol.getID()) {
            symbols_.erase(symbols_.begin() + static_cast<int>(i));
            index = i;
            erased = true;
            break;
        }
    }
    if (!erased) throw std::logic_error{ "remoteErase failed. It should not fail" };
    return index;
}

unsigned int AlgorithmCRDT::findInsertPosition(const Symbol &symbol) {
    if (symbols_.empty())
           return 0;

   if (symbols_[symbols_.size() - 1] < symbol)
       return symbols_.size();

   for (unsigned int i = 0; i < symbols_.size(); ++i) {
       if (symbols_[i] > symbol) {
           return i;
       }
   }
   return symbols_.size();
}

void AlgorithmCRDT::clear() {
    this->site_id_ = -1;
    this->counter_ = 100;
    this->symbols_.clear();
    this->last_operation_source_ = NONE;

}

std::string AlgorithmCRDT::to_string() const {
    QString string;
    for (const Symbol &s : symbols_) {
        string += s.getValue();
    }
    return string.toStdString();
}

std::vector<int> AlgorithmCRDT::getTextAuthors() const {
    std::vector<int> res;
    for (auto iter = this->symbols_.begin(); iter != this->symbols_.end(); iter++) {
        res.emplace_back(iter->getAuthor());
    }
    return res;

}

