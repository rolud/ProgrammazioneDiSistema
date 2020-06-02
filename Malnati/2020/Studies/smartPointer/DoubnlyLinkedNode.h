//
// Created by rolud on 28/05/2020.
//

#ifndef STUDIES_DOUBNLYLINKEDNODE_H
#define STUDIES_DOUBNLYLINKEDNODE_H

#include <memory>

template <typename T>
class DoublyLinkedNode {
    std::shared_ptr<DoublyLinkedNode<T>> _next;
    std::weak_ptr<DoublyLinkedNode<T>> _prev;
    std::unique_ptr<T> _data;

    DoublyLinkedNode(const DoublyLinkedNode<T>&);
    DoublyLinkedNode<T>&operator=(const DoublyLinkedNode<T>&);

public:
    explicit DoublyLinkedNode(T* data):
    _data(data), _next(nullptr), _prev(nullptr) {}

    DoublyLinkedNode(DoublyLinkedNode<T>* next, DoublyLinkedNode<T>* prev, T* data) :
        _next(next), _prev(prev), _data(data){}

    ~DoublyLinkedNode() {
        _next.reset();
        _prev.reset();
        _data.release();
    }
};
#endif //STUDIES_DOUBNLYLINKEDNODE_H
