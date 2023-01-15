#include "RegisterForm.h"
#include<qmessagebox.h>

RegisterForm::RegisterForm(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	ui.username->setPlaceholderText("Enter your username");
	ui.password->setPlaceholderText("Enter your password");

	//create a regular expression that checks if input is alphanumeric
	QRegularExpression rxUsername("[a-zA-Z0-9_\\-\\.]+");
	QValidator* usernameValidator = new QRegularExpressionValidator(rxUsername, this);

	ui.username->setValidator(usernameValidator);

	ui.stackedWidget->insertWidget(1, &homescreen);
	ui.groupBox->move(this->width() / 2 - ui.groupBox->geometry().width() / 2, this->height() / 2 - ui.groupBox->geometry().height() / 2);

	m_WarningMsgBox.setWindowIcon(QIcon(pixmap));
	m_WarningMsgBox.setIcon(QMessageBox::Warning);

	m_InformationMsgBox.setWindowIcon(QIcon(pixmap));
	m_InformationMsgBox.setIcon(QMessageBox::Information);
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

		m_WarningMsgBox.setText("Username or password cannot be empty!");
		m_WarningMsgBox.exec();

		//QMessageBox::warning(this, "Warning", "Username or password cannot be empty!");
		return;
	}

	statusCode status = CreateNewPlayer(username, password);

	if (status == 200) {

		m_InformationMsgBox.setText("You have successfully registered!");
		m_InformationMsgBox.exec();

		//QMessageBox::information(this, "Success", "You have successfully registered!");
		homescreen.SetUsername(username);
		ui.stackedWidget->setCurrentIndex(1);
	}
	else if (status == 409) {

		m_WarningMsgBox.setText("Username already exists!");
		m_WarningMsgBox.exec();

	}
	//QMessageBox::warning(this, "Warning", "Username already exists!");
	else {
		std::string message = "Something went wrong! Error code: " + std::to_string(status);

		m_WarningMsgBox.setText(QString::fromUtf8(message));
		m_WarningMsgBox.exec();

		//QMessageBox::warning(this, "Warning", QString::fromUtf8(message));
	}

}