#include "mainwindow.h"
#include <QApplication>
#include <rpcserver.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TRPCServer  *server = new TRPCServer();

    server->addHandler("/vplay/youget", [](QJsonObject data){
        QString url = data["url"].toString();
        if(!url.length()) return "";

        return "{\"code\":0}";
    });

    server->addHandler("/vplay/bilibili", [](QJsonObject data){
        QString cid = data["cid"].toString();
        if(!cid.length()) return "";

        return "{\"code\":0}";
    });

    MainWindow w;
    w.show();
    return a.exec();
}
