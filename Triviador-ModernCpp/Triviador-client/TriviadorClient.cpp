#include "TriviadorClient.h"
#include <qmessagebox.h>
#include <qgraphicsview.h>
#include <qsizepolicy.h>
TriviadorClient::TriviadorClient(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	ui.username->setPlaceholderText("Enter your username");
	ui.password->setPlaceholderText("Enter your password");

	bool notResized = true;

	//ui.stackedWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	ui.stackedWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	ui.centralWidget->resize(QGuiApplication::primaryScreen()->availableGeometry().size() /** 0.9*/);
	//QRect graphicsViewGeometry = ui.centralWidget->geometry();
	//ui.groupBox->setGeometry({ graphicsViewGeometry.width() - ui.groupBox->geometry().width()/10 - 25, graphicsViewGeometry.height() - ui.groupBox->geometry().height()/10 - 25, ui.groupBox->geometry().width(), ui.groupBox->geometry().height()});

	ui.stackedWidget->insertWidget(1, &regForm);
	
}

TriviadorClient::~TriviadorClient()
{}

statusCode TriviadorClient::CheckLoginInfo(std::string& username, std::string& password)
{
	auto response = cpr::Put(
		cpr::Url{ "http://localhost:18080/checkplayer" },
		cpr::Payload{
			{ "username", username },
			{ "password", password }
		}
	);

	return response.status_code;
}

statusCode TriviadorClient::CreateNewPlayer(std::string& username, std::string& password)
{
	auto response = cpr::Put(
		cpr::Url{ "http://localhost:18080/addnewplayer" },
		cpr::Payload{
			{ "username", username },
			{ "password", password }
		}
	);

	return response.status_code;
}

void TriviadorClient::checkIfGameCanStart()
{
	crow::json::rvalue resBody; //the list of all players and their votes and the flag for starting the game
	do
	{
		cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/checkwaitingroom" });
		resBody = crow::json::load(response.text);
		std::chrono::system_clock::time_point timePt = std::chrono::system_clock::now() + std::chrono::seconds(5);
		std::this_thread::sleep_until(timePt);

		//for each player display their option
		for (int i = 0; i < resBody.size() - 1; i++)
		{
			if (resBody[i]["votedToStart"] == "true")
			{
				std::cout << resBody[i]["username"] << " voted to start" << std::endl;
			}
			else
			{
				std::cout << resBody[i]["username"] << " did not vote" << std::endl;
			}
		}
		//if the flag is set to true then the game can start
	} while (resBody[resBody.size() - 1]["startGame"] != "true");
}

void TriviadorClient::updatePosition()
{
	QRect graphicsViewGeometry = ui.centralWidget->geometry();
	//ui.groupBox->setGeometry({ graphicsViewGeometry.height()/2, graphicsViewGeometry.width()/2 + 200,ui.groupBox->geometry().width(), ui.groupBox->geometry().height()});
	//ui.groupBox->move({ graphicsViewGeometry.width() / 2 + 600, graphicsViewGeometry.height() / 2});
	ui.groupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void TriviadorClient::on_GoToRegisterButton_clicked()
{

	//regForm = new RegisterForm(this);

	//regForm.show();
	//this->hide();

	ui.stackedWidget->setCurrentIndex(1);
}

void TriviadorClient::on_LoginButton_clicked() {

	QString username = ui.username->text();
	QString password = ui.password->text();
	//std::string x = username.toUtf8().toStdString();
	//testing purposes

	if (username == "admin" && password == "admin") {
		QMessageBox::information(this, "Login", "Login successful!");
		homescreen = new Homescreen(this);
		//homescreen->setUsername(x);
		homescreen->show();
		//this->hide();
	}
	else {
		QMessageBox::warning(this, "Login", "Login not successful!");
	}

}