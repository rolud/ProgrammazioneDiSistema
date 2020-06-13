//
// Created by rolud on 07/06/20.
//

#ifndef ESERCITAZIONE3_FUNCTIONS_H
#define ESERCITAZIONE3_FUNCTIONS_H

#include <fstream>
#include <memory>
#include <regex>
#include <vector>

std::shared_ptr<char*> char_vector_to_char_shared_ptr(std::vector<char> v) {
    size_t sz = v.size();
    char *str = new char[sz + 1];
    for (size_t i = 0; i < sz; i++) { str[i] = v[i]; }
    str[sz] = '\0';
    return std::shared_ptr<char*>(&str);
}

template<typename MapperInputT, typename ResultT>
std::vector<ResultT> map_ip(const MapperInputT& input) {
    std::string line = input.getContent();
    std::smatch match;
    std::regex regex_exp("^[^ ]+");
    std::regex_search(line, match, regex_exp);
    return std::vector<ResultT> { ResultT(match[0], 1) };
}

//template<typename MapperInputT, typename ResultT>
//std::vector<ResultT> map_request(const MapperInputT& input) {
//    std::string line = input.getContent();
//    std::smatch match;
//    std::regex regex_exp("\"(.*?)\"");
//    std::regex_search(line, match, regex_exp);
//    std::string str = match[1];
//    std::string request = str.substr(0, str.find(" "));
//    return std::vector<ResultT> { ResultT(request, 1) };
//}

template<typename ReducerInputT, typename ResultT>
ResultT reduce_ip(const ReducerInputT& input) {
    return ResultT(input.getKey(), input.getValue() + input.getAccumulator());
}

template<typename MapperInputT, typename ResultT, typename ReducerInputT, typename M, typename R>
std::map<std::string, ResultT> mapreduce(const std::string &input_filename, M& mapfun, R& reducefun) {

    std::map<std::string, ResultT> accs;
    std::ifstream ifs(input_filename);
    if (!ifs.is_open()) {
        std::cout << "Error occurred while opening file." << std::endl;
        return std::map<std::string, ResultT>();
    }
    std::string line;
    while(std::getline(ifs, line)) {
        std::vector<ResultT> res = mapfun(MapperInputT(line));
        std::for_each(res.begin(), res.end(), [&](ResultT& r) {
            auto element = accs.find(r.getKey());
            ResultT acc = element == accs.end() ? ResultT(r.getKey(), 0) : element->second;
            ResultT new_acc = reducefun(ReducerInputT(r.getKey(), r.getValue(), acc.getValue()));
            accs[new_acc.getKey()] = new_acc;
        });
    }
    return accs;
}

#endif // ESERCITAZIONE3_FUNCTIONS_H