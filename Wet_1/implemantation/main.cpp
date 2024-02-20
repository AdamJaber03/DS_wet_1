//
// Created by amir on 19/02/2024.
//
#include "avl.h"
#include <iostream>
int main(){
    std::cout << "start test" << std::endl;
    avl<int, int> intintTree;
    std::cout << "init tree" << std::endl;
    int x = 5;
    if (intintTree.find(x) == nullptr){
        std::cout << "test empty find success" << std::endl;
    } else{
        std::cout << "test empty find failed" << std::endl;
        return 1;
    }
    intintTree.insert(x, x);
    if (*intintTree.find(x) == x){
        std::cout << "test find success" << std::endl;
    } else{
        std::cout << "test find failed" << std::endl;
        return 1;
    }
    for (int i = 1; i < 21; i++){
        if (i == 5){
            if(intintTree.insert(i, i) == StatusType::FAILURE){
                std::cout << "planned fail success" << std::endl;
            }else{
                std::cout << "planned fail failed" << std::endl;
                return 1;
            }
            continue;
        }
        if(intintTree.insert(i, i) == StatusType::SUCCESS){
            std::cout << "insert success" << std::endl;
        } else{
            std::cout << "insert fail" << std::endl;
            return 1;

        }
    }
    for (int i = 1; i < 21; i++){
        if(intintTree.find(i) && *intintTree.find(i) == i){
            std::cout << "find test success" << std::endl;
        } else{
            std::cout << "find test failed" << std::endl;
            return 1;
        }
    }
    int toRemove = 20;
    if( intintTree.remove(toRemove) == StatusType::SUCCESS){
        std::cout << "success remove from mid" << std::endl;
    } else{
        std::cout << "failed remove from mid" << std::endl;
    }
    toRemove = 18;
    if( intintTree.remove(toRemove) == StatusType::SUCCESS){
        std::cout << "success remove leaf and rotate" << std::endl;
    } else{
        std::cout << "failed remove leaf and rotate" << std::endl;
    }
    toRemove = 15;
    if( intintTree.remove(toRemove) == StatusType::SUCCESS){
        std::cout << "success remove leaf and rotate" << std::endl;
    } else{
        std::cout << "failed remove leaf and rotate" << std::endl;
    }
    for (int i = 1; i < 21; i++){
        intintTree.remove(i);
    }

    std::cout << "success" << std::endl;
}
