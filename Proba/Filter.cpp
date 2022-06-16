//
// Created by Lenovo on 5/9/2022.
//

#include "Filter.h"
#include "Country.h"
#include "Exceptions.h"

list<shared_ptr<Competitor>> Filter::sportFiltering(list<shared_ptr<Competitor>> competitors) {
    if (sport.empty())return competitors;
    competitors.erase(remove_if(competitors.begin(), competitors.end(),
                                [this](const shared_ptr<Competitor> &comp) {
                                    return comp->getEvent()->getSport()->getName() != sport;
                                }),
                      competitors.end());
    return competitors;
}

list<shared_ptr<Competitor>> Filter::countryFiltering(list<shared_ptr<Competitor>> competitors) {
    if (country.empty())return competitors;
    competitors.erase(remove_if(competitors.begin(), competitors.end(),
                                [this](const shared_ptr<Competitor> &comp) {
                                    return comp->getCountry()->getName() != country;
                                }),
                      competitors.end());
    return competitors;
}

list<shared_ptr<Competitor>> Filter::yearFiltering(set<Game> games, string season) {
    list<Game> vectGame(games.begin(), games.end());
    vectGame.erase(remove_if(vectGame.begin(), vectGame.end(),
                             [this, season](const Game &g) { 
            if (season.empty())return g.getYear() != year;
            return (g.getYear() != year) || (g.getName() != season); }), vectGame.end());

    if (vectGame.empty())throw ReturnError();

    list<shared_ptr<Competitor>> returnVect;
    for (auto tmp: vectGame) {
        for (const auto &person: *tmp.getCompetitors()) {
            returnVect.push_back(person);
        }
    }
    return returnVect;
}

list<shared_ptr<Competitor>> Filter::eventTypeFiltering(list<shared_ptr<Competitor>> competitors) {
    if (!eventFlag)return competitors;
    competitors.erase(remove_if(competitors.begin(), competitors.end(),
                                [this](const shared_ptr<Competitor> &comp) {
                                    return comp->getEvent()->getType() != eventType;
                                }),
                      competitors.end());
    return competitors;
}

list<shared_ptr<Competitor>> Filter::medalTypeFiltering(list<shared_ptr<Competitor>> competitors) {
    if (!medalFlag) return competitors;
    competitors.erase(remove_if(competitors.begin(), competitors.end(),
                                [this](const shared_ptr<Competitor> &comp) { return comp->getMedal() != medalType; }),
                      competitors.end());
    return competitors;
}
