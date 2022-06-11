#include "AthletesParser.h"
#include "DataManipulation.h"
#include "EventParser.h"

#ifndef FUNCTIONS_H
#define FUNCTIONS_H
extern People* athletes;
extern EventParser* evParser;
//static DataManipulation dm;

//dodati su default consturct za people i dm

/*void initData(const char* filename1, const char* filename2) {
	athletes = People::getInstance();
	evParser.eventParsing(filename1); //vidi za drugi arg
	AthetesParser ath(evParser.getAthleteIds()); //moze ovako??? da bi se jednom ucitavalo
	ath.athletesParse(filename2, athletes);
	dm = DataManipulation(&evParser, &athletes);
}*/

#endif