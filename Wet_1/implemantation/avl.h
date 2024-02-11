
#ifndef DS_WET_1_AVL_H
#define DS_WET_1_AVL_H

#include "node.h"

template <typename T, typename S>
class avl{
public:
    avl();
    virtual ~avl() = default;
    void push(S & key, T &value);
    void remove(S & key);
    T &find(S & key);

private:
    node<T, S> root;
    int size;
    void rotateRR(node<T, S>);
    void rotateLL(node<T, S>);
    void rotateRL(node<T, S>);
    void rotateLR(node<T, S>);


};

#endif //DS_WET_1_AVL_H
