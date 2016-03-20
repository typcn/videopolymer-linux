#include "mainwindow.h"
#include <QApplication>
#include <rpcserver.h>

#include "providers/youget.h"
#include "players/mpv.h"
#include "playerloader.h"

#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TRPCServer  *server = new TRPCServer();

    server->addHandler("/check", [](QJsonObject data){
        return "ok";
    });

    server->addHandler("/vplay/youget", [](QJsonObject data){
        QString url = data["url"].toString();
        if(!url.length()) return "error";
        map<string,string> params;
        params["url"] = url.toUtf8().constData();
        params["title"] = data["title"].toString().toUtf8().constData();
        PlayerLoader ldr;
        ldr.loadVideo(new vp_youget, params);
        ldr.show();
        ldr.exec();
        return "ok";
    });

    server->addHandler("/vplay/bilibili", [](QJsonObject data){
        QString cid = data["cid"].toString();
        if(!cid.length()) return "error";

        return "ok";
    });

//    MainWindow w;
//    w.show();

    while(true){
        a.exec();
    }
    return 0;
}
