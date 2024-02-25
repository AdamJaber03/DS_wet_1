//
// Created by amir on 19/02/2024.
//
#include "Contestant.h"

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
    strength = updateStrength;      //TODO - not sure about this i think its strength = strength - update
}

void Contestant::addTeam(int teamId) {
    numTeams++;
    for (int i = 0; i < 3; i++){
        if (!teamIds[i]){
            teamIds[i] = teamId;
            break;
        }
    }
}

void Contestant::removeTeam(int teamId) {
    numTeams--;
    for (int i = 0; i < 3; ++i) {
        if (teamIds[i] == teamId){
            teamIds[i] =0;
            break;
        }
    }
}

int Contestant::getCountryId() {
    return countryId;
}

int* Contestant::getTeamIds() {
    return teamIds;
}

Contestant::Contestant(int contestantID, int countryId, Sport sport, int strength, Country *country): contestantID(contestantID), countryId(countryId), sport(sport),
                                                                                                      strength(strength), numTeams(0), teamIds{0, 0, 0}, country(country){
    country->addContestant();

}

Contestant::~Contestant() {
    country->removeContestant();
}

bool Contestant::operator<(const Contestant &toCompare) const {
    return true;
}
