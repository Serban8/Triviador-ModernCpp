#include "WaitingRoom.h"

WaitingRoom::WaitingRoom(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_waitingRoomTimer = new QTimer(this);
	connect(m_waitingRoomTimer, SIGNAL(timeout()), this, SLOT(UpdateWaitingRoom()));
	ui.listWidget->move(this->width() / 2 - ui.listWidget->geometry().width() / 2, this->height() / 2 - ui.listWidget->geometry().height() / 3);
	ui.vote_pushButton->move(this->width() / 2 - ui.vote_pushButton->geometry().width() / 2, ui.listWidget->pos().y() - ui.vote_pushButton->geometry().height() * 1.5);
}

WaitingRoom::~WaitingRoom()
{}

void WaitingRoom::SetUsername(std::string username) {
	m_playerUsername = username;

}

void WaitingRoom::SetTimer() {
	UpdateWaitingRoom();
	m_waitingRoomTimer->start(3500);
}

void WaitingRoom::UpdateWaitingRoom()
{
	ui.listWidget->clear();
	crow::json::rvalue resBody;

	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/checkwaitingroom" });
	resBody = crow::json::load(response.text);

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
}

void WaitingRoom::on_vote_pushButton_clicked() {

	auto response = cpr::Put(
		cpr::Url{ "http://localhost:18080/addvote" },
		cpr::Payload{
			{ "username", m_playerUsername }
		});

	UpdateWaitingRoom();
}
