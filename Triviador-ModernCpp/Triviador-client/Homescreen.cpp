#include "Homescreen.h"

Homescreen::Homescreen(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
    ui.stackedWidget->insertWidget(1, &waitingRoom);
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
}
void Homescreen::on_playButton_clicked() 
{
    auto response1 = cpr::Put(
        cpr::Url{ "http://localhost:18080/addtowaitingroom" },
        cpr::Payload{
            { "username", username }
        });
    ui.stackedWidget->setCurrentIndex(1);
}
