
#ifndef DS_WET_1_AVL_H
#define DS_WET_1_AVL_H

#include "node.h"
#include "../Scripts/wet1util.h"
#include <cmath>

template <typename T, typename S>
class avl{
public:
    avl();
    virtual ~avl();
    StatusType insert(T & key, S &value);
    StatusType remove(T & key);
    S * find(T & key);
    void changeUnique();

private:
    node<T, S> * root;
    int size;
    bool unique = true;
    node<T, S> * findAux(T & key);
    void fixTree(node<T, S> * start);
    void rotateRR(node<T, S> * toFix);
    void rotateLL(node<T, S> * toFix);
    void rotateRL(node<T, S> * toFix);
    void rotateLR(node<T, S> * toFix);
    void destructorAux(node<T, S> * cur);

};




#endif //DS_WET_1_AVL_H
