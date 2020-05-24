#include <iostream>

#include "NetworkServer.h"
#include "SharedEditor.h"

int main() {
//    std::cout << "Hello, World!" << std::endl;

    NetworkServer network;
    SharedEditor ed1(network);
    SharedEditor ed2(network);
    ed1.localInsert(0, 'c');
    ed1.localInsert(1, 'a');
    ed1.localInsert(2, 't');
    network.dispatchMessages();
    std::cout<<"ed1: "<<ed1.to_string()<<std::endl;
    std::cout<<"ed2: "<<ed2.to_string()<<std::endl;
    ed1.localInsert(1,'h');
    ed2.localErase(1);
    network.dispatchMessages();
    std::cout<<"ed1: "<<ed1.to_string()<<std::endl;
    std::cout<<"ed2: "<<ed2.to_string()<<std::endl;
    ed2.localInsert(1,'2');
    ed2.localInsert(1,'d');
    ed2.localInsert(1,'2');
    ed2.localInsert(1,'r');
    ed2.localInsert(7,' ');
    ed2.localInsert(8,'c');
    ed2.localInsert(9,'i');
    ed2.localInsert(10,'a');
    ed2.localInsert(11,'o');
    network.dispatchMessages();
    std::cout<<"ed1: "<<ed1.to_string()<<std::endl;
    std::cout<<"ed2: "<<ed2.to_string()<<std::endl;
    ed1.localErase(1);
    ed1.localErase(1);
    ed1.localErase(1);
    ed1.localErase(0);
    ed1.localErase(0);
    ed1.localErase(0);
    ed1.localErase(0);
    network.dispatchMessages();
    std::cout<<"ed1: "<<ed1.to_string()<<std::endl;
    std::cout<<"ed2: "<<ed2.to_string()<<std::endl;
    network.disconnect(&ed1);
    network.disconnect(&ed1);
    return 0;
}