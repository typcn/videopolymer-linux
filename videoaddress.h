#ifndef VIDEOADDRESS_H
#define VIDEOADDRESS_H


#include <vector>
#include <string>

class VideoAddress
{
private:
    std::vector<std::string> urls;
    std::vector<std::vector<std::string>> backups;

    std::string generateEDL(std::vector<std::string> urls);

    int currentURLIndex;

public:
    std::string firstFragment;

    std::string nextPlayURL();

    void addURL(std::string url);
    void addBackup(std::vector<std::string> urls);
};

#endif // VIDEOADDRESS_H
