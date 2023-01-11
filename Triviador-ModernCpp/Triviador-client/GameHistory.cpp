#include "GameHistory.h"

GameHistory::GameHistory(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
    std::stringstream urlBuilder;
    urlBuilder << "http://localhost:18080/player/" << "Flo";
    cpr::Response response = cpr::Get(cpr::Url{ urlBuilder.str() });

    auto games = crow::json::load(response.text);
    if (games.size() != 0)
    {
        for (const auto& game : games) 
        {
            ui.listWidget->addItem(QString::fromStdString(game["date"].s())+" "+QString::fromStdString(game["rounds"].s()) + " " + QString::fromStdString(game["winner"].s()));
        }
    }
    else
        ui.listWidget->addItem("No games played");

    ui.close_pushButton->move(50, 50);
    ui.listWidget->move(this->width() / 2 - ui.listWidget->geometry().width() / 2, this->height() / 2 - ui.listWidget->geometry().height() / 2);

}

GameHistory::~GameHistory()
{}

void GameHistory::on_close_pushButton_clicked() {

    this->close();

}
