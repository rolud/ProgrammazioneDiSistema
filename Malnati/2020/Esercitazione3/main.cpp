#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <vector>
#include "MapperInputT.h"
#include "ResultT.h"
#include "ReducerInputT.h"

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

int main() {
    std::string filename("../localhost_access_log.2020.txt");
//    std::string filename("../input_file.txt");

    clock_t start_time = std::clock();
    auto res_ip = mapreduce<MapperInputT, ResultT<int>, ReducerInputT<int, int>>(
            filename,
            map_ip<MapperInputT, ResultT<int>>,
            reduce_ip<ReducerInputT<int,int>, ResultT<int>>
    );
    clock_t end_time = std::clock();

    std::for_each(res_ip.begin(), res_ip.end(), [](auto& r) {
        std::cout << r.first << " : " << r.second.getValue() << std::endl;
    });

    std::cout << "Duration time: " << end_time - start_time << " clocks" << std::endl;

    /// 12842598 clocks senza parallelizzazioze
    return 0;
}
