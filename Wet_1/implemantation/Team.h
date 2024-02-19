
#ifndef DS_WET_1_TEAM_H
#define DS_WET_1_TEAM_H
#include "country.h"
#include "Contestant.h"
#include "wet1util.h"
#include "avl.h"

class Team{
public:
    Team();
    Team(int teamId, int * country, Sport sport): teamId(teamId), country(country), sport(sport), numContestants(0) {};
    ~Team() = default;
    Sport getSport();
    int getID();
    int getNumContestants();
    void addContestant(int contestant_id);
    int getStrength();
    int get_austerity();
    int * getCountry();

private:
    int teamId;
    int *country;
    Sport sport;
    int numContestants;
    avl<int, Contestant*> s1, s2, s3;
    avl<int, int> st1, st2, st3;

};


#endif //DS_WET_1_TEAM_H
