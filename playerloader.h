#ifndef PLAYERLOADER_H
#define PLAYERLOADER_H

#include <QDialog>
#include <QDesktopWidget>
#include "providers/videoprovider.h"

namespace Ui {
class PlayerLoader;
}

class PlayerLoader : public QDialog
{
    Q_OBJECT

public:
    explicit PlayerLoader(QWidget *parent = 0);
    ~PlayerLoader();

signals:
    void onGenerateParam(VideoProvider *provider,map<string,string> params);
    void onVideoAddress(VideoAddress *video,map<string,string> attrs);
    void onError(string text);

public slots:
    void loadVideo(VideoProvider *provider,string url);
    void loadVideo(VideoProvider *provider,map<string,string> params);
    void _startPlayer(VideoAddress *video,map<string,string> attrs);

private:
    Ui::PlayerLoader *ui;
    QDesktopWidget *desktop;

private slots:
    void showError(string text);
};

#endif // PLAYERLOADER_H
