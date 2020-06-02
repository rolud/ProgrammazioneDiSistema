//
// Created by rolud on 27/05/2020.
//

#ifndef STUDIES_SMART_PTR_H
#define STUDIES_SMART_PTR_H

template <typename T>
class smart_ptr {
    T* ptr;
    smart_ptr(const smart_ptr<T>&);
    smart_ptr<T>&operator=(const smart_ptr<T>&);
public:
    explicit smart_ptr(T* p = 0) : ptr(p) {}
    ~smart_ptr() { delete ptr; }
    T&operator*() { return *ptr; }
    T*operator->() { return ptr; }
};

#endif //STUDIES_SMART_PTR_H
