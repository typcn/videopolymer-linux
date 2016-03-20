#ifndef VIDEOPROVIDER_H
#define VIDEOPROVIDER_H

#include "videoaddress.h"
#include <map>
#include <iostream>
using namespace std;

class VideoProvider
{
public:
    virtual map<string,string> generateParamsFromURL(string url){
        map<string,string> params;
        params["url"] = url;
        return params;
    }

    virtual VideoAddress *getVideoAddress(map<string,string> params){
        cout << "Unkown video provider for url:" << params["url"] << endl;
        return nullptr;
    }
};

#endif // VIDEOPROVIDER_H
