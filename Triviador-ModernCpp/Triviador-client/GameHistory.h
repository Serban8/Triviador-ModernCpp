#pragma once

#include <QMainWindow>
#include <sstream>
#include "ui_GameHistory.h"
#include "cpr/cpr.h"
#include "crow.h"

class GameHistory : public QMainWindow
{
	Q_OBJECT

public:
	GameHistory(QWidget *parent = nullptr);
	~GameHistory();

private:
	Ui::GameHistoryClass ui;
private slots:
	void on_close_pushButton_clicked();
};
