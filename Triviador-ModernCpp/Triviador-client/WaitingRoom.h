#pragma once

#include <QMainWindow>
#include <QTimer>
#include <qmessagebox.h>

#include "crow/json.h"
#include "cpr/cpr.h"

#include "ui_WaitingRoom.h"
#include "MapWindow.h"

class WaitingRoom : public QMainWindow
{
	Q_OBJECT

public:
	WaitingRoom(QWidget* parent = nullptr);
	~WaitingRoom();
	void SetUsername(std::string username);
	void SetTimer();

private:
	MapWindow map;
	Ui::WaitingRoomClass ui;
	std::string m_playerUsername;
	QTimer* m_waitingRoomTimer;
	QMessageBox m_InformationMsgBox;
	QPixmap pixmap = QPixmap(":/Triviadorclient/images/triv.png");

private slots:
	void on_vote_pushButton_clicked();
	void UpdateWaitingRoom();
};
