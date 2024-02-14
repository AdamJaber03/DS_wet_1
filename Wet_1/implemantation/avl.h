
#ifndef DS_WET_1_AVL_H
#define DS_WET_1_AVL_H

#include "node.h"
#include "wet1util.h"
#include <cmath>

template <typename T, typename S>
class avl{
public:
    avl();
    virtual ~avl();
    StatusType push(S & key, T &value);
    StatusType remove(S & key);
    T * find(S & key);
    void changeUnique();
    avl(avl& toCopy);
    avl& operator=(const avl& toCopy);

private:
    node<T, S> * root;
    int size;
    bool unique = true;
    T * findAux(S & key);
    void fixTree(node<T, S> * start);
    void rotateRR(node<T, S> * toFix);
    void rotateLL(node<T, S> * toFix);
    void rotateRL(node<T, S> * toFix);
    void rotateLR(node<T, S> * toFix);
    void destructorAux(node<T, S> * cur);
    void copyAux(node<T, S> * cur, node<T, S> * curCopy);

};




#endif //DS_WET_1_AVL_H
