#include "rpcserver.h"
#include <QTcpSocket>
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>

TRPCServer::TRPCServer(QObject *parent) : QObject(parent){
    tcpServer = new QTcpServer(this);

listen:

    if(portNumber > 18079){
        qDebug() << "Too many failed attempt to listen , give up";
        portNumber = 0;
        return;
    }

    if(!tcpServer->listen(QHostAddress::LocalHost, portNumber))
    {
        portNumber++;
        qDebug() << "Server could not start, Trying to use port " << portNumber;
        goto listen;
    }
    else
    {
        qDebug() << "RPC Server Started on Port" << portNumber;
        connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
        isListening = true;
    }
}

int TRPCServer::getPortNumber(){
    return isListening ? portNumber : 0;
}

void TRPCServer::addHandler(QString route,HANDLER_FUNC func){
    handlers.insert(std::make_pair(route, func));
}

void TRPCServer::newConnection(){
    while (tcpServer->hasPendingConnections())
    {
        QTcpSocket *socket = tcpServer->nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    }
}

void TRPCServer::disconnected()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    socket->deleteLater();
    reqBufs.remove(socket);
}


qint32 ArrayToInt(QByteArray source)
{
    qint32 temp;
    QDataStream data(&source, QIODevice::ReadWrite);
    data >> temp;
    return temp;
}

void TRPCServer::readyRead(){
    #define CRLF "\r\n"
    #define finish socket->disconnectFromHost(); return;

    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    QByteArray buffer;

    if(!reqBufs[socket].isEmpty()){
        buffer = reqBufs[socket];
    }

    while (socket->bytesAvailable() > 0)
    {
        buffer.append(socket->readAll());
    }


    QString requestText = QString::fromUtf8(buffer.data());
    if(requestText.length() < 1) { finish } ;

    qDebug() << requestText;

    QStringList list = requestText.split("\n");

    QStringList meta = list.value(0).split(" ");
    if(meta.length() < 1) { finish };

    QString block;
    QTextStream out(&block, QIODevice::WriteOnly);

    out << "HTTP/1.1 200 OK" CRLF
        << "Access-Control-Allow-Origin: *" CRLF
        << "Connection: close" CRLF
        << "Cache-Control: max-age=0, no-cache, no-store, must-revalidate" CRLF
        << "Server: TYPCNClient" CRLF;


    QJsonObject jsonData;

    QString method = meta.value(0);
    QString url = meta.value(1);

    if(method == "OPTIONS"){
        for(int i = 0; i < list.length(); i++){
            QStringList header_field = list.value(i).split(":");
            if(header_field.length() != 2){
                continue;
            }else if(header_field.value(0) == "Access-Control-Request-Method"){
                out << "Access-Control-Allow-Method:" << header_field.value(1) << CRLF;
            }else if(header_field.value(0) == "Access-Control-Request-Headers"){
                out << "Access-Control-Allow-Headers:" << header_field.value(1) << CRLF;
            }
        }
    }else if(method == "POST"){
        int fullContentLen = 0;
        for(int i = 0; i < list.length(); i++){
            QStringList header_field = list.value(i).split(":");
            if(header_field.length() != 2){
                continue;
            }else if(header_field.value(0) == "Content-Length"){
                fullContentLen = header_field.value(1).toInt();
            }
        }
        int contentLen = requestText.length() - (requestText.indexOf("\r\n\r\n") + 4);
        QStringRef data = requestText.rightRef(contentLen);
        if(contentLen != fullContentLen){ // Some special browser will send data later
            reqBufs[socket] = buffer;
            return;
        }
        QJsonParseError error;
        QJsonDocument loadDoc = QJsonDocument::fromJson(data.toUtf8(),&error);
        if(error.error != QJsonParseError::NoError){ finish }
        jsonData = loadDoc.object();
    }
    out << CRLF;

    if(!handlers[url]) { finish }

    HANDLER_FUNC func = handlers[url];

    out << func(jsonData);

    socket->write(block.toUtf8());
    socket->disconnectFromHost();
}
