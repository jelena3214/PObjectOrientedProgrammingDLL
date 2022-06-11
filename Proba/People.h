//
// Created by Lenovo on 4/28/2022.
//

#ifndef POOP_PEOPLE_H
#define POOP_PEOPLE_H

#include <map>
#include "Person.h"
#include<memory>
#include <set>

//Postoji samo jedna instanca People koja zadrzi sve potrebne ucesnike za taj filter
//Na kraju programa se to dealocira
//A dok u Athlete i Team cuvamo samo id/jeve koji im trebaju
using namespace std;

class People {
    map<int, shared_ptr<Person>> athletes;

    static People *instance;

    //People() = default;

public:
    static People* getInstance() {
        if (instance == nullptr) {
            instance = new People();
        }
        return instance;
    }

    People() = default;

    static void deleteInstance() { delete instance; }

    void addPerson(shared_ptr<Person>& p) {
        athletes[p->getId()] = p;
    }

    set<shared_ptr<Person>> getPeople(set<int> ids);

    shared_ptr<Person>& getPerson(int id) {
        return athletes.at(id);
    }

};


#endif //POOP_PEOPLE_H
