#pragma once

#include <QMainWindow>

#include "cpr/cpr.h"
#include "crow/json.h"

#include "ui_Homescreen.h"

#include "GameHistory.h"
#include "WaitingRoom.h"

class Homescreen : public QMainWindow
{
	Q_OBJECT

public:
	Homescreen(QWidget* parent = nullptr);
	~Homescreen();
	void SetUsername(std::string username);

private:
	Ui::HomescreenClass ui;
	GameHistory* gameHistory;
	WaitingRoom waitingRoom;
	std::string m_playerUsername;

private slots:
	void on_play_pushButton_clicked();
	void on_GameHistory_pushButton_clicked();


};
