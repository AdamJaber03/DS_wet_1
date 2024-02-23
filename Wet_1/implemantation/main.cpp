//
// Created by amir on 19/02/2024.
//
#include "avl.h"
#include <iostream>
#include "Team.h"
#include "country.h"
#include <random>

int getRandomNumber(int min, int max) {
    static std::random_device rd;
    static std::mt19937 eng(rd());
    std::uniform_int_distribution<int> distr(min, max);
    return distr(eng);
}

int main(){
//    std::cout << "start test" << std::endl;
//    avl<int, int> intintTree;
//    std::cout << "init tree" << std::endl;
//    int x = 5;
//    if (intintTree.find(x) == nullptr){
//        std::cout << "test empty find success" << std::endl;
//    } else{
//        std::cout << "test empty find failed" << std::endl;
//        return 1;
//    }
//    intintTree.insert(x, x);
//    if (*intintTree.find(x) == x){
//        std::cout << "test find success" << std::endl;
//    } else{
//        std::cout << "test find failed" << std::endl;
//        return 1;
//    }
//    for (int i = 1; i < 21; i++){
//        if (i == 5){
//            if(intintTree.insert(i, i) == StatusType::FAILURE){
//                std::cout << "planned fail success" << std::endl;
//            }else{
//                std::cout << "planned fail failed" << std::endl;
//                return 1;
//            }
//            continue;
//        }
//        if(intintTree.insert(i, i) == StatusType::SUCCESS){
//            std::cout << "insert success" << std::endl;
//        } else{
//            std::cout << "insert fail" << std::endl;
//            return 1;
//
//        }
//    }
//    for (int i = 1; i < 21; i++){
//        if(intintTree.find(i) && *intintTree.find(i) == i){
//            std::cout << "find test success" << std::endl;
//        } else{
//            std::cout << "find test failed" << std::endl;
//            return 1;
//        }
//    }
//    int toRemove = 20;
//    if( intintTree.remove(toRemove) == StatusType::SUCCESS){
//        std::cout << "success remove from mid" << std::endl;
//    } else{
//        std::cout << "failed remove from mid" << std::endl;
//    }
//    toRemove = 18;
//    if( intintTree.remove(toRemove) == StatusType::SUCCESS){
//        std::cout << "success remove leaf and rotate" << std::endl;
//    } else{
//        std::cout << "failed remove leaf and rotate" << std::endl;
//    }
//    toRemove = 15;
//    if( intintTree.remove(toRemove) == StatusType::SUCCESS){
//        std::cout << "success remove leaf and rotate" << std::endl;
//    } else{
//        std::cout << "failed remove leaf and rotate" << std::endl;
//    }
//    for (int i = 1; i < 21; i++){
//        intintTree.remove(i);
//    }
//
//    std::cout << "success" << std::endl;
    Country country = Country(1, 2);
    Team team1 = Team(2, &country, Sport::ACROBATICS);
    Team team2 = Team(3, &country, Sport::ACROBATICS);
    team1.addContestant(2, 5);
    team1.addContestant(3, 7);
    team1.addContestant(4, 5);
    int st = team1.getStrength();
    std::cout << "strength basic test " << ((st==17)? "success": "failed") << std::endl;
    team1.removeContestant(4);
    std::cout << "strength 0 basic test " << ((team1.getStrength()==0)? "success": "failed") << std::endl;
    team1.removeContestant(2);
    team1.removeContestant(3);
    std::cout << "strength empty basic test " << ((team1.getStrength()==0)? "success": "failed") << std::endl;
    for (int i = 1; i < 16; ++i) {
        team1.addContestant(i, (16 - i));
    }
    //create team with 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 with strengths 15,14,13,12,11,10,9,8,7,6,5,4,3,2,1 strength should be 15+10+5=30
    std::cout << "strength complex test " << ((team1.getStrength()==30)? "success": "failed") << std::endl;
    for (int i = 10; i < 22; ++i) {
        if (i < 16){
            team2.addContestant(i, (16 - i));
        }else{
            team2.addContestant(i, (30 - i));
        }
    }
    //team2 is 10,11,12,13,14,15,16,17,18,19,20,21 with strengths 6,5,4,3,2,1,14,13,12,11,10,9 teamStrength should be 6+14+12=32
    std::cout << "strength complex test 2 " << ((team2.getStrength()==32)? "success": "failed") << std::endl;

    team1.unite(team2);
    //new team1 is 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21 with strengths 15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,14,13,12,11,10,9 teamStrength should be 15+8+14=37
    std::cout << "strength unite test " << ((team1.getStrength()==37)? "success": "failed") << std::endl;
}
