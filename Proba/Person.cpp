//
// Created by Lenovo on 4/28/2022.
//

#include "Person.h"

ostream &operator<<(ostream &os, const Person &c) {
    os << c.id << ", " << c.name << ", " << c.gender << ", " << c.yearsOld << ", " << c.weight << ", " << c.height;
    return os;
}
