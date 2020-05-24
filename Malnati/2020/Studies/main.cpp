#include <iostream>
#include "Buffer.h"
int main() {
    int sz = 5;
    Buffer bff { sz };
    std::cout << " buffer size: " << sz << std::endl;
    for (int i = 0; i < sz; i++) {
        bool res = bff.setValue(i, i);
        std::cout << "Inserting value " << i << " at " << i << " | success: " << res << std::endl;
    }
    for (int i = 0; i < sz; i++) {
        int v = -1;
        bool res = bff.getValue(i, v);
        std::cout << "Getting value " << v << " from " << i << " | success: " << res << std::endl;
    }
    return 0;
}