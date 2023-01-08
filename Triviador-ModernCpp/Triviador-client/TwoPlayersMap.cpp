#include "TwoPlayersMap.h"

TwoPlayersMap::TwoPlayersMap(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	resize(QGuiApplication::primaryScreen()->availableGeometry().size() * 0.9);
}

TwoPlayersMap::~TwoPlayersMap()
{}
