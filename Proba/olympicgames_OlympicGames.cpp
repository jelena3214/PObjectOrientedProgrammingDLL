#include "olympicgames_OlympicGames.h"
#include "functions.h"

People* athletes = People::getInstance();
EventParser* evParser = new EventParser();

#include <unordered_map>
#include <unordered_set>

string getType(int num) {
	switch (num) {
	case 1:
		return "Team";
	case 2:
		return "Individual";
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


JNIEXPORT jobject JNICALL Java_olympicgames_OlympicGames_numOfParticipants
(JNIEnv* env, jobject, jobject athletess, jobject events, jstring sport, jint year, jint type, jint medalType) {
	
	DataManipulation dm(evParser, athletes);

	jboolean isCopy;
	const char* convSport = (*env).GetStringUTFChars(sport, &isCopy);
	string ss = convSport;
	env->ReleaseStringUTFChars(sport, convSport);

	unordered_map<string, unordered_set<int>> map_contry_competitior;

	auto c_type = (int)type;
	auto c_medal = (int)medalType;

	auto games = evParser->getGames();

	for (auto &&game : games) {

		auto competitors = *game.getCompetitors();

		auto fyear = game.getYear();

		for (auto& competitor : competitors)
		{
			auto country = competitor->getCountry()->getName();
			auto fsport = competitor->getEvent()->getSport()->getName();
			auto ftype = competitor->getEvent()->getType();
			auto fmedal = competitor->getMedal();

			if ((ss.empty() || fsport == ss) && (year == 0 || fyear == year) && (c_type < 0 || ftype == c_type) && (c_medal < 0 || fmedal == c_medal)) {
				auto id = competitor->getId();
				
				if (map_contry_competitior.find(country) == map_contry_competitior.end())
				{
					map_contry_competitior[country] = std::move(unordered_set<int>(id.begin(), id.end()));
				}
				else
				{
					map_contry_competitior[country].insert(id.begin(), id.end());
				}
			}
		}
	}

	fflush(stdout);
	cout << map_contry_competitior.size() << "MAP SIZE" << endl;
	jclass mapClass = env->FindClass("java/util/HashMap");
	jmethodID init = env->GetMethodID(mapClass, "<init>", "()V");
	jobject hashMap = env->NewObject(mapClass, init);
	jmethodID put = env->GetMethodID(mapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

	auto consIterator = map_contry_competitior.begin();
	for (; consIterator != map_contry_competitior.end(); ++consIterator) {
		jstring keyJava = env->NewStringUTF(consIterator->first.c_str());
		jint valueJava = consIterator->second.size();

		jclass cls = env->FindClass("java/lang/Integer");
		jmethodID midInit = env->GetMethodID(cls, "<init>", "(I)V");
		jobject newObj = env->NewObject(cls, midInit, valueJava);

		env->CallObjectMethod(hashMap, put, keyJava, newObj);
		env->DeleteLocalRef(keyJava);
		env->DeleteLocalRef(newObj);
	}
	
	jobject hashMapGobal = static_cast<jobject>(env->NewGlobalRef(hashMap));
	env->DeleteLocalRef(hashMap);
	env->DeleteLocalRef(mapClass);
	
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


JNIEXPORT jobject JNICALL Java_olympicgames_OlympicGames_averageHeightSeason
(JNIEnv* env, jobject, jobject, jobject, jstring sportN, jint startY, jint endY, jint type, jint medal, jstring seasonN) {
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
	
	for (int i = startYear; i <= endYear; i++) {
		Filter f(sport, "", i, eventType, medalType);
		//cout << "DOVDEEEEEE" << endl;
		returnMap.insert({ i, dm.averageAthletesHeight(f, season) });
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

JNIEXPORT jobject JNICALL Java_olympicgames_OlympicGames_averageWeightSeason
(JNIEnv* env, jobject, jobject, jobject, jstring sportN, jint startY, jint endY, jint type, jint medal, jstring seasonN) {
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

	for (int i = startYear; i <= endYear; i++) {
		Filter f(sport, "", i, eventType, medalType);
		//cout << "DOVDEEEEEE" << endl;
		returnMap.insert({ i, dm.averageAthletesWeight(f, season) });
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
