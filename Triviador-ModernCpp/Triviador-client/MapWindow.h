#pragma once

#include <QMainWindow>
#include <qscreen.h>
#include "ui_TwoPlayersMap.h"
#include "ui_ThreePlayersMap.h"
#include "ui_FourPlayersMap.h"

class MapWindow : public QMainWindow
{
	Q_OBJECT

public:
	MapWindow(QWidget *parent = nullptr);
	~MapWindow();
	void SetUI(int numberOfPlayers);
private:
	Ui::TwoPlayersMapClass twoPlayersMap_ui;
	Ui::ThreePlayersMapClass threePlayersMap_ui;
	Ui::FourPlayersMapClass fourPlayersMap_ui;
	std::vector<QPushButton> Territories;

};
