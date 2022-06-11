//
// Created by Lenovo on 4/24/2022.
//

#include "Game.h"
#include <tuple>

void Game::addCompetitor(shared_ptr<Competitor>& c) const{
    if (c != nullptr) {
        competitors.push_back(c);
    }
}

bool Game::operator==(const Game &g) {
    return (season == g.season && year == g.year) || (season == g.season && year == g.year && city == g.city);
}

bool Game::operator<(const Game &g2) const {
    if(g2.city.empty() || city.empty())return tie(season, year) < tie(g2.season, g2.year);
    else {
        return tie(season, year, city) < tie(g2.season, g2.year, g2.city);
    }
}

vector<shared_ptr<Competitor>> *Game::getCompetitors() {
    return &competitors;
}

const vector<shared_ptr<Competitor>> *Game::getCompetitors() const {
    return &competitors;
}




