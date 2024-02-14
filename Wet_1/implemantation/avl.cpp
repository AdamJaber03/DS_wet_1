#include "avl.h"

template<typename T, typename S>
avl<T, S>::avl(): size(0), root(nullptr) {}

template<typename T, typename S>
StatusType avl<T, S>::push(S &key, T &value) {
    node<T, S> * newNode = new node<T, S>(key, value);
    if (newNode == nullptr){
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
T *avl<T, S>::findAux(S &key) {
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
T *avl<T, S>::find(S &key) {
    node<T, S> * parent = findAux(key);
    if (parent->getLeft()->getKey() == key){
        return parent->getLeft();
    } else if(parent->getRight()->getKey() == key){
        return parent->getRight();
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
StatusType avl<T, S>::remove(S &key) {
    avl<T, S> backupTree = avl<T, S>(*this);
    node<T, S> * toRemove = find(key);
    if (!toRemove){
        return StatusType::FAILURE;
    }
    bool pside = toRemove->getParent()->getKey() > key;

    // if node has only one son connect him to parent
    if(!toRemove->getLeft() && toRemove->getRight()){
        if (pside){
            toRemove->getParent()->setLeft(toRemove->getRight());
        } else{
            toRemove->getParent()->setRight(toRemove->getRight());
        }
        toRemove->getRight()->setParent(toRemove->getParent());
    } else if(toRemove->getLeft() && !toRemove->getRight()){
        if (pside){
            toRemove->getParent()->setLeft(toRemove->getLeft());
        } else{
            toRemove->getParent()->setRight(toRemove->getLeft());
        }
        toRemove->getLeft()->setParent(toRemove->getParent());
    }
    // if node has two sons replace it with smallest node in right subtree
    else if(toRemove->getLeft() && toRemove->getRight()){
        //find smallest
        bool first = true;
        node<T, S> * curSmall = toRemove->getRight();
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
        if (curSmall->getRight()) {
            curSmall->getRight()->setParent(curSmall->getParent());
        }
        // connect smallest to toRemove parent
        curSmall->setParent(toRemove->getParent());
        if(pside){
            toRemove->getParent()->setLeft(curSmall);
        } else{
            toRemove->getParent()->setRight(curSmall);
        }
        // connect smallest to toRemove sons
        curSmall->setLeft(toRemove->setLeft());
        curSmall->setRight(toRemove->setRight());
        curSmall->getLeft()->setParent(curSmall);
        curSmall->getRight()->setParent(curSmall);
    } else{
        if (pside){
            toRemove->getParent()->setLeft(nullptr);
        }else{
            toRemove->getParent()->setRight(nullptr);
        }
    }
    node<T, S> * startFix = toRemove->getParent();
    try{
        delete toRemove;
    } catch (std::bad_alloc& e){
        *this = backupTree;
        return StatusType::ALLOCATION_ERROR;
    }
    fixTree(startFix);
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

template<typename T, typename S>
avl<T, S>::avl(avl &toCopy): size(toCopy.size), unique(toCopy.unique){
    if (!toCopy.root) return;
    // TODO- deal with allocation error
    root = new node<T, S>(toCopy.root->getKey(), toCopy.root.getValue);
    copyAux(root, toCopy.root);
}

template<typename T, typename S>
void avl<T, S>::copyAux(node<T, S> *cur, node<T, S> *curCopy) {

}