#ifndef YOUGET_H
#define YOUGET_H
#include "videoprovider.h"

class vp_youget : public VideoProvider
{
public:
    virtual VideoAddress *getVideoAddress(map<string,string> params);
};

#endif // YOUGET_H
