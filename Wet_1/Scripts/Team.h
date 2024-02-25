
#ifndef DS_WET_1_TEAM_H
#define DS_WET_1_TEAM_H
#include "country.h"
#include "Contestant.h"
#include "wet1util.h"
#include "pair.h"
#include "avl.h"

class Team{
public:
    Team()=default;
    Team(int teamId, Country * country, Sport sport);
    ~Team();
    bool operator<(const Team &toCompare) const;
    Sport getSport();
    int getID();
    int getNumContestants();
    StatusType addContestant(int id, int strength, Contestant* contestant, bool fromUpdate=false);
    StatusType removeContestant(int id, bool fromUpdate=false);
    int getStrength();
    int getCountryId();
    Country* getCountry();
    StatusType unite(Team &team2);
    int getAusterity();
    void setContestants(int update);

private:
    int teamId;
    Country *country;
    Sport sport;
    int numContestants;
    int austerity;
    avl<int, Contestant*>* s1, *s2, *s3;
    avl<pair<int, int>, int> *st1, *st2, *st3;    //strength keys id values
    StatusType balanceTeam();
    void updateAusterity();
    int calculateAusterity(int toRemoveS1, int toRemoveS2, int toRemoveS3);
    void updateTeamIds(avl<int, Contestant*>* tree, int oldID);
    void updateTeamIdsAux(int oldID, node<int, Contestant*>* cur);
};
#endif //DS_WET_1_TEAM_H
