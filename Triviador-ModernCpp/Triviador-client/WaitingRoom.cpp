#include "WaitingRoom.h"

WaitingRoom::WaitingRoom(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_waitingRoomTimer = new QTimer(this);
	connect(m_waitingRoomTimer, SIGNAL(timeout()), this, SLOT(UpdateWaitingRoom()));
	ui.listWidget->move(this->width() / 2 - ui.listWidget->geometry().width() / 2, this->height() / 2 - ui.listWidget->geometry().height() / 3);
	ui.vote_pushButton->move(this->width() / 2 - ui.vote_pushButton->geometry().width() / 2, ui.listWidget->pos().y() - ui.vote_pushButton->geometry().height() * 1.5);
	ui.stackedWidget->insertWidget(1, &map);
	//ui.stackedWidget->widget(0)->setStyleSheet("#stackedWidget{border-image:url(:/Triviadorclient/images/background.jpg)}");

	m_InformationMsgBox.setWindowIcon(QIcon(pixmap));
	m_InformationMsgBox.setIcon(QMessageBox::Information);
}

WaitingRoom::~WaitingRoom()
{}

void WaitingRoom::SetUsername(std::string username) {
	m_playerUsername = username;

}

void WaitingRoom::SetTimer() {
	m_waitingRoomTimer->start(3500);
	UpdateWaitingRoom();
}

void WaitingRoom::UpdateWaitingRoom()
{
	ui.listWidget->clear();
	crow::json::rvalue resBody;

	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/checkwaitingroom" });
	resBody = crow::json::load(response.text);
	if (resBody[resBody.size() - 1]["startGame"] == "true")
	{
		m_InformationMsgBox.setText("The game will start soon!");
		m_InformationMsgBox.exec();

		//QMessageBox::information(this,"information", "The game will start soon!");
		m_waitingRoomTimer->stop();
		cpr::Response response1 = cpr::Get(cpr::Url{ "http://localhost:18080/startgame" });
		map.SetUI(resBody.size() - 1);
		ui.stackedWidget->setCurrentIndex(1);
	}
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