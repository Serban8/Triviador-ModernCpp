#include "Homescreen.h"

Homescreen::Homescreen(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

Homescreen::~Homescreen()
{}
void Homescreen::setUsername(std::string username)
{
    this->username = username;
}
void Homescreen::on_GameHistoryButton_clicked()
{
    gameHistory = new GameHistory(this);
    gameHistory->show();
    this->hide();
}
void Homescreen::on_playButton_clicked() 
{
    auto response1 = cpr::Put(
        cpr::Url{ "http://localhost:18080/addtowaitingroom" },
        cpr::Payload{
            { "username", username }
        });
    waitingRoom = new WaitingRoom(this);
    waitingRoom->show();
    this->hide();
}
