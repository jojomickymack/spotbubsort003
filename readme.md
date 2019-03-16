# Cool Android Ndk Spotify Api App

This was an experiment to play with the android ndk and try out json parsing using c++. Do you need to use the ndk to achive something like 
this? Absolutely not! This was really just for fun and helped me understand what the android ndk does in a fun way.

Keep in mind that if you would like to use this app, you'll need to add the client id and client secret from your own spotify account. You 
will find where those credentials go when you open the MainActivity in the app.

So many things can go wrong with an ndk project! Basically, what's happening here is that there are hooks in the MainActivity to use the 
'JNI', or Java Native Interface, to c++ functions that are in file in src/main/cpp. There is a CMakeLists.txt file in the src/main/cpp folder 
that tells the ndk how to build the 'native library' - it says that there is an include folder. Inside of the include folder is the amazing 
[nlohmann json library](https://github.com/nlohmann/json), which is a single hpp file! This is used extensively in the cpp sourcecode.

When this is built or run (there's a separate task for each - technically the cpp file is compiled in the build task) the cpp code is compiled 
into a .so file using cmake. The .so file is included in the apk and is linked against when the application is run. Your application can 
execute the compiled c++ code using that 'native library'.

I built out various methods to hit different spotify endpoints and commented them out. To change what the app does, uncomment the different 
sections and rebuild.

One thing that changed since the last time I ran this (different ndk version), is that the compiler now needs types to be more explicit when 
they come from a json object = I had to add .get<string>() in a few places, as shown below.

```cpp
string name = cjson[i]["name"].get<string>();
```

to get numbers out of a json object, use a static cast.

```cpp
int popularity = static_cast<int>(cjson[i]["popularity"]);
```

Note that this was updated from a new Android Studio ndk template that also capable of interpreting kotlin.
