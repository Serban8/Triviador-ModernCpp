#include "FourPlayersMap.h"

FourPlayersMap::FourPlayersMap(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	resize(QGuiApplication::primaryScreen()->availableGeometry().size() * 0.9);
}

FourPlayersMap::~FourPlayersMap()
{}
