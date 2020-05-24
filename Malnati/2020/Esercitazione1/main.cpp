#include <iostream>
#include <ctime>
#include "DurationLogger.h"
#include "Message.h"
#include "MessageStore.h"

void message() {
    DurationLogger dlm("Message function");
    Message msg1(10);
    Message msg2 { 5 };

    std::cout << msg1 << std::endl << msg2 << std::endl;

    Message buff1[10];
    Message *buff2 = new Message[10];

    buff1[0] = Message(20);
    buff1[1] = msg1;

    {
        DurationLogger dl("assign operator");
        for (int i = 0; i < 10; i++) {
            buff1[i] = Message(1048576);
        }

    }

    {
        DurationLogger dl("move assign operator");
        for (int i = 0; i < 10; i++) {
            buff2[i] = buff1[i];
        }
    }

    delete[] buff2;

}


void messageStore() {
    DurationLogger dlms("MessageStore function");
    MessageStore ms(10);

    {
        DurationLogger dl("MessageStore: adding");
        for (int i = 0; i < 100; i++) {
            ms.add( Message(1048576) );
        }
    }

    {
        DurationLogger dl("MessageStore: removing");
        for (int i = 0; i < 50; i++) {
            ms.remove(i);
        }
    }

    {
        DurationLogger dl("MessageStore: compacting");
        ms.compact();
    }

}

int main() {
    DurationLogger dl0("main");

    message();

    messageStore();

    std::cout << "Message allocationCounter: " << Message::getAllocationCounter() << std::endl;

    return 0;
}