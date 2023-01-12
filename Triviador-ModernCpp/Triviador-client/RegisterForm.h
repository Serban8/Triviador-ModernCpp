#pragma once
#include <QMainWindow>
#include <QCloseEvent>
#include <cpr/cpr.h>
#include <crow.h>

#include "ui_RegisterForm.h"
#include "Homescreen.h"

using statusCode = decltype(cpr::Response::status_code);

class RegisterForm : public QMainWindow
{
	Q_OBJECT

public:
	RegisterForm(QWidget* parent = nullptr);
	~RegisterForm();

private:
	statusCode CreateNewPlayer(std::string username, std::string password);

	void closeEvent(QCloseEvent* ev) override;
private:
	Homescreen homescreen;
	Ui::RegisterFormClass ui;
	QMessageBox m_WarningMsgBox;
	QMessageBox m_InformationMsgBox;
	QPixmap pixmap = QPixmap(":/Triviadorclient/images/triv.png");
private slots:
	void on_register_pushButton_clicked();
};
