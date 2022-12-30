#pragma once

#include "ui_TriviadorClient.h"
#include "cpr/cpr.h"
#include "crow.h"

#include <QtWidgets/QMainWindow>
#include <QLabel>
#include <thread>
#include <chrono>
#include "RegisterForm.h"

using statusCode = decltype(cpr::Response::status_code);

class TriviadorClient : public QMainWindow
{
	Q_OBJECT

public:
	TriviadorClient(QWidget* parent = nullptr);
	~TriviadorClient();
	statusCode CheckLoginInfo(std::string& username, std::string& password);
	statusCode CreateNewPlayer(std::string& username, std::string& password);
	void checkIfGameCanStart();
private:
	Ui::TriviadorclientClass ui;
	RegisterForm *regForm;
private slots:
	void on_LoginButton_clicked();
	void on_GoToRegisterButton_clicked();
};
