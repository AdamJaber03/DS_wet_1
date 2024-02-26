#include "Olympicsa1.h"



Olympics::Olympics(){
    try{
        countries = new avl<int, Country*>;
    }
    catch (std::bad_alloc &e){
        throw StatusType::ALLOCATION_ERROR;
    }
    try{
        teams = new avl<int, Team*>;
    }
    catch (std::bad_alloc &e){
        delete countries;
        throw StatusType::ALLOCATION_ERROR;
    }
    try{
        contestants = new avl<int, Contestant*>;
    }
    catch (std::bad_alloc &e){
        delete countries;
        delete teams;
        throw StatusType::ALLOCATION_ERROR;
    }

}

Olympics::~Olympics(){
    deleteAux<Contestant>(contestants->getRoot());
    deleteAux<Team>(teams->getRoot());
    deleteAux<Country>(countries->getRoot());
    delete contestants;
    delete teams;
    delete countries;
}
	
StatusType Olympics::add_country(int countryId, int medals){
    if (countryId <= 0 || medals < 0){
        return StatusType::INVALID_INPUT;
    }
    Country *country;
    try{
        country = new Country(countryId, medals);
    }catch (std::bad_alloc& err){
        return StatusType::ALLOCATION_ERROR;
    }
    StatusType status = countries->insert(countryId, country);
    if(status != StatusType::SUCCESS) delete country;
    return status;
}
	
StatusType Olympics::remove_country(int countryId){
	if(countryId <= 0){
        return StatusType::INVALID_INPUT;
    }
    Country ** toRemove_p = countries->find(countryId);
    if (!toRemove_p) return StatusType::FAILURE;
    Country* toRemove = *toRemove_p;
    if(toRemove->getTeams() != 0 || toRemove->getContestants() != 0){
        return StatusType::FAILURE;
    }
    delete toRemove;
    return countries->remove(countryId);
}

StatusType Olympics::add_team(int teamId,int countryId,Sport sport){
    if(countryId <= 0 || teamId <= 0) return StatusType::INVALID_INPUT;
    Country ** country_pp = countries->find(countryId); //problem
    if(!country_pp) return StatusType::FAILURE;
    Country* country_p = *country_pp;
    Team* toAdd;
    try {
        toAdd = new Team(teamId, country_p, sport);
    } catch (std::bad_alloc& err){
        return StatusType::ALLOCATION_ERROR;
    }
    StatusType status = teams->insert(teamId, toAdd);
    if(status != StatusType::SUCCESS) delete toAdd;
    return status;
}

StatusType Olympics::remove_team(int teamId){
	if(teamId <= 0) return StatusType::INVALID_INPUT;
    Team ** teamToRemove_p = teams->find(teamId);
    if(!teamToRemove_p) return StatusType::FAILURE;
    Team * teamToRemove = *teamToRemove_p;
    if(teamToRemove->getNumContestants() > 0) return StatusType::FAILURE;
    Country * country = teamToRemove->getCountry();
    StatusType status = teams->remove(teamId);
    if (status != StatusType::SUCCESS) return status;
    delete teamToRemove;
    country->removeTeam();
    return status;
}
	
StatusType Olympics::add_contestant(int contestantId ,int countryId,Sport sport,int strength){
	if(contestantId <= 0 || countryId <= 0 ||strength < 0) return StatusType::INVALID_INPUT;
    Country ** country_p = countries->find(countryId);
    if(!country_p) return StatusType::FAILURE;
    Country *country = *country_p;
    Contestant* addContestant;
    try{
        addContestant = new Contestant(contestantId, countryId, sport, strength, country);
    }catch (std::bad_alloc& err){
        return StatusType::ALLOCATION_ERROR;
    }
    StatusType status = contestants->insert(contestantId, addContestant);
    if (status != StatusType::SUCCESS) delete addContestant;
    return status;
}
	
StatusType Olympics::remove_contestant(int contestantId){
    if(contestantId <= 0) return StatusType::INVALID_INPUT;
    Contestant ** toRemove_p = contestants->find(contestantId);
    if(!toRemove_p) return StatusType::FAILURE;
    Contestant * toRemove = *toRemove_p;
    if(toRemove->getNumTeams() > 0) return StatusType::FAILURE;
    StatusType status = contestants->remove(contestantId);
    if (status != StatusType::SUCCESS) return status;
    delete toRemove;
    return StatusType::SUCCESS;
}

StatusType Olympics::add_contestant_to_team(int teamId,int contestantId){
    if(teamId <= 0 || contestantId <= 0) return StatusType::INVALID_INPUT;
    Team ** team_p = teams->find(teamId);
    Contestant ** contestant_p = contestants->find(contestantId);
    if(!contestant_p || !team_p) return StatusType::FAILURE;
    Team *team = *team_p;
    Contestant *contestant = *contestant_p;
    if(team->getSport() != contestant->getSport() || team->getCountryId() != contestant->getCountryId() || contestant->getNumTeams() >= 3) return StatusType::FAILURE;
    int strength = contestant->getStrength();
    StatusType status = team->addContestant(contestantId, strength, contestant);
    if (status != StatusType::SUCCESS) return status;
    contestant->addTeam(teamId);
    return StatusType::SUCCESS;
}

