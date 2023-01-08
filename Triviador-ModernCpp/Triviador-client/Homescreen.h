#pragma once

#include <QMainWindow>
#include "ui_Homescreen.h"
#include "cpr/cpr.h"
#include "crow.h"


class Homescreen : public QMainWindow
{
	Q_OBJECT

public:
	Homescreen(QWidget *parent = nullptr);
	~Homescreen();
	void setUsername(std::string username);
private:
	std::string username = "gigel";
	Ui::HomescreenClass ui;
private slots:
	void on_playButton_clicked();
};
