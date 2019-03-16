#include <jni.h>
#include <string>
#include <sstream>
#include <json.hpp>

using namespace std;
using json = nlohmann::json;


/*
 * Below - cpp functions to be called internally
 */

/*
 * transforms jstring into format cpp can deal with
 */

string ConvertJString(JNIEnv* env, jstring str) {
    const jsize len = env->GetStringUTFLength(str);
    const char* strChars = env->GetStringUTFChars(str, (jboolean*)0);
    string result(strChars, len);

    env->ReleaseStringUTFChars(str, strChars);
    return result;
}

/*
 * transforms a c int primitive type into java Integer object - required when storing an integer in a HashMap
 */

jobject cIntReturnJavaInteger(JNIEnv* env, int cInt) {
    jclass integerClass = env->FindClass("java/lang/Integer");
    if (integerClass == NULL) return NULL;
    jint number = cInt;
    jmethodID methodID = env->GetMethodID(integerClass, "<init>", "(I)V");
    jobject integer = env->NewObject(integerClass, methodID, number);
    return integer;
}

/*
 * transforms a cpp map of string, string pairs into java LinkedHashMap of String, String
 * this is for general key value use, might be practical but I was worried about casting numbers to and from strings
 */

jobject getJavaLinkedHashMapStringsFromCppMap(JNIEnv *env, map<string, string> myMap) {
    jclass mapClass = env->FindClass("java/util/LinkedHashMap");
    if (mapClass == NULL) return NULL;
    jsize map_len = (int)myMap.size();
    jmethodID init = env->GetMethodID(mapClass, "<init>", "(I)V");
    jobject hashMap = env->NewObject(mapClass, init, map_len);
    jmethodID put = env->GetMethodID(mapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

    for (pair<string, string> myPair : myMap) {
        env->CallObjectMethod(hashMap, put, env->NewStringUTF(myPair.first.c_str()), env->NewStringUTF(myPair.second.c_str()));
    }

    return hashMap;
}

/*
 * transforms a cpp map of string, int pairs into java LinkedHashMap of String, Integer
 * - I set this one up before realizing I needed a vector since a cmap can't be accessed by index and a vector can
 */

jobject getJavaLinkedHashMapFromCppMap(JNIEnv *env, map<string, int> myMap) {
    jclass mapClass = env->FindClass("java/util/LinkedHashMap");
    if (mapClass == NULL) return NULL;
    jsize map_len = (int)myMap.size();
    jmethodID init = env->GetMethodID(mapClass, "<init>", "(I)V");
    jobject hashMap = env->NewObject(mapClass, init, map_len);
    jmethodID put = env->GetMethodID(mapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

    for (pair<string, int> myPair : myMap) {
        env->CallObjectMethod(hashMap, put, env->NewStringUTF(myPair.first.c_str()), cIntReturnJavaInteger(env, myPair.second));
    }

    return hashMap;
}

/*
 * transforms a cpp vector of string, int pairs into java LinkedHashMap of String, Integer
 */

jobject getJavaLinkedHashMapFromCppVector(JNIEnv *env, vector<pair<string, int>> myVector) {
    jclass mapClass = env->FindClass("java/util/LinkedHashMap");
    if (mapClass == NULL) return NULL;
    jsize vec_len = (int)myVector.size();
    jmethodID init = env->GetMethodID(mapClass, "<init>", "(I)V");
    jobject hashMap = env->NewObject(mapClass, init, vec_len);
    jmethodID put = env->GetMethodID(mapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

    for (pair<string, int> myMap : myVector) {
        env->CallObjectMethod(hashMap, put, env->NewStringUTF(myMap.first.c_str()), cIntReturnJavaInteger(env, myMap.second));
    }

    return hashMap;
}

/*
 * this is the bubblesort sorting algorithm
 */

vector<pair<string, int>> bubbleSortArtistsByPopularity(vector<pair<string, int>> myArtistsVec, bool ascending) {
    int size = static_cast<int>(myArtistsVec.size());

    int i;
    pair<string, int> temp = {};
    bool more;
    bool swapped;

    do {
        swapped = false;
        for (i = 0; i < (size - 1); i++) {
            more = ascending ? myArtistsVec[i].second > myArtistsVec[i + 1].second : myArtistsVec[i].second < myArtistsVec[i + 1].second;
            if (more) {
                swapped = true;
                temp = myArtistsVec[i];
                myArtistsVec[i] = myArtistsVec[i + 1];
                myArtistsVec[i + 1] = temp;
            }
        }
    } while (swapped == true);
    return myArtistsVec;
};


/*
 * Below - JNI methods to be called from java
 */

/**
 *  this just converts the jstring to something cpp can understand, parses the json, then returns it as a java string again
 */

extern "C"

jstring Java_org_central_MainActivity_parseDumpJsonJNI(JNIEnv *env, jobject /* this */, jstring myJsonJstring) {
    string cstring = ConvertJString(env, myJsonJstring);
    json cjson = json::parse(cstring);
    string output = cjson.dump(2);
    return env->NewStringUTF(output.c_str());
}

/**
 *  this is what gets the token from the authentication response
 *  myJsonJstring comes from GetToken response
 */

extern "C"

jstring Java_org_central_MainActivity_parseTokenJsonGetTokenJNI(JNIEnv *env, jobject /* this */, jstring myJsonJstring) {
    string cstring = ConvertJString(env, myJsonJstring);
    json cjson = json::parse(cstring);
    string output = "";
    int keyCount = static_cast<int>(cjson.count("access_token"));
    if (keyCount > 0) output = cjson.at("access_token").get<string>();
    return env->NewStringUTF(output.c_str());
}

/**
 *  just returns the first artist id in the list
 *  myJsonJstring comes from SearchArtist response
 */

extern "C"

jstring Java_org_central_MainActivity_parseSearchJsonGetArtistIdJNI(JNIEnv *env, jobject /* this */, jstring myJsonJstring) {
    string cstring = ConvertJString(env, myJsonJstring);
    json cjson = json::parse(cstring);
    string output = "";
    int artistCount = static_cast<int>(cjson.count("artists"));
    if (artistCount > 0) if (cjson["artists"]["items"].size() > 0) output = cjson["artists"]["items"][0]["id"];
    return env->NewStringUTF(output.c_str());
}

/**
 *  just returns the first album id in the list
 *  myJsonJstring comes from ArtistAlbums response
 */

extern "C"

jstring Java_org_central_MainActivity_parseArtistAlbumsJsonGetAlbumIdJNI(JNIEnv *env, jobject /* this */, jstring myJsonJstring) {
    string cstring = ConvertJString(env, myJsonJstring);
    json cjson = json::parse(cstring);
    string output = "";
    int itemCount = static_cast<int>(cjson.count("items"));
    if (itemCount > 0) if (cjson["items"].size() > 0) output = cjson["items"][0]["id"];
    return env->NewStringUTF(output.c_str());
}

/**
 *  just returns the first track id in the list
 *  myJsonJstring comes from GetTrack response
 */

extern "C"

jstring Java_org_central_MainActivity_parseAlbumTracksJsonGetTrackIdJNI(JNIEnv *env, jobject /* this */, jstring myJsonJstring) {
    string cstring = ConvertJString(env, myJsonJstring);
    json cjson = json::parse(cstring);
    string output = "";
    int itemCount = static_cast<int>(cjson.count("items"));
    if (itemCount > 0) if (cjson["items"].size() > 0) output = cjson["items"][0]["id"];
    return env->NewStringUTF(output.c_str());
}

/*
 * this function is just for testing the getJavaLinkedHashMapStringsFromCppMap function
 */

extern "C"
JNIEXPORT jobject
JNICALL

Java_org_central_MainActivity_returnLinkedHashMapJNI(JNIEnv *env, jobject /* this */) {
    map<string, string> myMap = {{"key1", "super_c_val"}, {"key2", "value2"}, {"key3", "value3"}, {"key4", "value4"}};
    return getJavaLinkedHashMapStringsFromCppMap(env, myMap);
}

/*
 * this function is just for testing the getJavaLinkedHashMapFromCppMap function
 */

extern "C"
JNIEXPORT jobject
JNICALL

Java_org_central_MainActivity_returnIntLinkedHashMapJNI(JNIEnv *env, jobject /* this */) {
    map<string, int> myMap = {{"key1", 52}, {"key2", 12}, {"key3", 19}, {"key4", 82}};
    return getJavaLinkedHashMapFromCppMap(env, myMap);
}

/**
 *  this is the one that's actually called - popularity is consistently part of the ArtistSimilarArtists response (I hope?)
 */

extern "C"
JNIEXPORT jobject JNICALL
Java_org_central_MainActivity_relatedArtistsJsonToLinkedHashMapJNI(JNIEnv *env, jobject instance, jstring myJson_, jboolean myAsc) {
    string cstring = ConvertJString(env, myJson_);
    json cjson = json::parse(cstring);
    string output = "";
    vector<pair<string, int>> myArtistsVector = {};
    cjson = cjson["artists"];

    for (int i = 0; i < (int)cjson.size(); i++) {
        output += cjson[i]["name"].get<string>();
        output += " - ";
        string name = cjson[i]["name"].get<string>();
        int popularity = static_cast<int>(cjson[i]["popularity"]);
        output += to_string(popularity);
        output += "\n";
        pair<string, int> myPair = {name, popularity};
        myArtistsVector.push_back(myPair);
    }

    return getJavaLinkedHashMapFromCppVector(env, bubbleSortArtistsByPopularity(myArtistsVector, myAsc));
}

/**
 *  collects the number of followers from the first item in the list (not used)
 *  myJsonJstring is the ArtistSearch response
 */

extern "C"

jstring Java_org_central_MainActivity_parseSearchJsonGetFollowersJNI(JNIEnv *env, jobject /* this */, jstring myJsonJstring) {
    string cstring = ConvertJString(env, myJsonJstring);
    json cjson = json::parse(cstring);
    string output = "";
    int artistCount = cjson.count("artists");

    if (artistCount > 0) {
        int followers = static_cast<int>(cjson["artists"]["items"][0]["followers"]["total"]);
        output = to_string(followers);
    }

    return env->NewStringUTF(output.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_org_central_MainActivity_getStringJNI(JNIEnv *env, jobject instance) {
    // this is the 'hello world' function
    return env->NewStringUTF("testing");
}