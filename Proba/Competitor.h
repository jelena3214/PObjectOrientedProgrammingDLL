//
// Created by Lenovo on 4/24/2022.
//

#ifndef POOP_COMPETITOR_H
#define POOP_COMPETITOR_H

#include "Event.h"
#include "MedalTypeClass.h"
#include <memory>
#include <set>
#include <iostream>

using namespace std;

class Country;

class Competitor {
protected:
    MedalType medal;
    shared_ptr<Event> event;
    shared_ptr<Country> country;
public:
    void setMedal(MedalType& m) { medal = m; }

    void setEvent(shared_ptr<Event>& e) { event = e; }

    shared_ptr<Event> getEvent() const { return event; }

    MedalType getMedal() const { return medal; }

    void setCountry(shared_ptr<Country> &c) { country = c; }

    shared_ptr<Country> getCountry() const { return country; }

    virtual set<int>& getId() = 0;

};


#endif //POOP_COMPETITOR_H
