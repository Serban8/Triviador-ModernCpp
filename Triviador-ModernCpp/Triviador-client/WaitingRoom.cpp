#include "WaitingRoom.h"

WaitingRoom::WaitingRoom(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	crow::json::rvalue resBody;
	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/checkwaitingroom" });
	resBody = crow::json::load(response.text);
	//for each player display their option
	for (int i = 0; i < resBody.size() - 1; i++)
	{
		std::string stringUsername = resBody[i]["username"].s();
		QString username = QString::fromUtf8(stringUsername.c_str());
		if (resBody[i]["votedToStart"] == "true")
		{
			ui.listWidget->addItem(username + "                                 - voted to start");
		}
		else
		{
			ui.listWidget->addItem(username + "                                 - did not vote");
		}

	}
	ui.listWidget->update();
}

WaitingRoom::~WaitingRoom()
{}

void WaitingRoom::on_pushButton_clicked() {

	auto response8 = cpr::Put(
		cpr::Url{ "http://localhost:18080/addvote" },
		cpr::Payload{
			{ "username", "Gigi" }
		});

	crow::json::rvalue resBody;
	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/checkwaitingroom" });
	resBody = crow::json::load(response.text);
	//for each player display their option

	ui.listWidget->clear();

	for (int i = 0; i < resBody.size() - 1; i++)
	{
		std::string stringUsername = resBody[i]["username"].s();
		QString username = QString::fromUtf8(stringUsername.c_str());
		if (resBody[i]["votedToStart"] == "true")
		{
			ui.listWidget->addItem(username + "                                 - voted to start");
		}
		else
		{
			ui.listWidget->addItem(username + "                                 - did not vote");
		}

	}
	ui.listWidget->update();

}
