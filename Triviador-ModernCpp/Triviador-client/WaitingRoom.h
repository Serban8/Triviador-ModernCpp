#pragma once

#include <QMainWindow>
#include "ui_WaitingRoom.h"
#include "crow.h"
#include "cpr/cpr.h"
class WaitingRoom : public QMainWindow
{
	Q_OBJECT

public:
	WaitingRoom(QWidget *parent = nullptr);
	~WaitingRoom();
	void SetUsername(std::string username);
	bool CheckingWaitingRoom();
private:
	Ui::WaitingRoomClass ui;
	std::string m_playerUsername;
	
private slots:
	void on_vote_pushButton_clicked();
};
