#ifndef PLAYER_H
#define PLAYER_H

#include "videoaddress.h"
#include <map>

using namespace std;

class Player
{
protected:
    VideoAddress *video;

public:
    map<string,string> attrs;

    virtual void setVideo(VideoAddress *m_video){
        video = m_video;
    }

    virtual void play(){}
};


#endif // PLAYER_H
