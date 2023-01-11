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

void RegisterForm::closeEvent(QCloseEvent* ev)
{
	parentWidget()->show();
	ev->accept();
}

void RegisterForm::on_register_pushButton_clicked() {

	//testing
	QMessageBox::information(this, "Register", "Registration successful! You will now be directed to the homescreen.");

	ui.stackedWidget->setCurrentIndex(1);

}