#pragma once

#include <QMainWindow>
#include <QCloseEvent>
#include "ui_RegisterForm.h"
#include "Homescreen.h"

class RegisterForm : public QMainWindow
{
	Q_OBJECT

public:
	RegisterForm(QWidget* parent = nullptr);
	~RegisterForm();

private:
	void closeEvent(QCloseEvent* ev) override;
private:
	Homescreen* homescreen;
	Ui::RegisterFormClass ui;
private slots:
	void on_RegisterButton_clicked();
};
