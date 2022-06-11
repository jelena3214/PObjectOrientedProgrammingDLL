//
// Created by Lenovo on 5/9/2022.
//

#include "DataManipulation.h"
#include "Exceptions.h"
#include <unordered_map>
#include <utility>

int DataManipulation::numberOfPlayers(Filter f, string country) {
    //cout << "POSLE "; f.ispisi();
    vector<shared_ptr<Competitor>> res;
    try {
        res = getFilteredCompetitors(std::move(f));
    } catch (const exception &e) {
        
        cout << e.what() << endl;
        throw BasicFilteringError();
    }
    set<int> ids;

    for (const shared_ptr<Competitor> &cmp: res) {
        if (cmp->getCountry()->getName() != country)continue;
        auto &tmp = cmp->getId();
        for (int p: tmp) {
            ids.insert(p);
        }
    }

    return ids.size();
}

int DataManipulation::numOfDisciplines(Filter f) {
    vector<shared_ptr<Competitor>> res;
    try {
        res = getFilteredCompetitors(std::move(f));
    } catch (const exception &e) {
        cout << e.what() << endl;
        throw BasicFilteringError();
    }
    set<shared_ptr<Event>, Event::EventPrtComp> events;
    for (const shared_ptr<Competitor> &c: res) {
        events.insert(c->getEvent());
    }

    return events.size();
}

vector<shared_ptr<Competitor>> DataManipulation::getFilteredCompetitors(Filter f) {
    auto res = evParser->getCompetitors();
    if (res.empty()) {
        cout << "getFilteredCompetitors function :: ";
        throw ErrorGettingDataFromEventParser();
    }

    if (f.isYearSet()) {
        try {
            res = f.yearFiltering(evParser->getGames());
        } catch (const ReturnError &r) {
            
            throw ReturnError();
        }
    }
    res = f.sportFiltering(res);
    res = f.countryFiltering(res);
    res = f.eventTypeFiltering(res);
    res = f.medalTypeFiltering(res);
    return res;
}

double DataManipulation::averageAthletesHeight(Filter f) {
    vector<shared_ptr<Competitor>> res;
    try {
        res = getFilteredCompetitors(std::move(f));
    } catch (const exception &e) {
        cout << e.what() << endl;
        throw BasicFilteringError();
    }

    int averageHeight = 0;
    int numOfAthletes = 0;
    for (const shared_ptr<Competitor> &cmp: res) {
        for (int i: cmp->getId()) {
            int height = athletes->getPerson(i)->getHeight();
            if (height) {
                averageHeight += height;
                numOfAthletes++;
            }
        }
    }
    if (averageHeight == 0)return 0;
    return averageHeight / numOfAthletes;

}

double DataManipulation::averageAthletesWeight(Filter f) {
    vector<shared_ptr<Competitor>> res;
    try {
        res = getFilteredCompetitors(std::move(f));
    } catch (const exception &e) {
        cout << e.what() << endl;
        throw BasicFilteringError();
    }

    int averageWeight = 0;
    int numOfAthletes = 0;
    for (const shared_ptr<Competitor> &cmp: res) {
        for (int i: cmp->getId()) {
            int weight = athletes->getPerson(i)->getWeight();
            if (weight) {
                averageWeight += weight;
                numOfAthletes++;
            }
        }
    }
    if (averageWeight == 0)return 0;
    return averageWeight / numOfAthletes;
}

int DataManipulation::numberOfDifferentSportsWithMedal(const string &country) {
    auto countries = evParser->getCountries();
    auto found = countries.find(Country(country));

    if (found == countries.end()) {
        cout << "numberOfDifferentSportsWithMedal function :: Can't find country :: ";
        throw AdvancedFilteringError();
    }

    set<Sport> sportsWithMedal;
    auto countryData = *found;

    for (const shared_ptr<Competitor> &comp: countryData->getCompetitors()) {
        if (comp->getMedal() != MedalType::NA) {
            sportsWithMedal.insert(*comp->getEvent()->getSport());
        }
    }
    return sportsWithMedal.size();
}

