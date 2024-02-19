#include "node.h"
#include <cmath>

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
const node<T, S> *node<T, S>::getLeft() {
    return left;
}

template<typename T, typename S>
const node<T, S> *node<T, S>::getRight() {
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
