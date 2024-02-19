#include "avl.h"

template<typename T, typename S>
avl<T, S>::avl(): size(0), root(nullptr) {}

template<typename T, typename S>
StatusType avl<T, S>::insert(T &key, S &value) {
    node<T, S> * newNode = new node<T, S>(key, value);
    if (newNode == nullptr){    //Todo - check if legal
        return StatusType::ALLOCATION_ERROR;
    }
    if (!root){
        root = newNode;
        return StatusType::SUCCESS;
    }
    if(unique && newNode->getKey() == find(key)){
        return StatusType::FAILURE;
    }
    node<T, S> * parent = findAux(key);
    if (key < parent->getKey()){
        parent->setLeft(newNode);
    } else {
        parent->setRight(newNode);
    }
    while(parent){
        parent->updateHeight();
        parent = parent->getParent();
    }
    fixTree(newNode);
    size++;
}

template<typename T, typename S>
node<T, S> *avl<T, S>::findAux(T &key) {
    node<T, S> * cur = root, parent = nullptr;
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
    node<T, S> * parent = findAux(key);
    if (parent->getLeft()->getKey() == key){
        return &parent->getLeft()->getValue();
    } else if(parent->getRight()->getKey() == key){
        return &parent->getRight()->getValue();
    }
    return nullptr;
}

template<typename T, typename S>
void avl<T, S>::rotateLL(node<T, S> * toFix) {
    node<T, S> * lSon = toFix->getLeft();
    toFix->setLeft(lSon->getRight());
    lSon->setRight(toFix);
    lSon->setParent(toFix->getParent());
    toFix->setParent(lSon);
    toFix->updateHeight();
    lSon->updateHeight();
}

template<typename T, typename S>
void avl<T, S>::rotateRR(node<T, S> * toFix) {
    node<T, S> * rSon = toFix->getRight();
    toFix->setRight(rSon->getLeft());
    rSon->setLeft(toFix);
    rSon->setParent(toFix->getParent());
    toFix->setParent(rSon);
    toFix->updateHeight();
    rSon->updateHeight();
}

template<typename T, typename S>
void avl<T, S>::rotateLR(node<T, S> * toFix) {
    node<T, S> * lSon = toFix->getLeft();
    node<T, S> * lrSon = lSon->getRight();
    lSon->setRight(lrSon->getLeft());
    toFix->setLeft(lrSon->getRight());
    lrSon->setLeft(lSon);
    lrSon->setRight(toFix);
    lSon->getRight()->setParent(lSon);
    toFix->getLeft()->setParent(toFix);
    lrSon->setParent(toFix->getParent());
    toFix->setParent(lrSon);
    lSon->setParent(lrSon);
    lSon->updateHeight();
    toFix->updateHeight();
    lrSon->updateHeight();
}

template<typename T, typename S>
void avl<T, S>::rotateRL(node<T, S> * toFix) {
    node<T, S> * rSon = toFix->getRight();
    node<T, S> * rlSon = rSon->getLeft();
    rSon->setLeft(rlSon->getRight());
    toFix->setRight(rlSon->getLeft());
    rlSon->setRight(rSon);
    rlSon->setLeft(toFix);
    rSon->getLeft()->setParent(rSon);
    toFix->getRight()->setParent(toFix);
    rlSon->setParent(toFix->getParent());
    toFix->setParent(rlSon);
    rSon->setParent(rlSon);
    rSon->updateHeight();
    toFix->updateHeight();
    rlSon->updateHeight();
}

template<typename T, typename S>
void avl<T, S>::fixTree(node<T, S> *start) {
    node<T, S> * cur = start;
    while (cur){
        cur->updateHeight();
        int bf = cur->getBf();
        if (abs(bf) < 2){
            cur = cur->getParent();
            continue;
        }
        if (bf == 2){
            if (cur->getLeft()->getBf() == 1){
                rotateLL(cur);
            } else{
                rotateLR(cur);
            }
        } else {
            if (cur->getLeft()->getBf() == -1) {
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
    avl<T, S> backupTree = avl<T, S>(*this);
    node<T, S> * toRemove = find(key);
    if (!toRemove){
        return StatusType::FAILURE;
    }
    bool pside = toRemove->getParent()->getKey() > key;
    node<T, S> * parent = toRemove->getParent(), lson = toRemove->getLeft(), rson = toRemove->getRight();
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
        //find smallest
        bool first = true;
        node<T, S> * curSmall = rson;
        while (curSmall->getLeft()){
            first = false;
            curSmall = curSmall->getLeft();
        }
        // detach smallest from current position and connect his parent to his right son
        if(first){
            curSmall->getParent()->setRight(curSmall->getRight());
        } else{
            curSmall->getParent()->setLeft(curSmall->getRight());
        }
        startfix = curSmall->getParent();
        if (curSmall->getRight()) {
            curSmall->getRight()->setParent(curSmall->getParent());
        }
        // connect smallest to toRemove parent
        curSmall->setParent(parent);
        if(pside){
            parent->setLeft(curSmall);
        } else{
            parent->setRight(curSmall);
        }
        // connect smallest to toRemove sons
        curSmall->setLeft(lson);
        curSmall->setRight(rson);
        curSmall->getLeft()->setParent(curSmall);
        curSmall->getRight()->setParent(curSmall);
    } else{
        if (pside){
            parent->setLeft(nullptr);
        }else{
            parent->setRight(nullptr);
        }
        startfix = parent;
    }
    fixTree(startfix);
    size--;
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