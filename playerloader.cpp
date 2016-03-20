#include "playerloader.h"
#include "ui_playerloader.h"
#include "3rd-party/waitingspinnerwidget.h"

#include "players/mpv.h"

#include <QMessageBox>
#include <thread>

PlayerLoader::PlayerLoader(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlayerLoader)
{
    ui->setupUi(this);
    desktop = QApplication::desktop();
    this->setWindowFlags(this->windowFlags() |  Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    qRegisterMetaType< map<string,string> >( "map<string,string>" );
    qRegisterMetaType< string >( "string" );

    connect(this, SIGNAL(onGenerateParam(VideoProvider*,map<string,string>)), SLOT(loadVideo(VideoProvider *,map<string,string>)));
    connect(this, SIGNAL(onVideoAddress(VideoAddress*,map<string,string>)), SLOT(_startPlayer(VideoAddress*,map<string,string>)));
    connect(this, SIGNAL(onError(string)), SLOT(showError(string))  );

    WaitingSpinnerWidget* spinner = new WaitingSpinnerWidget(ui->spinnerWidget);
    spinner->setRoundness(12.0);
    spinner->setNumberOfLines(16);
    spinner->setLineLength(15);
    spinner->setLineWidth(2);
    spinner->setInnerRadius(10);
    spinner->setRevolutionsPerSecond(1);
    spinner->setColor(QApplication::palette().color(QPalette::WindowText));
    spinner->start();
}

void PlayerLoader::loadVideo(VideoProvider *provider,string url){
    ui->label->setText("Building request");
    std::thread t([provider,url,this]{
        try{
            auto params = provider->generateParamsFromURL(url);
            if(!params.size()){
                throw "Can't generate params";
            }

            emit onGenerateParam(provider,params);
        }catch(const char *e){
            emit onError(e);
        }
    });
    t.detach();
}

void PlayerLoader::loadVideo(VideoProvider *provider,map<string,string> params){
    ui->label->setText("Resolving Video Address");
    std::thread t([provider,params,this]{
        try{
            VideoAddress *video = provider->getVideoAddress(params);
            if(!video){
                throw "Empty response";
            }
            emit onVideoAddress(video,params);
        }catch(const char *e){
            emit onError(e);
        }
    });
    t.detach();
}

void PlayerLoader::_startPlayer(VideoAddress *video,map<string,string> attrs){
    ui->label->setText("Starting Player");
    mpv *player_mpv = new mpv();
    player_mpv->attrs = attrs;
    player_mpv->setVideo(video);
    player_mpv->play();
    QTimer::singleShot(2000, this, SLOT(close()));
}

void PlayerLoader::showError(string text){
    QMessageBox Msgbox;
    Msgbox.setText(QString::fromStdString(text));
    Msgbox.exec();
    this->close();
}

PlayerLoader::~PlayerLoader()
{
    delete ui;
}
