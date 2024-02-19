//
// Created by JaberA on 19/02/2024.
//

#ifndef DS_WET_1_COUNTRY_H
#define DS_WET_1_COUNTRY_H

class Country{
public:
    Country(int id, int medals): id(id), medals(medals), contestants(0), teams(0){};
    getId();
    getMedals;
    addMedal();
private:
    int id;
    int medals;
    int contestants;
    int teams;
};

#endif //DS_WET_1_COUNTRY_H
