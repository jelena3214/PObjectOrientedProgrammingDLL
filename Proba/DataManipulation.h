//
// Created by Lenovo on 5/9/2022.
//

#ifndef POOP_DATAMANIPULATION_H
#define POOP_DATAMANIPULATION_H

#include <algorithm>
#include <vector>
#include "Filter.h"
#include "EventParser.h"
#include "AthletesParser.h"
#include <deque>

using namespace std;

class DataManipulation {
private:
    EventParser *evParser;
    People *athletes;
public:
    DataManipulation() = default;
    DataManipulation(EventParser *ev, People *ath) {
        evParser = ev;
        athletes = ath;
    }

    /**
     * Basic filtering options
     */
    int numberOfPlayers(Filter f, string country);

    int numOfDisciplines(Filter f, string season);

    double averageAthletesHeight(Filter f);

    double averageAthletesWeight(Filter f);

    vector<shared_ptr<Competitor>> getFilteredCompetitors(Filter f, string season = "");

    /**
     * Advanced data operations
     */
    int numberOfDifferentSportsWithMedal(const string &country);

    deque<shared_ptr<Country>> bestCountriesAtGame(int year, const string &season);

    set<shared_ptr<Country>> bestCountries();

    vector<shared_ptr<Person>> bestYoungestAthletes();

    set<pair<shared_ptr<Country>, shared_ptr<Person>>> wonIndividualAndTeamMedal();

    set<shared_ptr<Person>> participatedAtGames(const pair<Game, Game>&);

    vector<shared_ptr<Competitor>> countryTeamsAtGame(int year, const string &season, const string &country);

    set<string> olympicCities();

    //Helper
    vector<shared_ptr<Competitor>> getCompetitorsOnGame(int year, const string &season);

};


#endif //POOP_DATAMANIPULATION_H
