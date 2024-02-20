
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
    avl<int, Contestant*> & getS1();
    avl<int, Contestant*> & getS2();
    avl<int, Contestant*> & getS3();
    avl<int, int> & getSt1();
    avl<int, int> & getSt2();
    avl<int, int> & getSt3();

private:
    int teamId;
    int *country;
    Sport sport;
    int numContestants;
    avl<int, int> s1, s2, s3;
    avl<int, int> st1, st2, st3;    //strength keys id values

};


#endif //DS_WET_1_TEAM_H
