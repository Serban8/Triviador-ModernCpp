#include "Homescreen.h"

Homescreen::Homescreen(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
    ui.stackedWidget->insertWidget(1, &waitingRoom);
}

Homescreen::~Homescreen()
{}
//void Homescreen::SetUsername(std::string username)
//{
//    this->m_username = username;
//}
void Homescreen::on_GameHistory_pushButton_clicked()
{
    gameHistory = new GameHistory(this);
    gameHistory->show();
}
void Homescreen::on_play_pushButton_clicked()
{
    auto response1 = cpr::Put(
        cpr::Url{ "http://localhost:18080/addtowaitingroom" },
        cpr::Payload{
            { "username", "Gigi"}
        });
    ui.stackedWidget->setCurrentIndex(1);
}
