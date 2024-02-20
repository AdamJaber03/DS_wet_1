//
// Created by amir on 19/02/2024.
//
#include "Team.h"
int Team::getStrength() {
    return st1.getMax() + st2.getMax() + st3.getMax();
}


int *Team::getCountry() {
    return country;
}

avl<int, Contestant *> &Team::getS1() {
    return s1;
}

avl<int, Contestant *> &Team::getS2() {
    return s2;
}

avl<int, Contestant *> &Team::getS3() {
    return s3;
}

avl<int, int> &Team::getSt1() {
    return st1;
}

avl<int, int> &Team::getSt2() {
    return st2;
}

avl<int, int> &Team::getSt3() {
    return st3;
}
