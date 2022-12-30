#include "RegisterForm.h"
#include<qmessagebox.h>

RegisterForm::RegisterForm(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	ui.username->setPlaceholderText("Enter your username");
	ui.password->setPlaceholderText("Enter your password");
}

RegisterForm::~RegisterForm()
{}

void RegisterForm::on_RegisterButton_clicked() {

	//testing
	QMessageBox::information(this, "Register", "Registration successful!");

}