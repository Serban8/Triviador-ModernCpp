#pragma once

#include <QMainWindow>
#include <qscreen.h>
#include "ui_TwoPlayersMap.h"

class TwoPlayersMap : public QMainWindow
{
	Q_OBJECT

public:
	TwoPlayersMap(QWidget *parent = nullptr);
	~TwoPlayersMap();

private:
	Ui::TwoPlayersMapClass ui;
	std::vector<QPushButton> Territories;

};
