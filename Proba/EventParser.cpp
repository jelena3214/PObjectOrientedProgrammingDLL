//
// Created by Lenovo on 4/25/2022.
//

#include "EventParser.h"
#include "EventTypeClass.h"
#include "Exceptions.h"
#include<regex>

using namespace std;

void EventParser::eventParsing(const char *fileName, int findForYear) {
    fstream eventFile;
    eventFile.open(fileName, ios::in);

    if (!eventFile)throw FileNotFound();

    string tmp;
    regex reLine("(\\d+) ([^!]+)!([^!]+)!([^!]+)!([^!]+)!([^!]+)!([^!]+)!([^!]+)!([^\\n]*)", regex::optimize);
    regex reIdParse("\\, ", regex::optimize);

    smatch matchLine;
    smatch matchYearSeason; //Year & season

    bool group = (findForYear == -1); //Group regime
  
    while (getline(eventFile, tmp)) {
        if (regex_search(tmp, matchLine, reLine)) {
            int year = stoi(matchLine.str(1));

            string season = matchLine.str(2);

            if (!group) {
                if (year != findForYear)continue;
            }

            string city = matchLine.str(3);
            string sport = matchLine.str(4);
            string event = matchLine.str(5);
            string type = matchLine.str(6);
            string country = matchLine.str(7);
            string ids = matchLine.str(8);
            string medal = matchLine.str(9);

            //Insert game
            auto tmpGamePair = gamesSet.insert(Game(season, year, city));
            auto &currentGame = *tmpGamePair.first;

            //Insert country
            auto tmpPair = countrySet.find(Country(country));
            shared_ptr<Country> currentCountry;
            if (tmpPair == countrySet.end()) { //Doesn't exist
                auto insertRes = countrySet.insert(make_shared<Country>(Country(country)));
                currentCountry = *insertRes.first;
            } else {
                currentCountry = *tmpPair;
            }

            //Insert sport
            auto tmpSportPair = sports.find(Sport(sport));
            shared_ptr<Sport> currentSport;
            if (tmpSportPair == sports.end()) { //Doesn't exist
                auto insertRes = sports.insert(make_shared<Sport>(Sport(sport)));
                currentSport = *insertRes.first;
            } else {
                currentSport = *tmpSportPair;
            }

            //Insert Event to sport
            auto currentEvent = currentSport->addEvent(event, type);
            currentEvent->setSport(currentSport);

            //Make new Competitor
            shared_ptr<Competitor> newCompetitor;
            if (ids[0] == '[') { //Team
                shared_ptr<Team> newTeam = make_shared<Team>();
                ids = ids.substr(1, ids.size() - 2);

                //Parsing string
                vector<string> out(
                        sregex_token_iterator(ids.begin(), ids.end(), reIdParse, -1),
                        sregex_token_iterator()
                );

                for (auto IdPart: out) {
                    IdPart = IdPart.substr(1, IdPart.size() - 2); //Removing quotes
                    int newId = stoi(IdPart);
                    newTeam->addAthlete(newId);
                    athletesId.insert(newId);
                }
                newCompetitor = newTeam;
            } else { //Athlete
                int newId = stoi(ids);
                shared_ptr<Athlete> newAthlete = make_shared<Athlete>(newId);
                athletesId.insert(newId);
                newCompetitor = newAthlete;
            }

            //Set Medal for Competitor
            MedalType m = MedalTypeClass::getMedalTypeFromString(medal);
            newCompetitor->setEvent(currentEvent);
            newCompetitor->setMedal(m);

            //Set Country pointer for Competitor
            newCompetitor->setCountry(currentCountry);
            currentCountry->addCompetitor(newCompetitor); //Adding competitor to the country

            //Adding competitor to the game
            currentGame.addCompetitor(newCompetitor);
            competitors.push_back(newCompetitor);
        } else {
            cout << "Not found" << " :: ";
            throw RegexError();
        }
    }

    if (gamesSet.empty()) {
        cout << "The needed year doesn't exist in the file" << " :: ";
        throw LoadingError();
    }

    cout << "SIZE ::: " << competitors.size();
    cout << endl << "KRAAAAAAJ PARS" << endl;
    eventFile.close();
}


