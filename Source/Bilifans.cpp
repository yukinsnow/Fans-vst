/*
  ==============================================================================

    @yukinsnow
    Sodatune.com

  ==============================================================================
*/
#define CURL_STATICLIB
#include <iostream>
#include <memory>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>

//Here is the setting for static libary build on windows. MacOS no need.
#pragma comment(lib, "Ws2_32.Lib")
#pragma comment(lib, "Wldap32.Lib")
#pragma comment(lib, "Crypt32.Lib")

#pragma comment(lib, "zlib.Lib")
#pragma comment(lib, "libcurl.lib")


using std::cin;
using std::cout;
using std::string;
using std::to_string;
using std::endl;
using std::tuple;
using json = nlohmann::json;

size_t WriteCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    string& buf = *static_cast<string*>(userdata);
    buf.append(ptr, std::next(ptr, nmemb * size));
    return nmemb * size;
}

size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

string name;
int fans;
string face;

void curlfunc(int mid)
{

    string url("https://api.bilibili.com/x/web-interface/card?mid=");
    url.append(to_string(mid));
    string faceurl = "";

    CURL* curl = curl_easy_init();

    // Set remote URL.
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "BiLiBiLi WP Client/1.0.0 (orz@loli.my)");

    // Don't wait forever, time out after 10 seconds.
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);

    // Follow HTTP redirects if necessary.
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);


    //Set HTTP proxy
    curl_easy_setopt(curl, CURLOPT_PROXY, "http://127.0.0.1:7890"); 

    // Response information.
    long httpCode(0);
    string readBuffer;

    // Hook up data handling function.
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

    // Save data stream to the buffer.
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    // Run our HTTP GET command, capture the HTTP response code, and clean up.
    curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup(curl);

    if (httpCode == 200)
    {
        json data = json::parse(readBuffer);
        name = data["data"]["card"]["name"].get<string>();
        fans = data["data"]["card"]["fans"].get<int>();
        face = data["data"]["card"]["face"].get<string>();
        //cout << name << endl;
        //cout << fans << endl;
        //cout << face << endl;
    }
    else
    {
        cout << "Couldn't GET from " << url << " - exiting" << endl;
        //return 1;
    }

}

//get name,fans,face in the same time
tuple<string, int, string> Bilifans(int mid)
{
    tuple<string, int, string> result;
    //Bug fixed: curl will crash with input 0.
    if (mid == 0) {
        mid = 36081646;
    }
    curlfunc(mid);
    return make_tuple(name, fans, face);
}
