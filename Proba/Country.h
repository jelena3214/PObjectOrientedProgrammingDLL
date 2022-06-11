//
// Created by Lenovo on 4/24/2022.
//

#ifndef POOP_COUNTRY_H
#define POOP_COUNTRY_H

#include "Competitor.h"
#include<string>
#include<iostream>
#include <utility>
#include<vector>

using namespace std;

class Country {
    string name;
    vector<shared_ptr<Competitor>> competitors;
public:
    Country(string n) : name(std::move(n)) {}

    Country() = default;

    void addCompetitor(const shared_ptr<Competitor> &c);

    Country(const Country &c);

    string getName() { return name; }

    vector<shared_ptr<Competitor>> getCompetitors() const { return competitors; }

    bool operator==(const Country &c);

    bool operator<(const Country &c) const;

    friend ostream &operator<<(ostream &, const Country &c);
};


#endif //POOP_COUNTRY_H
