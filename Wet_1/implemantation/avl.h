
#ifndef DS_WET_1_AVL_H
#define DS_WET_1_AVL_H

#include "node.h"
#include "../Scripts/wet1util.h"
#include <cmath>
#include <iostream>

template <typename T, typename S>
class avl{
public:
    avl(): root(nullptr), size(0), maxKey(0), minKey(0) {};
    virtual ~avl();
    StatusType insert(T & key, S &value);
    StatusType remove(T & key);
    S * find(T & key);
    void changeUnique();
    T getMax();
    T getMin();

private:
    node<T, S> * root;
    int size;
    bool unique = true;
    int maxKey;
    int minKey;
    T findMax();
    T findMin();
    node<T, S> * findAux(T & key);
    void fixTree(node<T, S> * start);
    void rotateRR(node<T, S> * toFix);
    void rotateLL(node<T, S> * toFix);
    void rotateRL(node<T, S> * toFix);
    void rotateLR(node<T, S> * toFix);
    void destructorAux(node<T, S> * cur);
    void printTree();
    void printTreeAux(node<T, S>* cur);
};

template<typename T, typename S>
void avl<T, S>::printTreeAux(node<T, S>* cur) {
    if (!cur) return;
    printTreeAux(cur->getLeft());
    std::cout << " " << cur->getKey() << " ";
    printTreeAux(cur->getRight());
}

template<typename T, typename S>
void avl<T, S>::printTree() {
    node<T, S> * cur = root;
    printTreeAux(cur);
    std::cout << std::endl;
}


template<typename T, typename S>
StatusType avl<T, S>::insert(T &key, S &value) {
    node<T, S> * newNode = new node<T, S>(key, value);
    std::cout << "newNode value: " << newNode->getValue() << std::endl;
    if (!newNode){    //Todo - check if legal
        return StatusType::ALLOCATION_ERROR;
    }
    if (!root){
        root = newNode;
        size++;
        maxKey = root->getKey();
        minKey = root->getKey();
        return StatusType::SUCCESS;
    }
    if(unique && find(key)){
        return StatusType::FAILURE;
    }
    node<T, S> * parent = findAux(key);
    if (key < parent->getKey()){
        parent->setLeft(newNode);
    } else {
        parent->setRight(newNode);
    }
    newNode->setParent(parent);
    while(parent){
        parent->updateHeight();
        parent = parent->getParent();
    }
    printTree();
    std::cout << "fix reached" << std::endl;
    fixTree(newNode);
    std::cout << "fixed" << std::endl;

    size++;
    maxKey = findMax();
    minKey = findMin();
    printTree();
    return StatusType::SUCCESS;
}

template<typename T, typename S>
node<T, S> *avl<T, S>::findAux(T &key) {
    node<T, S> * cur = root, *parent = nullptr;
    while (cur){
        if (cur->getKey() == key){
            return parent;
        }
        if (key < cur->getKey()){
            parent = cur;
            cur = cur->getLeft();
        } else{
            parent = cur;
            cur = cur->getRight();
        }
    }
    return parent;
}

template<typename T, typename S>
S *avl<T, S>::find(T &key) {
    std::cout << "key to find: " << key << std::endl;
    node<T, S> * parent = findAux(key);
    std::cout << "parent " << parent << std::endl;
    if (!parent && root) return &root->getValue();
    if (!parent) return nullptr;
    std::cout << "parent " << parent->getKey() << std::endl;
    if (parent->getLeft()) std::cout << "parent " << parent->getLeft()->getKey() << std::endl;
    if (parent->getRight()) std::cout << "parent " << parent->getRight()->getKey() << std::endl;
    if (parent->getLeft() && parent->getLeft()->getKey() == key){
        return &parent->getLeft()->getValue();
    } else if(parent->getRight() && parent->getRight()->getKey() == key){
        return &parent->getRight()->getValue();
    }
    return nullptr;
}

template<typename T, typename S>
void avl<T, S>::rotateLL(node<T, S> * toFix) {
    node<T, S> * lSon = toFix->getLeft(), *parent = toFix->getParent();
    node<T, S> * lrson = lSon->getRight();
    //pside is True if tofix is left son
    if (parent){
        bool pside = toFix->getParent()->getKey() > toFix->getKey();
        if (pside){
            parent->setLeft(lSon);
        } else{
            parent->setRight(lSon);
        }
    }else{
        root = lSon;
    }
    lSon->setParent(parent);

    toFix->setLeft(lrson);
    if (lrson) lrson->setParent(toFix);

    lSon->setRight(toFix);
    toFix->setParent(lSon);

    toFix->updateHeight();
    lSon->updateHeight();
}

template<typename T, typename S>
void avl<T, S>::rotateRR(node<T, S> * toFix) {
    node<T, S> * rSon = toFix->getRight(), *parent = toFix->getParent();
    node<T, S> * rlson = rSon->getLeft();
    //pside is True if tofix is left son
    if (parent){
        bool pside = toFix->getParent()->getKey() > toFix->getKey();
        if (pside){
            parent->setLeft(rSon);
        } else{
            parent->setRight(rSon);
        }
    } else{
        root = rSon;
    }
    rSon->setParent(parent);

    toFix->setRight(rlson);
    if (rlson) rlson->setParent(toFix);

    rSon->setLeft(toFix);
    toFix->setParent(rSon);

    toFix->updateHeight();
    rSon->updateHeight();
}

template<typename T, typename S>
void avl<T, S>::rotateLR(node<T, S> * toFix) {
    rotateRR(toFix->getLeft());
    rotateLL(toFix);
}

template<typename T, typename S>
void avl<T, S>::rotateRL(node<T, S> * toFix) {
    rotateLL(toFix->getRight());
    rotateRR(toFix);
}

