//
// Created by Lenovo on 4/25/2022.
//

#ifndef POOP_EVENTPARSER_H
#define POOP_EVENTPARSER_H

#include<fstream>
#include<iostream>
#include<string>
#include "Competitor.h"
#include "Sport.h"
#include "Game.h"
#include "Country.h"
#include "Athlete.h"
#include "Team.h"
#include <set>

using namespace std;

class EventParser {
public:

    template <typename T>  //For comparing shared pointers(Country and Sport)
    struct SharedPtrComparator{
        using is_transparent = void;
        bool operator()(const shared_ptr<T>& e, const shared_ptr<T>& e1) const {
            return *e < *e1;
        }
        bool operator()(const shared_ptr<T>& e, const T& e1) const {
            return *e < e1;
        }
        bool operator()(const T& e, const shared_ptr<T>& e1) const {
            return e < *e1;
        }
    };

public:

    vector<shared_ptr<Competitor>> competitors;
    set<shared_ptr<Sport>, SharedPtrComparator<Sport>> sports;
    set<Game> gamesSet;
    set<shared_ptr<Country>, SharedPtrComparator<Country>> countrySet;
    set<int> athletesId;


public:
    void eventParsing(const char *fileName, int year = -1);

    vector<shared_ptr<Competitor>> &getCompetitors() {
        return competitors; 
    }

    set<shared_ptr<Sport>, SharedPtrComparator<Sport>>& getSports() { return sports; }

    set<Game>& getGames() { return gamesSet; }

    set<shared_ptr<Country>, SharedPtrComparator<Country>>& getCountries() { return countrySet; }

    set<int>& getAthleteIds() { return athletesId; }

};


#endif //POOP_EVENTPARSER_H
