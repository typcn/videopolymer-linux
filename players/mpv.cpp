#include "mpv.h"
#include <QProcess>
#include <QString>

void mpv::play(){
    QProcess p;
    QStringList args;
    args << "--no-terminal";
    if(attrs.count("title")){
        args << "--force-media-title=" + QString::fromStdString(attrs["title"]);
    }
    args << QString::fromStdString(video->nextPlayURL());
    p.startDetached("mpv", args);
}