deque<shared_ptr<Country>> DataManipulation::bestCountriesAtGame(int year, const string &season) {
    struct medalCounter {
        mutable int gold, silver, bronze;

        medalCounter() : gold(0), silver(0), bronze(0) {}

        void incGold() const { gold++; }

        void incSilver() const { silver++; }

        void incBronze() const { bronze++; }
    };
    typedef pair<shared_ptr<Country>, medalCounter> MyPair;

    struct Comp { //for comparing pairs
        bool operator()(const MyPair &e, const MyPair &e1) const {
            if (e.first->getName() < e1.first->getName())return true;
            if (e.first->getName() > e1.first->getName())return false;
            return false;
        }
    };

    set<MyPair, Comp> countryMedalCount;
    vector<MyPair> sortedVect;

    vector<shared_ptr<Competitor>> competitors = getCompetitorsOnGame(year, season);

    for (auto &cmp: competitors) { //inserting into set first because of duplicates
        MyPair p;
        p.first = cmp->getCountry();
        auto a = countryMedalCount.insert(p);
        if (cmp->getMedal() == MedalType::GOLD)a.first->second.incGold();
        if (cmp->getMedal() == MedalType::SILVER)a.first->second.incSilver();
        if (cmp->getMedal() == MedalType::BRONZE)a.first->second.incBronze();
    }

    for (const MyPair &p: countryMedalCount) { //inserting in vector because of sorting
        sortedVect.push_back(p);
    }

    sort(sortedVect.begin(), sortedVect.end(), [](const MyPair &e, const MyPair &e1) {
        if (e.second.gold != e1.second.gold) { return e.second.gold > e1.second.gold; }
        if (e.second.silver != e1.second.silver) { return e.second.silver > e1.second.silver; }
        if (e.second.bronze != e1.second.bronze)return e.second.bronze > e1.second.bronze;
        return false;
    });

    deque<shared_ptr<Country>> threeBestCountries; //because we need to keep insertion order, set can't do that
    int lenght = 3;
    if (sortedVect.size() < 3)lenght = sortedVect.size();
    for (int i = 0; i < lenght; i++)threeBestCountries.push_back((sortedVect.begin() + i)->first);
    return threeBestCountries;
}

//todo za obe ove funkcije sta ako su neke drzave jednake u broju medalja, pretpostavka i da su jednake nek ih leksikografski rasporedi pa nek uzme prvi 3

set<shared_ptr<Country>> DataManipulation::bestCountries() {
    set<shared_ptr<Country>> countries;
    auto games = evParser->getGames();

    for (const Game &g: games) {
        auto ret = bestCountriesAtGame(g.getYear(), g.getName());
        auto s = ret.front();
        countries.insert(s);
    }

    return countries;
}

set<string> DataManipulation::olympicCities() {
    set<string> cities;

    auto games = evParser->getGames();

    for (const Game &tmpGame: games) {
        cities.insert(tmpGame.getCity());
    }

    return cities;
}

set<shared_ptr<Person>> DataManipulation::participatedAtGames(const pair<Game, Game> &gamePair) {
    auto games = evParser->getGames();
    auto gameTmp1 = games.find(gamePair.first);
    auto gameTmp2 = games.find(gamePair.second);

    if (gameTmp1 == games.end() || gameTmp2 == games.end()) {
        cout << "participatedAtGames error : game not found :: ";
        throw AdvancedFilteringError();
    }

    auto gameCompetitors1 = *gameTmp1->getCompetitors();
    auto gameCompetitors2 = *gameTmp2->getCompetitors();

    vector<int> game1Ids, game2Ids;
    set<int> intersection;
    
    for (const shared_ptr<Competitor> &comp: gameCompetitors1) {
        for (int p: comp->getId()) {
            game1Ids.push_back(p);
        }
    }

    for (const shared_ptr<Competitor> &comp: gameCompetitors2) {
        for (int p: comp->getId()) {
            game2Ids.push_back(p);
        }
    }

    sort(game1Ids.begin(), game1Ids.end());
    sort(game2Ids.begin(), game2Ids.end());

    set_intersection(game1Ids.begin(), game1Ids.end(), game2Ids.begin(), game2Ids.end(),
                     inserter(intersection, intersection.begin()));

    if (intersection.empty()) {
        cout << "No athletes that match the search\n";
        return {};
    }

    return athletes->getPeople(intersection);
}

vector<shared_ptr<Competitor>>
DataManipulation::countryTeamsAtGame(int year, const string &season, const string &country) {
    vector<shared_ptr<Competitor>> competitors = getCompetitorsOnGame(year, season);

    competitors.erase(remove_if(competitors.begin(), competitors.end(),
                                [this, &country](const shared_ptr<Competitor> &comp) {
                                    return comp->getCountry()->getName() != country || comp->getId().size() == 1;
                                }),
                      competitors.end());


    sort(competitors.begin(), competitors.end(), [](const shared_ptr<Competitor> &c, const shared_ptr<Competitor> &c1) {
        if (c->getId().size() == c1->getId().size()) {
            if (c->getEvent()->getName() != c1->getEvent()->getName())
                return c->getEvent()->getName() < c1->getEvent()->getName();
            return false;
        }
        return c->getId().size() > c1->getId().size();
    });


    return competitors;
}

