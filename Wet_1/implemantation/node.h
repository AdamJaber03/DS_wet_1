
#ifndef DS_WET_1_NODE_H
#define DS_WET_1_NODE_H

template <typename T, typename S>
class node{
public:
    virtual ~node() = default;
    node(S & key, T & value) : key(key), value(value), height(0) {};
    void setParent(node * parent);
    void setRight(node * right);
    void setLeft(node * left);
    const S & getKey();
    const node * getLeft();
    const node * getRight();
    node * getParent();
    int getHeight();
    void updateHeight();
    int getBf();

private:
    S & key;
    T & value;
    int height;
    node * left;
    node * right;
    node * parent;
};


#endif //DS_WET_1_NODE_H
