//
// Created by Lenovo on 4/24/2022.
//

#include "Sport.h"

bool Sport::operator==(const Sport &s) const {
    return name == s.name;
}

bool Sport::operator<(const Sport &s) const {
    return name < s.name;
}

shared_ptr<Event> Sport::addEvent(const string &name, const string &type) const {
    EventType e = EventTypeClass::getEventTypeFromString(type);
    auto eventPair = events.find(Event(name, e));
    shared_ptr<Event> event;
    if (eventPair == events.end()) { //Doesn't exist
        auto insertRes = events.insert(make_shared<Event>(Event(name, e)));
        event = *insertRes.first;
    } else {
        event = *eventPair;
    }
    return std::move(event);
}
