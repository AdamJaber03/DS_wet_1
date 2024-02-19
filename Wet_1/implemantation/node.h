
#ifndef DS_WET_1_NODE_H
#define DS_WET_1_NODE_H

#include <cmath>

template <typename T, typename S>
class node{
public:
    virtual ~node() = default;
    node(T & key, S & value) : key(key), value(value), height(0) {};
    void setParent(node * parent);
    void setRight(node * right);
    void setLeft(node * left);
    const S & getKey();
    node * getLeft();
    node * getRight();
    node * getParent();
    int getHeight();
    void updateHeight();
    int getBf();
    S & getValue();

private:
    T & key;
    S & value;
    int height;
    node * left;
    node * right;
    node * parent;
};

template<typename T, typename S>
void node<T, S>::setParent(node *parent) {
    this->parent = parent;
}

template<typename T, typename S>
void node<T, S>::setRight(node *right) {
    this->right = right;
}

template<typename T, typename S>
void node<T, S>::setLeft(node *left) {
    this->left = left;
}

template<typename T, typename S>
void node<T, S>::updateHeight() {
    int lheight = left->height ? left: -1;
    int rheight = right->height ? right: -1;
    height = fmax(lheight, rheight) + 1;
}

template<typename T, typename S>
const S &node<T, S>::getKey() {
    return key;
}

template<typename T, typename S>
node<T, S> *node<T, S>::getLeft() {
    return left;
}

template<typename T, typename S>
node<T, S> *node<T, S>::getRight() {
    return right;
}

template<typename T, typename S>
node<T, S> *node<T, S>::getParent() {
    return parent;
}

template<typename T, typename S>
int node<T, S>::getHeight() {
    return height;
}

template<typename T, typename S>
int node<T, S>::getBf() {
    return left->getHeight() - right->getHeight();
}

template<typename T, typename S>
S &node<T, S>::getValue() {
    return value;
}



#endif //DS_WET_1_NODE_H
