#include "olympicgames_OlympicGames.h"
#include "functions.h"

People* athletes = People::getInstance();
EventParser* evParser = new EventParser();

map<string, int> countryNumOfParticipants(string sport, int year, string evt, string mdt) {
	auto countries = evParser->getCountries();
	DataManipulation dm(evParser, athletes);
	map<string, int> returnMap;
	for (const shared_ptr<Country>& cnt : countries) {
		//returnMap.insert({ cnt->getName(), dm.numberOfPlayers(Filter(), cnt->getName()) });
	}
	return returnMap;
}

/*JNIEXPORT jobject JNICALL Java_olympicgames_OlympicGames_numOfParticipants
(JNIEnv* env, jobject, jstring sport, jint, jint, jint) {
	jboolean isCopy;
	const char* newDir = (*env).GetStringUTFChars(sport, &isCopy);
	//std::string Csport = std::string(newDir, )
}*/


//COPY CONSTRUCTON AKO HOCEMO DA RADI I PITAJ ACU ZASTO NECE

JNIEXPORT jobject JNICALL Java_olympicgames_OlympicGames_numOfParticipants
(JNIEnv* env, jobject, jobject athletess, jobject events, jstring sport, jint year, jint type, jint medalType) {
	//People* ppl = (People*)athletes;
	//EventParser* evP = (EventParser*)events;
	DataManipulation dm(evParser, athletes);
	auto countries = evParser->getCountries();
	map<string, int> returnMap;

	jboolean isCopy;
	const char* convSport = (*env).GetStringUTFChars(sport, &isCopy);
	string ss = convSport;
	cout << ss << "CONVV";
	env->ReleaseStringUTFChars(sport, convSport);
	string stype, smedal;
	switch ((int)type) {
		case 0: 
			stype = "Individal";
			break;
		case 1: 
			stype = "Team";
			break;
		default:
			stype = "";
	}
	

	switch ((int)medalType) {
		case 0:
			smedal = "Gold";
			break;
		case 1:
			smedal = "Silver";
			break;
		case 2:
			smedal = "Bronze";
			break;
		default:
			smedal = "";
	}
	//cout << evParser->competitors.size() << " VEL";
	//vector<shared_ptr<Competitor>> cp = evP->competitors;
	//cout << "DOVDE0";
	cout << smedal << " " << stype;
	
	for (const shared_ptr<Country>& cnt : countries) {
		Filter f(ss, cnt->getName(), (int)year, stype, smedal);
		//cout << "PRE "; f.ispisi();
		int num = dm.numberOfPlayers(f, cnt->getName());
		if(num)returnMap.insert({ cnt->getName(), num });
	}
	cout << returnMap.size() << "MAP SIZE" << endl;
	jclass mapClass = env->FindClass("java/util/HashMap");
	//cout << "DOVDE11";
	jmethodID init = env->GetMethodID(mapClass, "<init>", "()V");
	//cout << "DOVDE111";
	jobject hashMap = env->NewObject(mapClass, init);
	//cout << "DOVDE11111";
	jmethodID put = env->GetMethodID(mapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
	//cout << "DOVDE1";
	auto consIterator = returnMap.begin();
	for (; consIterator != returnMap.end(); ++consIterator) {
		jstring keyJava = env->NewStringUTF(consIterator->first.c_str());
		jint valueJava = consIterator->second;

		jclass cls = env->FindClass("java/lang/Integer");
		jmethodID midInit = env->GetMethodID(cls, "<init>", "(I)V");
		jobject newObj = env->NewObject(cls, midInit, valueJava);

		env->CallObjectMethod(hashMap, put, keyJava, newObj);
		//vidi da li moze ovako za int vrednost ili i to da bude string pa da se konvertuje
		env->DeleteLocalRef(keyJava);
		env->DeleteLocalRef(newObj);
	}
	//cout << "DOVDE2";
	jobject hashMapGobal = static_cast<jobject>(env->NewGlobalRef(hashMap));
	env->DeleteLocalRef(hashMap);
	env->DeleteLocalRef(mapClass);
	//cout << "DOVDE3";
	return hashMapGobal;
}

JNIEXPORT jlong JNICALL Java_olympicgames_OlympicGames_initEvents
(JNIEnv* env, jclass, jstring filename) {
	jboolean isCopy;
	const char* filen = (*env).GetStringUTFChars(filename, &isCopy);
	env->ReleaseStringUTFChars(filename, filen);
	evParser->eventParsing(filen);
	return (jlong)evParser;
}



JNIEXPORT jlong JNICALL Java_olympicgames_OlympicGames_initAthletes
(JNIEnv* env, jclass, jstring filename) {
	jboolean isCopy;
	const char* filen = (*env).GetStringUTFChars(filename, &isCopy);
	env->ReleaseStringUTFChars(filename, filen);
	AthetesParser ath(evParser->getAthleteIds());
	ath.athletesParse(filen, *athletes);
	return (jlong)athletes;
}