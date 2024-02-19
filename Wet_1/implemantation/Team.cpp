//
// Created by amir on 19/02/2024.
//
#include "Team.h"
int Team::getStrength() {
    return st1.getMax() + st2.getMax() + st3.getMax();
}


