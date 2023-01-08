#include "ThreePlayersMap.h"


ThreePlayersMap::ThreePlayersMap(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	resize(QGuiApplication::primaryScreen()->availableGeometry().size() * 0.9);
}

ThreePlayersMap::~ThreePlayersMap()
{}