StatusType Olympics::remove_contestant_from_team(int teamId,int contestantId){
    if(teamId <= 0 || contestantId <= 0) return StatusType::INVALID_INPUT;
    Team ** team_p = teams->find(teamId);
    Contestant ** contestant_p = contestants->find(contestantId);
    if(!contestant_p || !team_p) return StatusType::FAILURE;
    Team *team = *team_p;
    Contestant *contestant = *contestant_p;
    StatusType status = team->removeContestant(contestantId);
    if (status != StatusType::SUCCESS) return status;
    contestant->removeTeam(teamId);
    return StatusType::SUCCESS;
}

StatusType Olympics::update_contestant_strength(int contestantId ,int change){
    if(contestantId <= 0) return StatusType::INVALID_INPUT;
    Contestant ** contestant_p = contestants->find(contestantId);
    if(!contestant_p) return StatusType::FAILURE;
    Contestant *contestant = *contestant_p;
    int updateStr = contestant->getStrength() + change;
    if(updateStr < 0) return StatusType::FAILURE;
    StatusType status;
    int teamIDs[3];
    for (int i = 0; i < 3; ++i) {
        teamIDs[i] = contestant->getTeamIds()[i];
    }
    for (int i = 0; i < 3; ++i) {
        remove_contestant_from_team(teamIDs[i], contestantId);
    }
    contestant->setStrength(updateStr);
    for (int i = 0; i < 3; ++i) {
        status = add_contestant_to_team(teamIDs[i], contestantId);
        if (status == StatusType::ALLOCATION_ERROR) return status;
    }
	return StatusType::SUCCESS;
}

output_t<int> Olympics::get_strength(int contestantId){
	if(contestantId <= 0) return output_t<int>(StatusType::INVALID_INPUT);
    Contestant ** contestant_p = contestants->find(contestantId);
    if(!contestant_p) return output_t<int>(StatusType::FAILURE);
    Contestant *contestant = *contestant_p;
    return output_t<int>(contestant->getStrength());
}

output_t<int> Olympics::get_medals(int countryId){
	if(countryId <= 0) return output_t<int>(StatusType::INVALID_INPUT);
    Country ** country_p = countries->find(countryId);
    if(!country_p) return output_t<int>(StatusType::FAILURE);
    Country *country = *country_p;
    return output_t<int>(country->getMedals());
}

output_t<int> Olympics::get_team_strength(int teamId){
    if (teamId <= 0) return output_t<int>(StatusType::INVALID_INPUT);
    Team **team_p = teams->find(teamId);
    if(!team_p) return output_t<int>(StatusType::FAILURE);
    Team* team = *team_p;
    return output_t<int>(team->getStrength());
}

StatusType Olympics::unite_teams(int teamId1,int teamId2){
    if (teamId1 <=0 || teamId2 <=0 || teamId1==teamId2) return StatusType::INVALID_INPUT;
    Team** team1_p = teams->find(teamId1);
    Team** team2_p = teams->find(teamId2);
    if(!team1_p || !team2_p) return StatusType::FAILURE;
    Team *team1 = *team1_p;
    Team *team2 = *team2_p;
    if (team1->getSport() != team2->getSport() || team1->getCountryId() != team2->getCountryId()) return StatusType::FAILURE;
    Team &team2_ = *team2;
    StatusType status = team1->unite(team2_);
    if (status != StatusType::SUCCESS) return status;
    return remove_team(teamId2);
}


StatusType Olympics::play_match(int teamId1,int teamId2){
	if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) return StatusType::INVALID_INPUT;
    Team** team1_p = teams->find(teamId1);
    Team** team2_p = teams->find(teamId2);
    if(!team1_p || !team2_p) return StatusType::FAILURE;
    Team *team1 = *team1_p;
    Team *team2 = *team2_p;
    if(team1->getSport() != team2->getSport()) return StatusType::FAILURE;
    Country* country1 = team1->getCountry(), *country2 = team2->getCountry();
    int score1 = team1->getStrength() + country1->getMedals();
    int score2 = team2->getStrength() + country2->getMedals();
    if (score1 == score2) return StatusType::SUCCESS;
    if (score1 > score2){
        country1->addMedal();
    }else{
        country2->addMedal();
    }
    return StatusType::SUCCESS;
}

output_t<int> Olympics::austerity_measures(int teamId){
    if(teamId <= 0) return output_t<int>(StatusType::INVALID_INPUT);
    Team **team_p = teams->find(teamId);
    if(!team_p) return output_t<int>(StatusType::FAILURE);
    Team *team = *team_p;
    if (team->getNumContestants() < 3) return output_t<int>(StatusType::FAILURE);
    int austerity = 0;
    try{
        austerity = team->getAusterity();
    }
    catch(std::bad_alloc &e){
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }
    return output_t<int>(austerity);
}

template<class T>
void Olympics::deleteAux(node<int, T *> *cur) {
    if (!cur) return;
    deleteAux(cur->getLeft());
    deleteAux(cur->getRight());
    delete cur->getValue();
}



