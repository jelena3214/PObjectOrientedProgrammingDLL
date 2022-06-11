//
// Created by Lenovo on 4/26/2022.
//

#ifndef POOP_ATHLETESPARSER_H
#define POOP_ATHLETESPARSER_H

#include <set>
#include "Athlete.h"
#include "People.h"
#include<fstream>
#include <regex>

class AthetesParser {
private:
    set<int> athletes;
public:
    AthetesParser(set<int> &at) : athletes(at) {}

    void athletesParse(const char *fileName, People &ppl);
};


#endif //POOP_ATHLETESPARSER_H
