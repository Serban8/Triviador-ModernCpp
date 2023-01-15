#include "GameHistory.h"

GameHistory::GameHistory(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
    std::stringstream urlBuilder;
    urlBuilder << "http://localhost:18080/player/" << "p1";
    cpr::Response response = cpr::Get(cpr::Url{ urlBuilder.str() });
    ui.listWidget->setWordWrap(true);
    auto games = crow::json::load(response.text);
    if (games.size() != 0)
    {
        for (const auto& game : games)
        { 
            std::string stringData = game["date"].s();
            QString message;
            message.append("Date:");
            message.append(QString::fromUtf8(stringData.c_str()));
            message.append(";");
            message.append("Won in ");
            stringData = game["rounds"].s();
            message.append(QString::fromUtf8(stringData.c_str()));
            message.append("rounds; ");
            /*stringData = game["winner"].s();
            message.append(QString::fromUtf8(stringData.c_str()));*/
            ui.listWidget->addItem(message);
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
