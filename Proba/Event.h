//
// Created by Lenovo on 4/24/2022.
//

#ifndef POOP_EVENT_H
#define POOP_EVENT_H

#include "EventTypeClass.h"
#include<string>
#include <memory>
#include <utility>

using namespace std;

class Sport;

class Event {
    string name;
    EventType evType;
    shared_ptr<Sport> sport;

public:
    Event(string n, EventType evt) : name(std::move(n)), evType(evt) {}

    void setSport(shared_ptr<Sport>& s) {
        sport = s;
    }

    string getName() const { return name; }

    EventType getType() const { return evType; }

    shared_ptr<Sport> getSport() const;

    bool operator==(const Event &e) const;

    bool operator<(const Event& e) const;

    struct EventPrtComp { //for comparing pointers in set of Event*
        using is_transparent = void;
        bool operator()(const shared_ptr<Event>& e, const shared_ptr<Event>& e1) const {
            if (e->getName() < e1->getName())return true;
            if (e->getName() > e1->getName())return false;
            if (e->getType() > e1->getType())return true;
            return false;
        }
        bool operator()(const shared_ptr<Event>& e, const Event& e1) const {
            if (e->getName() < e1.getName())return true;
            if (e->getName() > e1.getName())return false;
            if (e->getType() > e1.getType())return true;
            return false;
        }

        bool operator()(const Event& e, const shared_ptr<Event>& e1) const {
            if (e.getName() < e1->getName())return true;
            if (e.getName() > e1->getName())return false;
            if (e.getType() > e1->getType())return true;
            return false;
        }
    };

};


#endif //POOP_EVENT_H
