//
// Created by amir on 19/02/2024.
//
#include "avl.h"
#include <cassert>
int main(){
    avl<int, int> intintTree;
    int x = 5;
    assert(intintTree.find(x) == nullptr);
    intintTree.insert(x, x);
    assert(*intintTree.find(x) == x);
    for (int i = 3; i < 8; i++){
        if (i == 5){
            assert(intintTree.insert(i, i) == StatusType::FAILURE);
            continue;
        }
        assert(intintTree.insert(i, i) == StatusType::SUCCESS);
    }
    for (int i = 3; i < 8; i++){
        assert(*intintTree.find(i) == i);
    }
}
