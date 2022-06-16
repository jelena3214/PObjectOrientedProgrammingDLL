//
// Created by Lenovo on 5/9/2022.
//

#ifndef POOP_FILTER_H
#define POOP_FILTER_H

#include <string>
#include <list>
#include "EventTypeClass.h"
#include "MedalTypeClass.h"
#include "Competitor.h"
#include "Sport.h"
#include "Game.h"

using namespace std;

class Filter {
private:
    string sport, country;
    int year;
    EventType eventType;
    MedalType medalType;
    bool eventFlag = false, medalFlag = false;

public:
    explicit Filter(const string& ss = "", const string& cc = "", const int yy = 0, const string& ev = "",
                    const string& mt = "") :
            sport(ss), country(cc), year(yy) {
        if (!ev.empty()) eventType = EventTypeClass::getEventTypeFromString(ev), eventFlag = true;
        if (!mt.empty()) medalType = MedalTypeClass::getMedalTypeFromString(mt), medalFlag = true;
    }

    list<shared_ptr<Competitor>> sportFiltering(list<shared_ptr<Competitor>> competitors);

    list<shared_ptr<Competitor>> countryFiltering(list<shared_ptr<Competitor>> competitors);

    list<shared_ptr<Competitor>> yearFiltering(set<Game> games, string season = "");

    list<shared_ptr<Competitor>> eventTypeFiltering(list<shared_ptr<Competitor>> competitors);

    list<shared_ptr<Competitor>> medalTypeFiltering(list<shared_ptr<Competitor>> competitors);

    bool isYearSet() const { return year != 0; }
    void ispisi() {
        cout << sport << country << eventType << medalType << endl;
    }
    int getYear()const { return year; }
};


#endif //POOP_FILTER_H
