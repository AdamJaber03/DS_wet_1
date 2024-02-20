#include "Olympicsa1.h"



Olympics::Olympics(){

}

Olympics::~Olympics(){

}
	
StatusType Olympics::add_country(int countryId, int medals){
    Country * country_p = nullptr;
    if (countryId <= 0 || medals < 0){
        return StatusType::INVALID_INPUT;
    }
	try{
        country_p = new Country(countryId, medals);
    }
    catch(std::bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }
    Country country = *country_p;
    return countries.insert(countryId, country);
}
	
StatusType Olympics::remove_country(int countryId){
	if(countryId <= 0){
        return StatusType::INVALID_INPUT;
    }
    Country * toRemove = countries.find(&countryId);
    if(toRemove->getTeams() != 0 && toRemove->getContestants() != 0){
        return StatusType::FAILURE;
    }
    return countries.remove(countryId);
}

StatusType Olympics::add_team(int teamId,int countryId,Sport sport){
	Team * team_p = nullptr;
    if(countryId <= 0 || teamId <= 0){
        return StatusType::INVALID_INPUT;
    }
    Country * country_p = countries.find(&countryId);
    Team * team = teams.find(&teamId);
    if(team->getID() == teamId && country_p->getId() != countryId) return StatusType::FAILURE;
    try{
        team_p = new Team(teamId, &countryId, sport);
    }
    catch(std::bad_alloc &e){
        return StatusType::ALLOCATION_ERROR;
    }
    Team toAdd = *team_p;
    return teams.insert(teamId, toAdd);
}

StatusType Olympics::remove_team(int teamId){
	if(teamId <= 0) return StatusType::INVALID_INPUT;
        Team * teamToRemove = teams.find(&teamId);
    if(!teamToRemove || teamToRemove->getNumContestants() > 0) return StatusType::FAILURE;
    return teams.remove(teamId);
}
	
StatusType Olympics::add_contestant(int contestantId ,int countryId,Sport sport,int strength){
	if(contestantId <= 0 || countryId <= 0 ||strength < 0) return StatusType::INVALID_INPUT;
    Country * country = countries.find(&countryId);
    Contestant * contestant = contestants.find(&countryId);
    if(contestant || !country) return StatusType::FAILURE;
    Contestant * contestant_p = nullptr;
    try{
        contestant_p = new Contestant(contestantId, &countryId, sport, strength);
    }
    catch(std::bad_alloc &e){
        return StatusType::ALLOCATION_ERROR;
    }
    Contestant addContestant = *contestant_p;
    return contestants.insert(countryId, addContestant);
}
	
StatusType Olympics::remove_contestant(int contestantId){
    if(contestantId <= 0) return StatusType::INVALID_INPUT;
    Contestant * toRemove = contestants.find(&contestantId);
    if(!toRemove || toRemove->getNumTeams() > 0) return StatusType::FAILURE;
    return contestants.remove(contestantId);
    return StatusType::FAILURE;
}
	
StatusType Olympics::add_contestant_to_team(int teamId,int contestantId){
	if(teamId <= 0 || contestantId <= 0) return StatusType::INVALID_INPUT;
    Contestant * add = contestants.find(&contestantId);
    Team * team = teams.find(&teamId);
    if(add || team->getSport() != add->getSport() || team->getCountry() != add->getCountry()) return StatusType::FAILURE;
    int strength = add->getStrength();
    StatusType status = StatusType::SUCCESS;
    if(strength < team->getS1().getMax()){ //insert to s1
        status = team->getS1().insert(contestantId, add);
        if(status != StatusType::SUCCESS) return status;
        status = team->getSt1().insert(strength, *add->getCountry());
        if(status != StatusType::SUCCESS) return status;
    }
    else if(strength < team->getS2().getMax()){ //insert to s2
        status = team->getS2().insert(contestantId, add);
        if(status != StatusType::SUCCESS) return status;
        status = team->getSt2().insert(strength, *add->getCountry());
        if(status != StatusType::SUCCESS) return status;
    }
    else{ //insert to s3
        status = team->getS3().insert(contestantId, add);
        if(status != StatusType::SUCCESS) return status;
        status = team->getSt3().insert(strength, *add->getCountry());
        if(status != StatusType::SUCCESS) return status;
    }
    if(contestants.getSize() < team->getS1().getSize()*3){ //if first tree not good
        int idMax = team->getS1().getMax();
        Contestant ** change = team->getS1().find(&idMax);
        
    }
    if(contestants.getSize() < team->getS2().getSize()*3){ //if second tree not good

    }
    if(contestants.getSize() < team->getS1().getSize()*3){ //if third tree not good

    }
    return StatusType::FAILURE;
}

StatusType Olympics::remove_contestant_from_team(int teamId,int contestantId){
	return StatusType::FAILURE;
}

StatusType Olympics::update_contestant_strength(int contestantId ,int change){
	return StatusType::FAILURE;
}

output_t<int> Olympics::get_strength(int contestantId){
	if(contestantId < 0) return StatusType::INVALID_INPUT;
    Contestant * contestant = contestants.find(&contestantId);
    if(!contestant) return StatusType::FAILURE;
    return contestant->getStrength();
}

output_t<int> Olympics::get_medals(int countryId){
	if(countryId <= 0) return output_t<int>(StatusType::INVALID_INPUT);
    Country * country = countries.find(&countryId);
    if(!country) return output_t<int>(StatusType::FAILURE);
    return output_t<int>(country->getMedals());
}

output_t<int> Olympics::get_team_strength(int teamId){
    if (teamId <= 0) return output_t<int>(StatusType::INVALID_INPUT);
    Team *team = teams.find(&teamId);
    if(!team) return output_t<int>(StatusType::FAILURE);
    return output_t<int>(team->getStrength());
}

StatusType Olympics::unite_teams(int teamId1,int teamId2){
	return StatusType::FAILURE;
}

StatusType Olympics::play_match(int teamId1,int teamId2){
	if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) return StatusType::INVALID_INPUT;
    Team * team1 = teams.find(&teamId1);
    Team * team2 = teams.find(&teamId2);
    if(!team1 || !team2 || team1->getSport() != team2->getSport()) return StatusType::FAILURE;
    Country * country1 = countries.find(team1->getCountry());
    Country * country2 = countries.find(team2->getCountry());
    int score1 = team1->getStrength() + country1->getMedals();
    int score2 = team2->getStrength() + country2->getMedals();
    if(score1 > score2) country1->addMedal();
    else if(score2 > score1) country2->addMedal();
    else{
        country1->addMedal();
        country2->addMedal();
    }
    return StatusType::SUCCESS;
}

output_t<int> Olympics::austerity_measures(int teamId){
	return 0;
}



