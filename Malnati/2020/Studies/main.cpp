#include <iostream>
#include <memory>
#include "Buffer.h"
#include "patternCRTP/Amount.h"
#include "patternCRTP/Constant42.h"
#include "patternCRTP/Variable.h"
#include "smartPointer/DoublyLinkedList.h"

std::weak_ptr<int> gw;

template <typename T>
void print(Amount<T> amount) {
    std::cout << amount.getValue() << std::endl;
}

void fooWP() {
    if (auto spt = gw.lock())
        std::cout << "gw: " << *spt << std::endl;
    else
        std::cout << "gw scaduto" << std::endl;
}

int main() {

    Constant42 c42;
    Variable v;

    print(c42);
    print(v);

    {
        auto sp = std::make_shared<int>(42);
        gw = sp;
        fooWP();
    }
    fooWP();

    // generica doubly linked list
    DoublyLinkedList<std::string> dll(new std::string("ciao"));

    return 0;
}