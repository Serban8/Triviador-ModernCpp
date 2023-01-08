#pragma once

#include <QMainWindow>
#include <qscreen.h>
#include "ui_FourPlayersMap.h"

class FourPlayersMap : public QMainWindow
{
	Q_OBJECT

public:
	FourPlayersMap(QWidget *parent = nullptr);
	~FourPlayersMap();

private:
	Ui::FourPlayersMapClass ui;
};
