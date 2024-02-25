#include "country.h"

int Country::getId() const {
    return id;
}

int Country::getMedals() const {
    return medals;
}

void Country::addMedal() {
    medals++;
}

int Country::getTeams() const {
    return teams;
}

int Country::getContestants() const {
    return contestants;
}

void Country::removeTeam() {
    teams--;
}

void Country::addTeam() {
    teams++;
}

void Country::addContestant() {
    contestants++;
}

void Country::removeContestant() {
    contestants--;
}

bool Country::operator<(const Country &toCompare) const {
    return true;
}
