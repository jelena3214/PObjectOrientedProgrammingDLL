//
// Created by Lenovo on 4/24/2022.
//

#include "Athlete.h"

bool operator==(const Athlete &a1, const Athlete &a2) {
    return *a1.id.begin() == *a2.id.begin();
}
