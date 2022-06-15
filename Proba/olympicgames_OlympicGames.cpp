#include "olympicgames_OlympicGames.h"
#include "functions.h"

People* athletes = People::getInstance();
EventParser* evParser = new EventParser();

string getType(int num) {
	switch (num) {
	case 1:
		return "Individal";
	case 2:
		return "Team";
	default:
		return "";
	}
}

string getMedalType(int num) {
	switch (num) {
	case 1:
		return "Gold";
	case 2:
		return "Silver";
	case 3:
		return "Bronze";
	case 4:
		return "NA";
	default:
		return "";
	}
}

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
	env->ReleaseStringUTFChars(sport, convSport);
	string stype, smedal;
	stype = getType((int)type);
	smedal = getMedalType((int)medalType);

	int all = 0;
	for (const shared_ptr<Country>& cnt : countries) {
		Filter f(ss, cnt->getName(), (int)year, stype, smedal);
		int num = dm.numberOfPlayers(f, cnt->getName());
		if (num) {
			returnMap.insert({ cnt->getName(), num });
			all += num;
		}
	}

	cout << "BROJJ " << all << endl;
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


JNIEXPORT jobject JNICALL Java_olympicgames_OlympicGames_numOfDisciplinesSeason
(JNIEnv* env, jobject, jobject ath, jobject events, jstring sportN, jint startY, jint endY, jint type, jint medal, jstring seasonN) {
	DataManipulation dm(evParser, athletes);
	jboolean isCopy;
	const char* convSport = (*env).GetStringUTFChars(sportN, &isCopy);
	string sport = convSport;
	env->ReleaseStringUTFChars(sportN, convSport);

	const char* convSeason = (*env).GetStringUTFChars(seasonN, &isCopy);
	string season = convSeason;
	env->ReleaseStringUTFChars(seasonN, convSeason);

	int startYear = (int)startY;
	int endYear = (int)endY;
	string eventType = getType((int)type);
	string medalType = getMedalType((int)medal);


	map<int, int> returnMap;
	cout << "DOVDE" << endl;
	for (int i = startYear; i <= endYear; i++) {
		Filter f(sport, "", i, eventType, medalType);
		//cout << "DOVDEEEEEE" << endl;
		returnMap.insert({ i, dm.numOfDisciplines(f, season) });
	}
	
	cout << returnMap.size() << "MAP SIZE" << endl;
	jclass mapClass = env->FindClass("java/util/HashMap");
	jmethodID init = env->GetMethodID(mapClass, "<init>", "()V");
	jobject hashMap = env->NewObject(mapClass, init);
	jmethodID put = env->GetMethodID(mapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

	auto consIterator = returnMap.begin();
	for (; consIterator != returnMap.end(); ++consIterator) {
		jint keyJava = consIterator->first;
		jint valueJava = consIterator->second;

		jclass cls1 = env->FindClass("java/lang/Integer");
		jmethodID midInit1 = env->GetMethodID(cls1, "<init>", "(I)V");
		jobject newObj1 = env->NewObject(cls1, midInit1, keyJava);

		jclass cls2 = env->FindClass("java/lang/Integer");
		jmethodID midInit2 = env->GetMethodID(cls2, "<init>", "(I)V");
		jobject newObj2 = env->NewObject(cls2, midInit2, valueJava);

		env->CallObjectMethod(hashMap, put, newObj1, newObj2);
		env->DeleteLocalRef(newObj1);
		env->DeleteLocalRef(newObj2);
	}
	jobject hashMapGobal = static_cast<jobject>(env->NewGlobalRef(hashMap));
	env->DeleteLocalRef(hashMap);
	env->DeleteLocalRef(mapClass);
	return hashMapGobal;
}