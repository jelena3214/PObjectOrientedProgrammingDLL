//
// Created by Lenovo on 4/28/2022.
//

#ifndef POOP_PERSON_H
#define POOP_PERSON_H

#include<string>
#include <iostream>

using namespace std;

//Samo sadrzi informacije o ljudima koji su nam potrebni
class Person {
    string name, gender;
    int yearsOld, height, weight;
    int id;

public:
    Person(int idd, int year, int h, int w, const string &n, const string &g) : id(idd), yearsOld(year), gender(g),
                                                                                name(n), height(h), weight(w) {}

    int getId() const { return id; }

    int getHeight() const { return height; }

    int getWeight() const { return weight; }

    int getYears() const { return yearsOld; }

    friend ostream &operator<<(ostream &, const Person &c);
};


#endif //POOP_PERSON_H
