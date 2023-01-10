#include "WaitingRoom.h"

WaitingRoom::WaitingRoom(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//auto response1 = cpr::Put(
	//	cpr::Url{ "http://localhost:18080/addtowaitingroom" },
	//	cpr::Payload{
	//		{ "username", m_playerUsername}
	//	});

	//crow::json::rvalue resBody;
	//cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/checkwaitingroom" });
	//resBody = crow::json::load(response.text);
	////for each player display their option
	//for (int i = 0; i < resBody.size() - 1; i++)
	//{
	//	std::string stringUsername = resBody[i]["username"].s();
	//	QString username = QString::fromUtf8(stringUsername.c_str());
	//	if (resBody[i]["votedToStart"] == "true")
	//	{
	//		ui.listWidget->addItem(username + "                                 - voted to start");
	//	}
	//	else
	//	{
	//		ui.listWidget->addItem(username + "                                 - did not vote");
	//	}

	//}
	
}

WaitingRoom::~WaitingRoom()
{}

void WaitingRoom::SetUsername(std::string username) {

	m_playerUsername = username;

}

bool WaitingRoom::CheckingWaitingRoom() {

	crow::json::rvalue resBody;

	do
	{
		cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/checkwaitingroom" });
		resBody = crow::json::load(response.text);
		std::chrono::system_clock::time_point timePt = std::chrono::system_clock::now() + std::chrono::seconds(5);
		std::this_thread::sleep_until(timePt);

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
	} while (resBody[resBody.size() - 1]["startGame"] != "true");

	ui.listWidget->update();

	return true;
}

void WaitingRoom::on_vote_pushButton_clicked() {

	ui.listWidget->clear();

	auto response8 = cpr::Put(
		cpr::Url{ "http://localhost:18080/addvote" },
		cpr::Payload{
			{ "username", m_playerUsername }
		});

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
