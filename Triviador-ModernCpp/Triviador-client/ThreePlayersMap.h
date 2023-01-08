#pragma once

#include <QMainWindow>
#include <qscreen.h>
#include "ui_ThreePlayersMap.h"

class ThreePlayersMap : public QMainWindow
{
	Q_OBJECT

public:
	ThreePlayersMap(QWidget *parent = nullptr);
	~ThreePlayersMap();

private:
	Ui::ThreePlayersMapClass ui;
	std::vector<QPushButton> Territories;
};
