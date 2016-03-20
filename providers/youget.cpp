#include "youget.h"
#include <QProcess>
#include <QDebug>
#include <QUrl>

VideoAddress *vp_youget::getVideoAddress(map<string,string> params){
    if(!params["url"].length()){
        throw "URL cannot be empty";
        return nullptr;
    }
    qDebug() << "Starting you-get";
    QProcess p;
    QStringList args;
    args << "-u" << QString::fromStdString(params["url"]);
    p.start("you-get", args);
    p.waitForFinished(5000);
    QString stdout = p.readAllStandardError() + p.readAllStandardOutput();
    qDebug() << "you-get result:" << stdout;

    if(!stdout.length()){
        p.terminate();
        throw "You-Get timeout or empty output";
    }else if(!stdout.contains("Real URLs:\n")){
        throw stdout.toUtf8().constData();
    }

    QString urls_str = stdout.split("Real URLs:\n")[1];

    QStringList urls = urls_str.split("\n");

    VideoAddress *video = new VideoAddress();

    for (QStringList::iterator it = urls.begin();it != urls.end(); ++it) {
        QString urlstr = *it;
        if(!urlstr.length()) continue;
        QUrl url = QUrl::fromUserInput(urlstr);
        if(url.isValid()){
            video->addURL(urlstr.toUtf8().constData());
        }else{
            qDebug() << "Ignore invalid url: " << urlstr;
        }
    }

    return video;
}
