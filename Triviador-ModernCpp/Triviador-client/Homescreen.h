#pragma once

#include <QMainWindow>
#include <qthread.h>
#include "ui_Homescreen.h"
#include "cpr/cpr.h"
#include "crow.h"
#include "GameHistory.h"
#include "WaitingRoom.h"

class Homescreen : public QMainWindow
{
	Q_OBJECT

public:
	Homescreen(QWidget *parent = nullptr);
	~Homescreen();
	void SetUsername(std::string username);
	//void SetUsername(std::string username);
private:
	//std::string m_username = "gigel";
	Ui::HomescreenClass ui;
	GameHistory *gameHistory;
	WaitingRoom waitingRoom;
	std::string m_playerUsername;
	
private slots:
	void on_play_pushButton_clicked();
	void on_GameHistory_pushButton_clicked();
	

};
