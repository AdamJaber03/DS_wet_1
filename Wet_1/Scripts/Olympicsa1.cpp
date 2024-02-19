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
    Country * toRemove = countries.find(countryId);
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
    Country * country_p = countries.find(countryId);
    Team * team = teams.find(teamId);
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
	if(teamId <= 0){
        return StatusType::INVALID_INPUT;
    }
    Team * teamToRemove = teams.find(teamId);
    if(!teamToRemove || teamToRemove->getNumContestants() > 0){
        return StatusType::FAILURE;
    }
    return teams.remove(teamId);
}
	
StatusType Olympics::add_contestant(int contestantId ,int countryId,Sport sport,int strength){ 
	if(contestantId <= 0 || countryId <= 0 ||strength < 0) return StatusType::INVALID_INPUT;
    Country * country = countries.find(countryId);
    Contestant * contestant = contestants.find(countryId);
    if(contestant || !country) return StatusType::FAILURE;
    //continue
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
	return StatusType::FAILURE;
}
	
StatusType Olympics::add_contestant_to_team(int teamId,int contestantId){
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
    Contestant * contestant = contestants.find(contestantId);
    if(!contestant) return StatusType::FAILURE;
    return contestant->getStrength();
}

output_t<int> Olympics::get_medals(int countryId){
	if(countryId <= 0) return output_t<int>(StatusType::INVALID_INPUT);
    Country * country = countries.find(countryId);
    if(!country) return output_t<int>(StatusType::FAILURE);
    return output_t<int>(country->getMedals());
}

output_t<int> Olympics::get_team_strength(int teamId){
    if (teamId <= 0) return output_t<int>(StatusType::INVALID_INPUT);
    Team *team = teams.find(teamId);
    if(!team) return output_t<int>(StatusType::FAILURE);
    return output_t<int>(team->getStrength());
}

StatusType Olympics::unite_teams(int teamId1,int teamId2){
	return StatusType::FAILURE;
}

StatusType Olympics::play_match(int teamId1,int teamId2){
	return StatusType::FAILURE;
}

output_t<int> Olympics::austerity_measures(int teamId){
	return 0;
}