set<pair<shared_ptr<Country>, shared_ptr<Person>>> DataManipulation::wonIndividualAndTeamMedal() {
    auto &countries = evParser->getCountries();

    set<pair<shared_ptr<Country>, shared_ptr<Person>>> returnSet;

    //mora referenca za count jer bez nje ce se kopirati vrednost
    for (auto &count: countries) {
        set<int> wonIndividualMedal;
        set<int> wonTeamMedal;
        for (const shared_ptr<Competitor> &cmp: count->getCompetitors()) {
            if (cmp->getMedal() == MedalType::NA)continue;
            if (cmp->getId().size() == 1) { //individual
                wonIndividualMedal.insert(*cmp->getId().begin());
            } else {
                wonTeamMedal.insert(cmp->getId().begin(), cmp->getId().end());
            }
        }

        set<int> intersect;
        set_intersection(wonTeamMedal.begin(), wonTeamMedal.end(), wonIndividualMedal.begin(), wonIndividualMedal.end(),
                         std::inserter(intersect, intersect.begin()));
        if (intersect.empty())continue;
        for (int id: intersect) {
            pair<shared_ptr<Country>, shared_ptr<Person>> tmp;
            tmp.first = count;
            tmp.second = athletes->getPerson(id);
            returnSet.insert(tmp);
        }
    }
    return returnSet;
}

vector<shared_ptr<Person>> DataManipulation::bestYoungestAthletes() {
    struct YearMedal {
        int year;
        int wonMedal;

        YearMedal(int y, int wm) {
            year = y;
            wonMedal = wm;
        }
    };

    unordered_map<int, YearMedal> mapPlayerFirstMedal;
    auto games = evParser->getGames();

    for (auto &game: games) {
        auto competitors = *game.getCompetitors();
        for (const auto &competitor: competitors) {
            for (int id: competitor->getId()) {
                auto entry = mapPlayerFirstMedal.find(id);
                if (entry == mapPlayerFirstMedal.end()) { //Doesn't exist
                    mapPlayerFirstMedal.insert(make_pair(id, YearMedal(game.getYear(),
                                                                       (competitor->getMedal() != MedalType::NA ? 1
                                                                                                                : 0))));
                } else { //Already exists, change value if it is needed
                    if (game.getYear() < mapPlayerFirstMedal.find(id)->second.year) {
                        entry->second = YearMedal(game.getYear(), (competitor->getMedal() != MedalType::NA ? 1 : 0));
                    }
                }

            }
        }
    }


    vector<shared_ptr<Person>> athletesWithMedal;

    for (auto it: mapPlayerFirstMedal) {
        if (it.second.wonMedal) {
            athletesWithMedal.push_back(athletes->getPerson(it.first));
        }
    }

    athletesWithMedal.erase(
            std::remove_if(athletesWithMedal.begin(), athletesWithMedal.end(),
                           [](const shared_ptr<Person> &o) { return o->getYears() == 0; }),
            athletesWithMedal.end());

    sort(athletesWithMedal.begin(), athletesWithMedal.end(),
         [](const shared_ptr<Person> &p1, const shared_ptr<Person> &p2) {
             return p1->getYears() < p2->getYears();
         });


    if (athletesWithMedal.size() > 10) {
        athletesWithMedal.resize(10);
    }
    return athletesWithMedal;

}

vector<shared_ptr<Competitor>> DataManipulation::getCompetitorsOnGame(int year, const string &season) {
    auto games = evParser->getGames();

    vector<Game> vectGame;
    for (const auto &game: games) { //Find games that happened that year
        if (game.getYear() == year && game.getName() == season) {
            vectGame.push_back(game);
        }
    }

    if (vectGame.empty()) {
        cout << "helper error : game not found :: ";
        throw AdvancedFilteringError();
    }

    vector<shared_ptr<Competitor>> competitors; //Competitors on those games
    for (auto elem: vectGame) {
        for (const auto &comp: *elem.getCompetitors()) {
            competitors.push_back(comp);
        }
    }

    return competitors;
}
