#include "Olympicsa1.h"



Olympics::Olympics(){

}

Olympics::~Olympics(){
    delete countries;
    delete contestants;
    delete teams;
}
	
StatusType Olympics::add_country(int countryId, int medals){
    if (countryId <= 0 || medals < 0){
        return StatusType::INVALID_INPUT;
    }
    Country country = Country(countryId, medals);
    return countries->insert(countryId, country);
}
	
StatusType Olympics::remove_country(int countryId){
	if(countryId <= 0){
        return StatusType::INVALID_INPUT;
    }
    Country * toRemove = countries->find(countryId);
    if(toRemove->getTeams() != 0 || toRemove->getContestants() != 0){
        return StatusType::FAILURE;
    }
    return countries->remove(countryId);
}

StatusType Olympics::add_team(int teamId,int countryId,Sport sport){
    if(countryId <= 0 || teamId <= 0) return StatusType::INVALID_INPUT;
    Country * country_p = countries->find(countryId);
    Team toAdd = Team(teamId, country_p, sport);
    return teams->insert(teamId, toAdd);
}

StatusType Olympics::remove_team(int teamId){
	if(teamId <= 0) return StatusType::INVALID_INPUT;
    Team * teamToRemove = teams->find(teamId);
    if(!teamToRemove || teamToRemove->getNumContestants() > 0) return StatusType::FAILURE;
    return teams->remove(teamId);
}
	
StatusType Olympics::add_contestant(int contestantId ,int countryId,Sport sport,int strength){
	if(contestantId <= 0 || countryId <= 0 ||strength < 0) return StatusType::INVALID_INPUT;
    Country * country = countries->find(countryId);
    if(!country) return StatusType::FAILURE;
    Contestant addContestant = Contestant(contestantId, countryId, sport, strength, country);
    return contestants->insert(countryId, addContestant);
}
	
StatusType Olympics::remove_contestant(int contestantId){
    if(contestantId <= 0) return StatusType::INVALID_INPUT;
    Contestant * toRemove = contestants->find(contestantId);
    if(!toRemove || toRemove->getNumTeams() > 0) return StatusType::FAILURE;    //why??????
    return contestants->remove(contestantId);
}

StatusType Olympics::add_contestant_to_team(int teamId,int contestantId){
	if(teamId <= 0 || contestantId <= 0) return StatusType::INVALID_INPUT;
    Team * team = teams->find(teamId);
    Contestant * contestant = contestants->find(contestantId);
    if(!contestant || !team || team->getSport() != contestant->getSport() || team->getCountryId() != contestant->getCountryId() || contestant->getNumTeams() >= 3) return StatusType::FAILURE;
    int strength = contestant->getStrength();
    StatusType status = team->addContestant(contestantId, strength);
    if (status != StatusType::SUCCESS) return status;
    contestant->addTeam(teamId);
    return StatusType::SUCCESS;
}

StatusType Olympics::remove_contestant_from_team(int teamId,int contestantId){
    if(teamId <= 0 || contestantId <= 0) return StatusType::INVALID_INPUT;
    Team * team = teams->find(teamId);
    Contestant * contestant = contestants->find(contestantId);
    if(!contestant || !team) return StatusType::FAILURE;
    StatusType status = team->removeContestant(contestantId);
    if (status != StatusType::SUCCESS) return status;
    contestant->removeTeam(teamId);
    return StatusType::SUCCESS;
}

StatusType Olympics::update_contestant_strength(int contestantId ,int change){
    if(contestantId <= 0) return StatusType::INVALID_INPUT;
    Contestant * contestant = contestants->find(contestantId);
    if(!contestant) return StatusType::FAILURE;
    int updateStr = contestant->getStrength() + change;
    if(updateStr < 0) return StatusType::FAILURE;
    StatusType status;
    for (int i = 0; i < 3; ++i) {
        remove_contestant_from_team(contestant->getTeamIds()[i], contestantId);
    }
    contestant->setStrength(updateStr);
    for (int i = 0; i < 3; ++i) {
        status = add_contestant_to_team(contestant->getTeamIds()[i], contestantId);
        if (status == StatusType::ALLOCATION_ERROR) return status;
    }
	return StatusType::SUCCESS;
}

output_t<int> Olympics::get_strength(int contestantId){
	if(contestantId < 0) return output_t<int>(StatusType::INVALID_INPUT);
    Contestant * contestant = contestants->find(contestantId);
    if(!contestant) return output_t<int>(StatusType::FAILURE);
    return output_t<int>(contestant->getStrength());
}

output_t<int> Olympics::get_medals(int countryId){
	if(countryId <= 0) return output_t<int>(StatusType::INVALID_INPUT);
    Country * country = countries->find(countryId);
    if(!country) return output_t<int>(StatusType::FAILURE);
    return output_t<int>(country->getMedals());
}

output_t<int> Olympics::get_team_strength(int teamId){
    if (teamId <= 0) return output_t<int>(StatusType::INVALID_INPUT);
    Team *team = teams->find(teamId);
    if(!team) return output_t<int>(StatusType::FAILURE);
    return output_t<int>(team->getStrength());
}

StatusType Olympics::unite_teams(int teamId1,int teamId2){
    if (teamId1 <=0 || teamId2 <=0 || teamId1==teamId2) return StatusType::INVALID_INPUT;
    Team* team1 = teams->find(teamId1);
    Team* team2 = teams->find(teamId2);
    if (!team1 || !team2 || team1->getSport() != team2->getSport() || team1->getCountryId() != team2->getCountryId()) return StatusType::FAILURE;
    StatusType status = team1->unite(*team2);
    if (status != StatusType::SUCCESS) return status;
    delete team2;
    return StatusType::SUCCESS;
}



StatusType Olympics::play_match(int teamId1,int teamId2){
	if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) return StatusType::INVALID_INPUT;
    Team * team1 = teams->find(teamId1);
    Team * team2 = teams->find(teamId2);
    if(!team1 || !team2 || team1->getSport() != team2->getSport()) return StatusType::FAILURE;
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
    Team *team = teams->find(teamId);
    if(!team) return output_t<int>(StatusType::FAILURE);
    int austerity = 0;
    try{
        austerity = team->getAusterity();
    }
    catch(std::bad_alloc &e){
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }
    return output_t<int>(austerity);
}



