#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QtConcurrent\qtconcurrentrun.h>
#include <qfuture.h>

#include "ui_WaitingRoom.h"
#include "MapWindow.h"
#include "crow.h"
#include "cpr/cpr.h"
#include <qmessagebox.h>
class WaitingRoom : public QMainWindow
{
	Q_OBJECT

public:
	WaitingRoom(QWidget *parent = nullptr);
	~WaitingRoom();
	void SetUsername(std::string username);
	void SetTimer();
	
private:
	MapWindow map;
	Ui::WaitingRoomClass ui;
	std::string m_playerUsername;
	QTimer* m_waitingRoomTimer;
	
private slots:
	void on_vote_pushButton_clicked();
	void UpdateWaitingRoom();
};
