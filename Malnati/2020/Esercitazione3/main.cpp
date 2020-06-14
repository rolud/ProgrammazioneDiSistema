#include <algorithm>
#include <iostream>
#include <map>
#include "DurationLogger.h"
#include "functions.h"
#include "MapperInputT.h"
#include "ResultT.h"
#include "ReducerInputT.h"


int main() {
//    std::string filename("../localhost_access_log.2020.txt");
    std::string filename("../input_file.txt");

    DurationLogger dl("mapreduce");
    auto res_ip = mapreduce_concurrent<MapperInputT, ResultT<int>, ReducerInputT<int, int>>(
            filename,
            map_ip<MapperInputT, ResultT<int>>,
            reduce_ip<ReducerInputT<int,int>, ResultT<int>>
    );

    std::for_each(res_ip.begin(), res_ip.end(), [](auto& r) {
        std::cout << r.first << " : " << r.second.getValue() << std::endl;
    });

    /// 12842598 clocks senza parallelizzazioze
    return 0;
}
