//
// Created by Lenovo on 5/9/2022.
//

#include "Filter.h"
#include "Country.h"
#include "Exceptions.h"

vector<shared_ptr<Competitor>> Filter::sportFiltering(vector<shared_ptr<Competitor>> competitors) {
    if (sport.empty())return competitors;
    competitors.erase(remove_if(competitors.begin(), competitors.end(),
                                [this](const shared_ptr<Competitor> &comp) {
                                    return comp->getEvent()->getSport()->getName() != sport;
                                }),
                      competitors.end());
    return competitors;
}

vector<shared_ptr<Competitor>> Filter::countryFiltering(vector<shared_ptr<Competitor>> competitors) {
    if (country.empty())return competitors;
    competitors.erase(remove_if(competitors.begin(), competitors.end(),
                                [this](const shared_ptr<Competitor> &comp) {
                                    return comp->getCountry()->getName() != country;
                                }),
                      competitors.end());
    return competitors;
}

vector<shared_ptr<Competitor>> Filter::yearFiltering(set<Game> games) {
    vector<Game> vectGame(games.begin(), games.end());
    vectGame.erase(remove_if(vectGame.begin(), vectGame.end(),
                             [this](const Game &g) { return g.getYear() != year; }),
                   vectGame.end());

    if (vectGame.empty())throw ReturnError();

    vector<shared_ptr<Competitor>> returnVect;
    for (auto tmp: vectGame) {
        for (const auto &person: *tmp.getCompetitors()) {
            returnVect.push_back(person);
        }
    }
    return returnVect;
}

vector<shared_ptr<Competitor>> Filter::eventTypeFiltering(vector<shared_ptr<Competitor>> competitors) {
    if (!eventFlag)return competitors;
    competitors.erase(remove_if(competitors.begin(), competitors.end(),
                                [this](const shared_ptr<Competitor> &comp) {
                                    return comp->getEvent()->getType() != eventType;
                                }),
                      competitors.end());
    return competitors;
}

vector<shared_ptr<Competitor>> Filter::medalTypeFiltering(vector<shared_ptr<Competitor>> competitors) {
    if (!medalFlag) return competitors;
    competitors.erase(remove_if(competitors.begin(), competitors.end(),
                                [this](const shared_ptr<Competitor> &comp) { return comp->getMedal() != medalType; }),
                      competitors.end());
    return competitors;
}
