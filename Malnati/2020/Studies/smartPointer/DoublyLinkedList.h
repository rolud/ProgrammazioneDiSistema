//
// Created by rolud on 28/05/2020.
//

#ifndef STUDIES_DOUBLYLINKEDLIST_H
#define STUDIES_DOUBLYLINKEDLIST_H

#include <memory>
#include "DoubnlyLinkedNode.h"

template <typename T>
class DoublyLinkedList {
    std::shared_ptr<DoublyLinkedNode<T>> _head;
    std::shared_ptr<DoublyLinkedNode<T>> _tail;

public:
    explicit DoublyLinkedList(T* data) {
        _head = std::make_shared(data);
        _tail = _head;
    }
};

#endif //STUDIES_DOUBLYLINKEDLIST_H
