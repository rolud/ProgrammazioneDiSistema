//
// Created by rolud on 15/06/2020.
//

#ifndef ESERCITAZIONE4_LINE_H
#define ESERCITAZIONE4_LINE_H

class Line {
public:
    Line() = default;
    Line(const std::string& value, const std::string& filename, int line)
        : m_value(value), m_filename(filename), m_line(line) {}

    std::string getValue() const { return this->m_value; }
    std::string getFilename() const { return this->m_filename; }
    int getLine() const { return this->m_line; }
private:
    std::string m_value;
    std::string m_filename;
    int m_line;
};
#endif //ESERCITAZIONE4_LINE_H
