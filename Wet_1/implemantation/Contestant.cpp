//
// Created by amir on 19/02/2024.
//
#include "Contestant.h"

Contestant::~Contestant() {
    delete country;
}

int Contestant::getID() {
    return contestantID;
}

Sport Contestant::getSport() {
    return sport;
}

int Contestant::getStrength() {
    return strength;
}

int Contestant::getNumTeams() {
    return numTeams;
}

void Contestant::setStrength(int updateStrength) {
    strength = updateStrength;
}

void Contestant::addTeam() {
    numTeams++;
}

int *Contestant::getCountry() {
    return country;
}
