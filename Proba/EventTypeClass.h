//
// Created by Lenovo on 4/24/2022.
//

#ifndef POOP_EVENTTYPECLASS_H
#define POOP_EVENTTYPECLASS_H

#include <string>

using namespace std;


class EventTypeClass {
public:
    enum EventType {
        TEAM, INDIVIDUAL
    };

    static EventType getEventTypeFromString(const string &s) {
        if (s == "Individual") return INDIVIDUAL;
        return TEAM;
    }

};

using EventType = EventTypeClass::EventType;

#endif //POOP_EVENTTYPECLASS_H
