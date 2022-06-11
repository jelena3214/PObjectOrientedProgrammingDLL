//
// Created by Lenovo on 4/28/2022.
//

#include "People.h"

People *People::instance = nullptr;

set<shared_ptr<Person>> People::getPeople(set<int> ids) {
    set<shared_ptr<Person>> returnPeople;

    for (int id: ids) {
        returnPeople.insert(getPerson(id));
    }

    return returnPeople;
}
