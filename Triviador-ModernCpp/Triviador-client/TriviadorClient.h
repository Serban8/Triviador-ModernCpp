#pragma once

#include "ui_TriviadorClient.h"
#include "cpr/cpr.h"
#include "crow.h"

#include <QtWidgets/QMainWindow>
#include <QLabel>
#include <thread>
#include <chrono>
#include "RegisterForm.h"
#include "Homescreen.h"
#include "Player.h"

class TriviadorClient : public QMainWindow
{
	Q_OBJECT

public:
	TriviadorClient(QWidget* parent = nullptr);
	~TriviadorClient();
	statusCode CheckLoginInfo(std::string username, std::string password);

private:
	Ui::TriviadorclientClass ui;
	RegisterForm regForm;
	Homescreen homescreen;
	
private slots:
	void on_login_pushButton_clicked();
	void on_register_pushButton_clicked();
};
