#include "MapWindow.h"

MapWindow::MapWindow(QWidget* parent)
	: QMainWindow(parent)
{
	twoPlayersMap_ui.setupUi(this);
	resize(QGuiApplication::primaryScreen()->availableGeometry().size() * 0.9);
}


MapWindow::~MapWindow()
{}

void MapWindow::SetUI(int numberOfPlayers)
{
	if (numberOfPlayers == 2)
	{
		twoPlayersMap_ui.setupUi(this);
	}
	else
		if (numberOfPlayers == 3)
		{
			threePlayersMap_ui.setupUi(this);
		}
		else
			if (numberOfPlayers == 4)
			{
				fourPlayersMap_ui.setupUi(this);
			}
}
