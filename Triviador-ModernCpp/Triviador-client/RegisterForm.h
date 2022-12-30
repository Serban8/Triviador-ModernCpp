#pragma once

#include <QMainWindow>
#include <QCloseEvent>
#include "ui_RegisterForm.h"

class RegisterForm : public QMainWindow
{
	Q_OBJECT

public:
	RegisterForm(QWidget* parent = nullptr);
	~RegisterForm();

private:
	void closeEvent(QCloseEvent* ev) override;
private:
	Ui::RegisterFormClass ui;
private slots:
	void on_RegisterButton_clicked();
};
