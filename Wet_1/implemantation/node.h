
#ifndef DS_WET_1_NODE_H
#define DS_WET_1_NODE_H

template <typename T, typename S>
class node{
public:
    node();
    virtual ~node();
    node(S & key, T & value) : key(key), value(value), height(0) {};
    void setParent(node * parent);
    void setRight(node * right);
    void setLeft(node * left);
private:
    S & key;
    T & value;
    int height;
    node * left;
    node * right;
    node * parent;
    void updateHeight();
};

#endif //DS_WET_1_NODE_H