template<typename T, typename S>
void avl<T, S>::fixTree(node<T, S> *start) {
    node<T, S> * cur = start;
    while (cur){
        printTree();
        cur->updateHeight();
        int bf = cur->getBf();
        std::cout << "bf: " << bf << std::endl;

        if (abs(bf) < 2){
            cur = cur->getParent();
            continue;
        }
        if (bf == 2){
            if (cur->getLeft()->getBf() >= 0){ // Todo understand if 0 or 1
                rotateLL(cur);
            } else{
                rotateLR(cur);
            }
        } else {
            if (cur->getRight()->getBf() <= 0) { // TODO - understand if 0 or -1
                rotateRR(cur);
            } else {
                rotateRL(cur);
            }
        }
        cur = cur->getParent();
    }
}


template<typename T, typename S>
StatusType avl<T, S>::remove(T &key) {
    node<T, S> * parent = findAux(key);
    node<T, S> * toRemove = nullptr;
    if (!parent){
        if (!root) return StatusType::FAILURE;  //TODO - fix
        toRemove = root;
    }
    else if (!parent->getLeft() && !parent->getRight()){
        return StatusType::FAILURE;
    }else{
        bool pside = parent->getKey() > key;
        toRemove = pside? parent->getLeft(): parent->getRight();
    }
    node<T, S> * lson = toRemove->getLeft(), * rson = toRemove->getRight();
    //if toRemove only has one son
    if (!rson && !lson){
        bool isRoot = (toRemove == root);
        try{
            delete toRemove;
        } catch (std::bad_alloc& e){
            return StatusType::ALLOCATION_ERROR;
        }
        if (isRoot){
            root == nullptr;
            return StatusType::SUCCESS;
        }
        bool pside = parent->getKey() > key;
        if (pside){
            parent->setLeft(nullptr);
        } else{
            parent->setRight(nullptr);
        }
    }
    else if ((rson && !lson) || (!rson && lson)){
        try{
            delete toRemove;
        } catch (std::bad_alloc& e){
            return StatusType::ALLOCATION_ERROR;
        }
        node<T, S> * replacement = lson? lson: rson;
        if (toRemove == root){
            root = replacement;
            replacement->setParent(nullptr);
        }else{
            bool pside = parent->getKey() > key;
            if (pside){
                parent->setLeft(replacement);
            }else{
                parent->setRight(replacement);
            }
            replacement->setParent(parent);
        }
    }
    else{
        bool first = true;
        node<T, S> * curSmall = rson;
        while (curSmall->getLeft()){
            first = false;
            curSmall = curSmall->getLeft();
        }
        T * tempKey = & toRemove->getKey();
        S * tempValue = & toRemove->getValue();
    }



    try{
        delete toRemove;
    } catch (std::bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }
    node<T, S> * startfix;
    // if node has only one son connect him to parent
    if(!lson && rson){
        if (pside){
            parent->setLeft(rson);
        } else{
            parent->setRight(rson);
        }
        rson->setParent(parent);
        startfix = parent;
    } else if(lson && !rson){
        if (pside){
            parent->setLeft(lson);
        } else{
            parent->setRight(lson);
        }
        lson->setParent(parent);
        startfix = parent;
    }
        // if node has two sons replace it with smallest node in right subtree
    else if(lson && rson){
        std::cout << "both exist" << std::endl;
        //find smallest
        bool first = true;
        node<T, S> * curSmall = rson;
        while (curSmall->getLeft()){
            first = false;
            curSmall = curSmall->getLeft();
        }
        // detach smallest from current position and connect his parent to his right son
        if(!first){
            curSmall->getParent()->setLeft(curSmall->getRight());
            if (curSmall->getRight()) curSmall->getRight()->setParent(curSmall->getParent());
        }
        startfix = first? curSmall: curSmall->getParent();
        // connect smallest to toRemove parent
        curSmall->setParent(parent);
        if(pside){
            parent->setLeft(curSmall);
        } else{
            parent->setRight(curSmall);
        }
        // connect smallest to toRemove sons
        curSmall->setLeft(lson);
        curSmall->getLeft()->setParent(curSmall);
        if (!first) curSmall->setRight(rson);
        if (!first) curSmall->getRight()->setParent(curSmall);
    } else{
        std::cout << "none exist" << std::endl;
        if (pside){
            parent->setLeft(nullptr);
        }else{
            parent->setRight(nullptr);
        }
        startfix = parent;
    }
    fixTree(startfix);
    size--;
    maxKey = findMax();
    minKey = findMin();
    return StatusType::SUCCESS;
}

template<typename T, typename S>
void avl<T, S>::changeUnique() {
    unique = !unique;
}

template<typename T, typename S>
avl<T, S>::~avl() {
    destructorAux(root);
}

template<typename T, typename S>
void avl<T, S>::destructorAux(node<T, S> *cur) {
    if (!cur) return;
    destructorAux(cur->getLeft());
    destructorAux(cur->getRight());
    delete cur;
}

template<typename T, typename S>
T avl<T, S>::findMax() {
    node<T, S> *cur = root;
    while (cur->getRight()){
        cur = cur->getRight();
    }
    return cur->getKey();
}

template<typename T, typename S>
T avl<T, S>::findMin() {
    node<T, S> *cur = root;
    while (cur->getLeft()){
        cur = cur->getLeft();
    }
    return cur->getKey();
}

template<typename T, typename S>
T avl<T, S>::getMax() {
    return maxKey;
}

template<typename T, typename S>
T avl<T, S>::getMin() {
    return minKey;
}


#endif //DS_WET_1_AVL_H
