#include "Homescreen.h"

Homescreen::Homescreen(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
    ui.stackedWidget->insertWidget(1, &waitingRoom);
}

Homescreen::~Homescreen()
{}

void Homescreen::on_GameHistory_pushButton_clicked()
{
    gameHistory = new GameHistory(this);
    gameHistory->show();
}

void Homescreen::SetUsername(std::string username) {
    
    this->m_playerUsername = username;
}

void Homescreen::on_play_pushButton_clicked()
{
    waitingRoom.SetUsername(m_playerUsername);

    auto response1 = cpr::Put(
        cpr::Url{ "http://localhost:18080/addtowaitingroom" },
        cpr::Payload{
            { "username", m_playerUsername}
        });
	
    waitingRoom.SetTimer();
    ui.stackedWidget->setCurrentIndex(1);

}
