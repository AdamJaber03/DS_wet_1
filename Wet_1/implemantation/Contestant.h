
#ifndef DS_WET_1_CONTESTANT_H
#define DS_WET_1_CONTESTANT_H

#include "country.h"
#include "../Scripts/wet1util.h"
#include "avl.h"
#include "Team.h"

class Contestant{
public:
    Contestant(int contestantID, int countryId, Sport sport, int strength, Country* country);
    ~Contestant();
    bool operator<(const Contestant &toCompare) const;
    int getID();
    Sport getSport();
    int getStrength();
    int getNumTeams();
    void setStrength(int updateStrength);
    void addTeam(int teamId);
    void removeTeam(int teamId);
    int getCountryId();
    int* getTeamIds();

private:
    int contestantID;
    int countryId;
    Sport sport;
    int strength;
    int numTeams;
    int teamIds[3];
    Country* country;

};

#endif //DS_WET_1_CONTESTANT_H
