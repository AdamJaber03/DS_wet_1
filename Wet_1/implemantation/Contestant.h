
#ifndef DS_WET_1_CONTESTANT_H
#define DS_WET_1_CONTESTANT_H

#include "country.h"
#include "wet1util.h"
#include "avl.h"
#include "Team.h"

class Contestant{
public:
    Contestant(int contestantID, int * country, Sport sport, int strength) : contestantID(contestantID), country(country), sport(sport),
                strength(strength), numTeams(0) {};
    ~Contestant();
    int getID();
    Sport getSport();
    int getStrength();
    int getNumTeams();
    void setStrength(int updateStrength);
    void addTeam();
private:
    int contestantID;
    int * country;
    Sport sport;
    int strength;
    int numTeams;

};

#endif //DS_WET_1_CONTESTANT_H
