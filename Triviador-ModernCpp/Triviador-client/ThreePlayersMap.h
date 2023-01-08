#pragma once

#include <QMainWindow>
#include "ui_ThreePlayersMap.h"

class ThreePlayersMap : public QMainWindow
{
	Q_OBJECT

public:
	ThreePlayersMap(QWidget *parent = nullptr);
	~ThreePlayersMap();

private:
	Ui::ThreePlayersMapClass ui;
};
