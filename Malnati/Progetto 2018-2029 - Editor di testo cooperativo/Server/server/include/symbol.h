#ifndef SYMBOL_H
#define SYMBOL_H

#include <vector>
#include <string>
#include <Qt>
#include <QJsonObject>
#include <QTextCharFormat>

class Symbol {
public:
    enum AlignmentType {LEFT, CENTER, RIGHT, JUSTIFY};

    Symbol( const std::string& id, const int author, const QChar value, const std::vector<unsigned int>& position);
    Symbol( const std::string& id, const int author, const QChar value, const std::vector<unsigned int>& position, const QTextCharFormat format);
    Symbol( const std::string& id, const int author, const QChar value, const std::vector<unsigned int>& position, const bool is_italic, const bool is_underline, const int weight, QString fontFamily, qreal fontSize, AlignmentType alignment);

    void setStyle(const QTextCharFormat& format, Qt::Alignment alignment);

    std::string getID() const;
    int getAuthor() const;
    QChar getValue() const;
    std::vector<unsigned int> getPosition() const;
    bool isItalic() const;
    bool isUnderline() const;
    int getWeight() const;
    Qt::Alignment getAlignment() const;
    QString getFontFamily() const;
    qreal getFontSize() const;

    bool operator < (const Symbol &s);
    bool operator > (const Symbol &s);

    void toJsonObject( QJsonObject& jsonObject ) const;
    static Symbol fromJsonObject( const QJsonObject& jsonObject );


private:
    std::string id_;
    int author_;
    QChar value_;
    std::vector<unsigned int> position_;
    bool is_italic_;
    bool is_underline_;
    int weight_;
    QString font_family_;
    qreal font_size_;
    AlignmentType alignment_;
};

#endif // SYMBOL_H
