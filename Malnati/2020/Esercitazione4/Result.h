//
// Created by rolud on 16/06/2020.
//

#ifndef ESERCITAZIONE4_RESULT_H
#define ESERCITAZIONE4_RESULT_H

#include <regex>
#include <thread>
#include "Line.h"

class Result {
public:
    Result(std::thread::id thread_id, Line line, std::vector<std::smatch> matches)
        : m_thread_id(thread_id), m_line(line), m_matches(matches) {}

    std::thread::id getThreadId() const { return m_thread_id; }
    Line getLine() const { return m_line; }
    std::vector<std::smatch> getMatches() const { return m_matches; }

private:
    std::thread::id m_thread_id;
    Line m_line;
    std::vector<std::smatch> m_matches;
};

#endif //ESERCITAZIONE4_RESULT_H
