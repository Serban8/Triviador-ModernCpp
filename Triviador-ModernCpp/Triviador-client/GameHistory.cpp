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
}

GameHistory::~GameHistory()
{}
