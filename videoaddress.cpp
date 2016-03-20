#include "videoaddress.h"
#include <sstream>

std::string VideoAddress::nextPlayURL()
{
    if(!currentURLIndex){
        currentURLIndex = 1;
        return generateEDL(urls);
    }else{
        int backupIdx = currentURLIndex - 1;
        int backupCount = backups.size();
        if(backupCount > backupIdx){
            auto urls = backups[backupIdx];
            firstFragment = urls[0];
            return generateEDL(urls);
        }else{
            return NULL;
        }
    }
}

void VideoAddress::addURL(std::string url){
    urls.push_back(url);
}

void VideoAddress::addBackup(std::vector<std::string> urls){
    backups.push_back(urls);
}

std::string VideoAddress::generateEDL(std::vector<std::string> urls){
    if(urls.size() == 1){ return urls[0]; }
    std::stringstream stream;
    stream << "edl://";
    for(std::string url : urls){
        size_t len = url.length();
        stream << "%" << len << "%" << url << ";";
    }
    return stream.str();
}
