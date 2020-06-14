//
// Created by rolud on 07/06/20.
//

#ifndef ESERCITAZIONE3_FUNCTIONS_H
#define ESERCITAZIONE3_FUNCTIONS_H

#include <fstream>
#include <memory>
#include <regex>
#include <vector>
#include <boost/property_tree/json_parser/detail/read.hpp>

std::shared_ptr<char[]> char_vector_to_char_shared_ptr(std::vector<char> v) {
    size_t sz = v.size();
    std::shared_ptr<char[]> ptr(new char[sz+1]);
    for (size_t i = 0; i < sz; i++) { ptr.get()[i] = v[i]; }
    ptr.get()[sz] = '\0';
    return ptr;
}


// pipe operations

void write_to_pipe(int fd, std::vector<char> buffer) {
    write(fd, reinterpret_cast<char*>(&buffer[0]), buffer.size());
}

std::vector<char> read_from_pipe(int fd) {
    std::vector<char> b{};
    char c = ' ';
    while (c != '\0') {
        read(fd, &c, 1);
        b.push_back(c);
    }
    return b;
}

// MapReducer pattern
template<typename MapperInputT, typename ResultT>
ResultT map_ip(const MapperInputT& input) {
    std::vector<char> json = input.serialize();
    MapperInputT m("bah");
    m.deserialize(char_vector_to_char_shared_ptr(json));
    std::string line = input.getContent();
    std::smatch match;
    std::regex regex_exp("^[^ ]+");
    std::regex_search(line, match, regex_exp);
    return ResultT(match[0], 1);
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

    std::map<std::string, ResultT> accs{};

    std::ifstream ifs(input_filename);
    if (!ifs.is_open()) {
        std::cout << "Error occurred while opening file." << std::endl;
        return std::map<std::string, ResultT>();
    }
    std::string line;
    while(std::getline(ifs, line)) {
        ResultT res = mapfun(MapperInputT(line));
        auto element = accs.find(res.getKey());
        ResultT acc = element == accs.end() ? ResultT(res.getKey(), 0) : element->second;
        ResultT new_acc = reducefun(ReducerInputT(res.getKey(), res.getValue(), acc.getValue()));
        accs[new_acc.getKey()] = new_acc;
    }


    return accs;
}

template<typename MapperInputT, typename ResultT, typename ReducerInputT, typename M, typename R>
std::map<std::string, ResultT> mapreduce_concurrent(const std::string &input_filename, M& mapfun, R& reducefun) {

    int mapper_pipe[2];
    int reducer_pipe[2];
    int result_pipe[2];
    pid_t c_mapper, c_reducer;
    std::map<std::string, ResultT> accs{};

    if (pipe(mapper_pipe) < 0 || pipe(reducer_pipe) < 0 || pipe(result_pipe) < 0) {
        std::cout << "PIPE CREATION ERROR" << std::endl;
        return accs;
    }

    c_mapper = fork();

    if (c_mapper == 0) {
        // child mapper

        struct timeval tval{};
        fd_set cset;
        FD_ZERO(&cset);
        FD_SET(mapper_pipe[0], &cset);
        tval.tv_sec = 60;
        tval.tv_usec = 0;
        int map_tout = 1;
        MapperInputT map;
        while ((map_tout = select(FD_SETSIZE, &cset, nullptr, nullptr, &tval)) == 1) {
            std::vector<char> map_v = read_from_pipe(mapper_pipe[0]);
            std::cout << "map_v " << reinterpret_cast<char*>(&map_v[0]) << std::endl;
            map.deserialize(char_vector_to_char_shared_ptr(map_v));
            if (map.getContent().compare("END") == 0) break;
            ResultT map_res = mapfun(map);
            write_to_pipe(result_pipe[1], map_res.serialize());
        }
    } else {
        // parent
        c_reducer = fork();
        if (c_reducer == 0) {
            // child reducer

            struct timeval tval{};
            fd_set cset;
            FD_ZERO(&cset);
            FD_SET(reducer_pipe[0], &cset);
            tval.tv_sec = 60;
            tval.tv_usec = 0;
            int red_tout = 1;
            ReducerInputT red;
            while ((red_tout = select(FD_SETSIZE, &cset, nullptr, nullptr, &tval)) == 1) {
                std::vector<char> red_v = read_from_pipe(reducer_pipe[0]);
                std::cout << "red" << std::endl;
                red.deserialize(char_vector_to_char_shared_ptr(red_v));
                if (red.getKey().compare("END") == 0) break;
                ResultT red_res = reducefun(red);
                write_to_pipe(result_pipe[1], red_res.serialize());
            }

        } else {
            // parent
            std::ifstream ifs(input_filename);
            if (ifs.is_open()) {
                std::string line;
                while (std::getline(ifs, line)) {
                    MapperInputT m = MapperInputT(line);
                    write_to_pipe(mapper_pipe[1], m.serialize());

                    ResultT map_res;
                    std::vector<char> map_res_v = read_from_pipe(result_pipe[0]);
                    std::cout << "parent 1" << std::endl;
                    map_res.deserialize(char_vector_to_char_shared_ptr(map_res_v));

                    auto element = accs.find(map_res.getKey());
                    ResultT acc = element == accs.end() ? ResultT(map_res.getKey(), 0) : element->second;

                    ReducerInputT red_input = ReducerInputT(map_res.getKey(), map_res.getValue(), acc.getValue());
                    write_to_pipe(reducer_pipe[1], red_input.serialize());

                    ResultT red_res;
                    std::vector<char> red_res_v = read_from_pipe(result_pipe[0]);
                    std::cout << "parent 1" << std::endl;
                    red_res.deserialize(char_vector_to_char_shared_ptr(red_res_v));
                    accs[red_res.getKey()];

                }
            }
            write_to_pipe(mapper_pipe[1], MapperInputT("END").serialize());
            write_to_pipe(reducer_pipe[1], ReducerInputT("END", -1, -1).serialize());
            close(mapper_pipe[0]);
            close(mapper_pipe[1]);
            close(reducer_pipe[0]);
            close(reducer_pipe[1]);
            close(result_pipe[0]);
            close(result_pipe[1]);
            ifs.close();
            return accs;
        }
    }

    close(mapper_pipe[0]);
    close(mapper_pipe[1]);
    close(reducer_pipe[0]);
    close(reducer_pipe[1]);
    close(result_pipe[0]);
    close(result_pipe[1]);
    return accs;
}


#endif // ESERCITAZIONE3_FUNCTIONS_H