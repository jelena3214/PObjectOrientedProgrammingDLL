//
// Created by Lenovo on 4/24/2022.
//

#ifndef POOP_ATHLETE_H
#define POOP_ATHLETE_H

#include<string>
#include<iostream>
#include "Competitor.h"

using namespace std;

class Athlete : public Competitor {
private:
    set<int> id;

public:
    explicit Athlete(int idd) {id.insert(idd);}

    friend bool operator==(const Athlete &a1, const Athlete &a2);

    set<int>& getId() override {
        return id;
    }
};


#endif //POOP_ATHLETE_H
