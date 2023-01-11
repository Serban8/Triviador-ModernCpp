#include "RegisterForm.h"
#include<qmessagebox.h>

RegisterForm::RegisterForm(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	ui.username->setPlaceholderText("Enter your username");
	ui.password->setPlaceholderText("Enter your password");

	ui.stackedWidget->insertWidget(1, &homescreen);

}

RegisterForm::~RegisterForm()
{
	parentWidget()->show();
}

statusCode RegisterForm::CreateNewPlayer(std::string username, std::string password)
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

void RegisterForm::closeEvent(QCloseEvent* ev)
{
	parentWidget()->show();
	ev->accept();
}

void RegisterForm::on_register_pushButton_clicked() {

	std::string username = ui.username->text().toUtf8().constData();
	std::string password = ui.password->text().toUtf8().constData();

	if (username.empty() || password.empty()) {
		QMessageBox::warning(this, "Warning", "Username or password cannot be empty!");
		return;
	}

	statusCode status = CreateNewPlayer(username, password);

	if (status == 200) {
		QMessageBox::information(this, "Success", "You have successfully registered!");
		homescreen.SetUsername(username);
		ui.stackedWidget->setCurrentIndex(1);
	}
	else if (status == 409)
		QMessageBox::warning(this, "Warning", "Username already exists!");
	else {
		std::string message = "Something went wrong! Error code: " + std::to_string(status);
		QMessageBox::warning(this, "Warning", QString::fromUtf8(message));
	}

}