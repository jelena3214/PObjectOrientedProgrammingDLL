//
// Created by Lenovo on 4/24/2022.
//

#ifndef POOP_TEAM_H
#define POOP_TEAM_H

#include<iostream>
#include <set>
#include "Competitor.h"

using namespace std;

class Team : public Competitor {
private:
    set<int> athleteIds;

public:
    Team() = default;

    void addAthlete(int athlete);

    set<int>& getId() override {
        return athleteIds;
    }

};


#endif //POOP_TEAM_H
