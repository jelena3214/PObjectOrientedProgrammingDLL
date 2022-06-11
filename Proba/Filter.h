//
// Created by Lenovo on 5/9/2022.
//

#ifndef POOP_FILTER_H
#define POOP_FILTER_H

#include <string>
#include <vector>
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

    vector<shared_ptr<Competitor>> sportFiltering(vector<shared_ptr<Competitor>> competitors);

    vector<shared_ptr<Competitor>> countryFiltering(vector<shared_ptr<Competitor>> competitors);

    vector<shared_ptr<Competitor>> yearFiltering(set<Game> games);

    vector<shared_ptr<Competitor>> eventTypeFiltering(vector<shared_ptr<Competitor>> competitors);

    vector<shared_ptr<Competitor>> medalTypeFiltering(vector<shared_ptr<Competitor>> competitors);

    bool isYearSet() const { return year != 0; }
    void ispisi() {
        cout << sport << country << eventType << medalType << endl;
    }
};


#endif //POOP_FILTER_H
