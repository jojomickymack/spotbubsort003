#include <jni.h>
#include <string>
#include <sstream>
#include <json.hpp>

using namespace std;
using json = nlohmann::json;

/*
 * Below - cpp functions to be called internally
 */

string ConvertJString(JNIEnv* env, jstring str) {
    const jsize len = env->GetStringUTFLength(str);
    const char* strChars = env->GetStringUTFChars(str, (jboolean*)0);
    std::string Result(strChars, len);

    env->ReleaseStringUTFChars(str, strChars);
    return Result;
}

jobject cIntReturnJavaInteger(JNIEnv* env, int cInt) {
    jclass integerClass = env->FindClass("java/lang/Integer");
    if (integerClass == NULL) return NULL;
    jint number = cInt;
    jmethodID methodID = env->GetMethodID(integerClass, "<init>", "(I)V");
    jobject integer = env->NewObject(integerClass, methodID, number);
    return integer;
}

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

extern "C"

jstring Java_central_com_MainActivity_parseDumpJsonJNI(JNIEnv *env, jobject /* this */, jstring myJsonJstring) {
    string cstring = ConvertJString(env, myJsonJstring);
    json cjson = json::parse(cstring);
    string output = cjson.dump(2);
    return env->NewStringUTF(output.c_str());
}

extern "C"

jstring Java_central_com_MainActivity_parseTokenJsonGetTokenJNI(JNIEnv *env, jobject /* this */, jstring myJsonJstring) {
    string cstring = ConvertJString(env, myJsonJstring);
    json cjson = json::parse(cstring);
    string output = "";
    int keyCount = static_cast<int>(cjson.count("access_token"));
    if (keyCount > 0) output = cjson.at("access_token").get<string>();
    return env->NewStringUTF(output.c_str());
}

/**
 *  just returns the first artist id in the list or items
 */

extern "C"

jstring Java_central_com_MainActivity_parseSearchJsonGetArtistIdJNI(JNIEnv *env, jobject /* this */, jstring myJsonJstring) {
    string cstring = ConvertJString(env, myJsonJstring);
    json cjson = json::parse(cstring);
    string output = "";
    int artistCount = static_cast<int>(cjson.count("artists"));
    if (artistCount > 0) if (cjson["artists"]["items"].size() > 0) output = cjson["artists"]["items"][0]["id"];
    return env->NewStringUTF(output.c_str());
}

/**
 *  just returns the first album id in the list
 */

extern "C"

jstring Java_central_com_MainActivity_parseArtistAlbumsJsonGetAlbumIdJNI(JNIEnv *env, jobject /* this */, jstring myJsonJstring) {
    string cstring = ConvertJString(env, myJsonJstring);
    json cjson = json::parse(cstring);
    string output = "";
    int itemCount = static_cast<int>(cjson.count("items"));
    if (itemCount > 0) if (cjson["items"].size() > 0) output = cjson["items"][0]["id"];
    return env->NewStringUTF(output.c_str());
}

/**
 *  just returns the first track id in the list
 */

extern "C"

jstring Java_central_com_MainActivity_parseAlbumTracksJsonGetTrackIdJNI(JNIEnv *env, jobject /* this */, jstring myJsonJstring) {
    string cstring = ConvertJString(env, myJsonJstring);
    json cjson = json::parse(cstring);
    string output = "";
    int itemCount = static_cast<int>(cjson.count("items"));
    if (itemCount > 0) if (cjson["items"].size() > 0) output = cjson["items"][0]["id"];
    return env->NewStringUTF(output.c_str());
}

extern "C"

jstring Java_central_com_MainActivity_mapsJNI(JNIEnv *env, jobject /* this */) {
    map<string, string> myMap = {{"key1", "value1"}, {"key2", "value2"}, {"key3", "value3"}, {"key4", "value4"}};
    string output = myMap["key3"];
    return env->NewStringUTF(output.c_str());
}

extern "C"
JNIEXPORT jobject
JNICALL

Java_central_com_MainActivity_returnLinkedHashMapJNI(JNIEnv *env, jobject /* this */) {
    map<string, string> myMap = {{"key1", "super_c_val"}, {"key2", "value2"}, {"key3", "value3"}, {"key4", "value4"}};
    return getJavaLinkedHashMapStringsFromCppMap(env, myMap);
}

extern "C"
JNIEXPORT jobject
JNICALL

Java_central_com_MainActivity_returnIntLinkedHashMapJNI(JNIEnv *env, jobject /* this */) {
    map<string, int> myMap = {{"key1", 52}, {"key2", 12}, {"key3", 19}, {"key4", 82}};
    return getJavaLinkedHashMapFromCppMap(env, myMap);
}

// this one is not working because the recommendation endpoint doesn't always include popularity
//extern "C"
////JNIEXPORT jobject
////JNICALL
//
//jstring Java_central_com_MainActivity_recommendationJsonToLinkedHashMapJNI(JNIEnv *env, jobject /* this */, jstring myJsonJstring) {
//    string cstring = ConvertJString(env, myJsonJstring);
//    json cjson = json::parse(cstring)
//    string output = "";
//    cjson = cjson["tracks"];
//    int albumCount = (int)cjson.size();
//    output = to_string(albumCount);
//
//// for some reason a lot of the recommended albums don't include a popularity, so trying to access it will crash the app
//
//    for (int i = 0; i < (int)cjson.size(); i++) {
//        output += cjson[i]["album"]["artists"][0]["name"];
//        output += " - ";
//        output += cjson[i]["album"]["name"];
//        output += " - ";
//        int popularity = static_cast<int>(cjson[i]["album"]["popularity"]);
//        output += to_string(popularity);
//        output += "\n";
//    }
//
//    //map<string, string> myMap = {{"key1", "super_c_val"}, {"key2", "value2"}, {"key3", "value3"}, {"key4", "value4"}};
//    //return getJavaLinkedHashMapStringsFromCppMap(env, myMap);
//
//    return env->NewStringUTF(output.c_str());
//}

extern "C"
JNIEXPORT jobject
JNICALL

Java_central_com_MainActivity_relatedArtistsJsonToLinkedHashMapJNI(JNIEnv *env, jobject /* this */, jstring myJsonJstring, bool ascending) {
    string cstring = ConvertJString(env, myJsonJstring);
    json cjson = json::parse(cstring);
    string output = "";
    vector<pair<string, int>> myArtistsVector = {};
    cjson = cjson["artists"];

    for (int i = 0; i < (int)cjson.size(); i++) {
        output += cjson[i]["name"];
        output += " - ";
        string name = cjson[i]["name"];
        int popularity = static_cast<int>(cjson[i]["popularity"]);
        output += to_string(popularity);
        output += "\n";
        pair<string, int> myPair = {name, popularity};
        myArtistsVector.push_back(myPair);
    }

    return getJavaLinkedHashMapFromCppVector(env, bubbleSortArtistsByPopularity(myArtistsVector, ascending));
}

/**
 *  collects the number of followers
 */

extern "C"

jstring Java_central_com_MainActivity_parseSearchJsonGetFollowersJNI(JNIEnv *env, jobject /* this */, jstring myJsonJstring) {
    string cstring = ConvertJString(env, myJsonJstring);
    json cjson = json::parse(cstring);
    string output = "";
    int artistCount = cjson.count("artists");

    //output = cjson["artists"]["items"][0]["type"];

    if (artistCount > 0) {
        int followers = static_cast<int>(cjson["artists"]["items"][0]["followers"]["total"]);
        output = to_string(followers);
    }

    return env->NewStringUTF(output.c_str());
}