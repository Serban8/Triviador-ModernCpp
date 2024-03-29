#include "TriviadorClient.h"
#include <qmessagebox.h>
#include <qgraphicsview.h>
#include <qsizepolicy.h>
#include <qregularexpression.h>

TriviadorClient::TriviadorClient(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	ui.username->setPlaceholderText("Enter your username");
	ui.password->setPlaceholderText("Enter your password");

	//create a regular expression that checks if input is alphanumeric
	QRegularExpression rxUsername("[a-zA-Z0-9_\\-\\.]+");
	QValidator* usernameValidator = new QRegularExpressionValidator(rxUsername, this);

	ui.username->setValidator(usernameValidator);

	ui.centralWidget->resize(QGuiApplication::primaryScreen()->availableGeometry().size() /** 0.9*/);

	ui.groupBox->move(this->width() / 2 - ui.groupBox->geometry().width() / 2, this->height() / 2 - ui.groupBox->geometry().height() / 2);
	ui.register_pushButton->move(this->width() / 2 - ui.register_pushButton->geometry().width() / 2, ui.groupBox->pos().y() - ui.register_pushButton->geometry().height() * 1.5);

	ui.stackedWidget->insertWidget(1, &regForm);
	ui.stackedWidget->insertWidget(2, &homescreen);
}

TriviadorClient::~TriviadorClient()
{}

statusCode TriviadorClient::CheckLoginInfo(std::string username, std::string password)
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

void TriviadorClient::on_register_pushButton_clicked()
{
	ui.stackedWidget->setCurrentIndex(1);
}

void TriviadorClient::on_login_pushButton_clicked() {

	std::string username = ui.username->text().toUtf8().constData();
	std::string password = ui.password->text().toUtf8().constData();

	if (username.empty() || password.empty()) {
		QMessageBox::warning(this, "Login", "Please enter a username and password.");
		return;
	}

	statusCode status = CheckLoginInfo(username, password);

	if (status == 200) {
		homescreen.SetUsername(username);
		ui.stackedWidget->setCurrentIndex(2);
	}
	else if (status == 404) {
		QMessageBox::warning(this, "Login failed", "No player with this username has been found!");
	}
	else if (status == 401) {
		QMessageBox::warning(this, "Login failed", "Password is incorrect");
	}
	else {
		std::string error = "Something went wrong! Error code: " + std::to_string(status);
		QMessageBox::warning(this, "Login failed", QString::fromUtf8(error));
	}

}