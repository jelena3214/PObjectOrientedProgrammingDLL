//
// Created by Lenovo on 4/24/2022.
//

#ifndef POOP_MEDALTYPECLASS_H
#define POOP_MEDALTYPECLASS_H

#include<string>

using namespace std;

class MedalTypeClass {
public:
    enum MedalType {
        GOLD, SILVER, BRONZE, NA
    };

    static MedalType getMedalTypeFromString(const string &medal) {
        if (medal == "Gold") {
            return GOLD;
        } else if (medal == "Silver") {
            return SILVER;
        } else if (medal == "Bronze") {
            return BRONZE;
        } else {
            return NA;
        }
    }
};

using MedalType = MedalTypeClass::MedalType;


#endif //POOP_MEDALTYPECLASS_H
