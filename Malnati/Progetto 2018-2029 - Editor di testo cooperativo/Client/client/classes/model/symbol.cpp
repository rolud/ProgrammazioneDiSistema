#include "include/symbol.h"
#include <algorithm>
#include <string>
#include <QJsonObject>
#include <QJsonArray>

Symbol::Symbol( const std::string& id, const int author, const QChar value, const std::vector<unsigned int>& position)
    : id_(id), author_(author), value_(value), position_(position) {
    this->is_italic_ = false;
    this->is_underline_ = false;
    this->weight_ = 0;
}

Symbol::Symbol( const std::string& id, const int author, const QChar value, const std::vector<unsigned int>& position, const QTextCharFormat format)
    : id_(id), author_(author), value_(value), position_(position) {
    this->is_italic_ = format.fontItalic();
    this->is_underline_ = format.fontUnderline();
    this->weight_ = format.fontWeight();
}

Symbol::Symbol(
        const std::string& id,
        const int author,
        const QChar value,
        const std::vector<unsigned int>& position,
        const bool is_italic,
        const bool is_underline,
        const int weigth,
        const QString fontFamily,
        const qreal fontSize,
        const AlignmentType alignment
    ) : id_(id), author_(author), value_(value), position_(position), is_italic_(is_italic), is_underline_(is_underline), weight_(weigth), font_family_(fontFamily), font_size_(fontSize), alignment_(alignment) {}

void Symbol::setStyle(const QTextCharFormat &format, Qt::Alignment alignment) {
    this->is_italic_ = format.fontItalic();
    this->is_underline_ = format.fontUnderline();
    this->weight_ = format.fontWeight();
    this->font_family_ = format.fontFamily();
    this->font_size_ = format.fontPointSize();
    switch (alignment) {
        case Qt::AlignLeft:
            this->alignment_ = Symbol::LEFT;
            break;
        case Qt::AlignRight:
            this->alignment_ = Symbol::RIGHT;
            break;
        case Qt::AlignCenter:
            this->alignment_ = Symbol::CENTER;
            break;
        case Qt::AlignJustify:
            this->alignment_ = Symbol::JUSTIFY;
            break;
        default:
            this->alignment_ = Symbol::LEFT;
    }
}

std::string Symbol::getID() const {
    return this->id_;
}

int Symbol::getAuthor() const {
    return this->author_;
}

QChar Symbol::getValue() const {
    return this->value_;
}

std::vector<unsigned int> Symbol::getPosition() const {
    return this->position_;
}

bool Symbol::isItalic() const {
    return this->is_italic_;
}

bool Symbol::isUnderline() const {
    return this->is_underline_;
}

int Symbol::getWeight() const {
    return this->weight_;
}

QString Symbol::getFontFamily() const {
    return this->font_family_;
}

qreal Symbol::getFontSize() const {
    return this->font_size_;
}

Qt::Alignment Symbol::getAlignment() const {
    switch (this->alignment_) {
        case Symbol::LEFT: return Qt::AlignLeft;
        case Symbol::RIGHT: return Qt::AlignRight;
        case Symbol::CENTER: return Qt::AlignCenter;
        case Symbol::JUSTIFY: return Qt::AlignJustify;
    }
    return Qt::AlignLeft;
}

bool Symbol::operator<(const Symbol &s) {
    auto l = this->getPosition();
    auto r = s.getPosition();
    unsigned int min = std::min(l.size(), r.size());
    for (unsigned int i = 0; i < min; i++) {
        unsigned int l1 = l[i];
        unsigned int r1 = r[i];
        if (l1 < r1)
            return true;
        else if (l1 > r1)
            return false;
    }
    if (l.size() < r.size())
        return true;
    return false;
}

bool Symbol::operator>(const Symbol &s) {
    auto l = this->getPosition();
    auto r = s.getPosition();
    unsigned int min = std::min(l.size(), r.size());
    for (unsigned int i = 0; i < min; i++) {
        unsigned int l1 = l[i];
        unsigned int r1 = r[i];
        if (l1 > r1)
            return true;
        else if (l1 < r1)
            return false;
    }
    if (l.size() > r.size())
        return true;
    return false;
}


void Symbol::toJsonObject( QJsonObject& jsonObject ) const
{
    jsonObject[ "id" ] = this->getID().c_str();
    jsonObject["author"] = this->getAuthor();
    jsonObject[ "value" ] = QString(this->getValue());

    QJsonArray position{};

    for ( const unsigned int value : this->getPosition() )
    {
        position.append( static_cast<int>(value) );
    }

    jsonObject[ "position" ] = position;
    jsonObject["isItalic"] = this->is_italic_;
    jsonObject["isUnderline"] = this->is_underline_;
    jsonObject["weight"] = this->weight_;
    jsonObject["alignment"] = this->alignment_;
    jsonObject["fontFamily"] = this->font_family_;
    jsonObject["fontSize"] = this->font_size_;
}

Symbol Symbol::fromJsonObject( const QJsonObject& jsonObject )
{
    if ( !jsonObject.contains( "id" ) || !jsonObject[ "id" ].isString() )
    {
        throw std::invalid_argument{ "QJsonObject does not contain 'id'." };
    }
    if ( !jsonObject.contains("author") || !jsonObject["author"].isDouble() ) {
        throw std::invalid_argument{ "QJsonObject does not contain 'author'." };
    }

    if ( !jsonObject.contains( "value" ) || !jsonObject[ "value" ].isString() )
    {
        throw std::invalid_argument{ "QJsonObject does not contain 'value'." };
    }

    if ( !jsonObject.contains( "position" ) || !jsonObject[ "position" ].isArray() )
    {
        throw std::invalid_argument{ "QJsonObject does not contain 'position'." };
    }

    if ( !jsonObject.contains("alignment") || !jsonObject["alignment"].isDouble())
    {
        throw std::invalid_argument{ "QJsonObject does not contain 'position'." };
    }

    if ( !jsonObject.contains("fontFamily") || !jsonObject["fontFamily"].isString())
    {
        throw std::invalid_argument{ "QJsonObject does not contain 'fontFamily'." };
    }

    if ( !jsonObject.contains("fontSize") || !jsonObject["fontSize"].isDouble())
    {
        throw std::invalid_argument{ "QJsonObject does not contain 'fontSize'." };
    }


    std::string id = jsonObject[ "id" ].toString().toStdString();
    int author = jsonObject["author"].toInt();
    QChar value = (jsonObject[ "value" ].toString().at(0));

    QJsonArray positionArray = jsonObject[ "position" ].toArray();
    std::vector<unsigned int> position{};

    for ( QJsonValueRef v : positionArray )
    {
        position.push_back( static_cast<unsigned int>(v.toInt()) );
    }

    QString fontFamily = jsonObject["fontFamily"].toString();
    qreal fontSize = jsonObject["fontSize"].toDouble();

    bool isItalic = false;
    bool isUnderline = false;
    int weight = 50;

    if (jsonObject.contains("isItalic") && jsonObject["isItalic"].isBool())
        isItalic = jsonObject["isItalic"].toBool();

    if (jsonObject.contains("isUnderline") && jsonObject["isUnderline"].isBool())
        isUnderline = jsonObject["isUnderline"].toBool();

    if (jsonObject.contains("weight") && jsonObject["weight"].isDouble())
        weight = jsonObject["weight"].toInt();

    auto alignment = Symbol::AlignmentType(jsonObject["alignment"].toInt());

    return Symbol{ id, author, value, position, isItalic, isUnderline, weight,fontFamily, fontSize, alignment };
}
