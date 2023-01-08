#include "RegisterForm.h"
#include<qmessagebox.h>

RegisterForm::RegisterForm(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	ui.username->setPlaceholderText("Enter your username");
	ui.password->setPlaceholderText("Enter your password");
}

RegisterForm::~RegisterForm()
{
	parentWidget()->show();
}

void RegisterForm::closeEvent(QCloseEvent* ev)
{
	parentWidget()->show();
	ev->accept();
}

void RegisterForm::on_RegisterButton_clicked() {

	//testing
	QMessageBox::information(this, "Register", "Registration successful!");
	homescreen = new Homescreen(this);
	homescreen->show();
	this->hide();

}