//
// Created by rolud on 24/05/2020.
//

#ifndef STUDIES_MYSTRING_H
#define STUDIES_MYSTRING_H


class MyString {

public:
    MyString(int size);
    MyString(const MyString& that);
    MyString(MyString&& that);
    ~MyString();

    MyString& operator = (MyString source);

    int getSize() const;
    char* getString() const;

private:

    friend void swap(MyString& a, MyString& b);

    int size;
    char* str;

};


#endif //STUDIES_MYSTRING_H
