#ifndef RPCSERVER_H
#define RPCSERVER_H

#include <QTcpServer>
#include <QObject>
#include <QJsonObject>
#include <functional>
#include <map>

// This is an simple http server for RPC call , It cannot processing big data

class TRPCServer : public QObject
{
    Q_OBJECT

    #define HANDLER_FUNC const std::function<QString (QJsonObject data)>

public:
    explicit TRPCServer(QObject *parent = 0);

public slots:
    int getPortNumber();
    void addHandler(QString url,HANDLER_FUNC func);

private slots:
    void newConnection();
    void readyRead();
    void disconnected();

private:
    QTcpServer *tcpServer;
    QHash<QTcpSocket*, QByteArray> reqBufs;
    std::map<QString, HANDLER_FUNC> handlers;
    int portNumber = 18069;
    bool isListening = false;
};

#endif // RPCSERVER_H
