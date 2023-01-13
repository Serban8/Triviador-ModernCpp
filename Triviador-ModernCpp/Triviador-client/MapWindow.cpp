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
		setTwoPlayersRegions();
	}
	else
		if (numberOfPlayers == 3)
		{
			threePlayersMap_ui.setupUi(this);
			setTwoPlayersRegions();
		}
		else
			if (numberOfPlayers == 4)
			{
				fourPlayersMap_ui.setupUi(this);
				setFourPlayersRegions();
			}
}

void MapWindow::setTwoPlayersRegions()
{
	Regions.push_back(twoPlayersMap_ui.groupBox_Territory1);
	Regions.push_back(twoPlayersMap_ui.groupBox_Territory2);
	Regions.push_back(twoPlayersMap_ui.groupBox_Territory3);
	Regions.push_back(twoPlayersMap_ui.groupBox_Territory4);
	Regions.push_back(twoPlayersMap_ui.groupBox_Territory5);
	Regions.push_back(twoPlayersMap_ui.groupBox_Territory6);
	Regions.push_back(twoPlayersMap_ui.groupBox_Territory7);
	Regions.push_back(twoPlayersMap_ui.groupBox_Territory8);
	Regions.push_back(twoPlayersMap_ui.groupBox_Territory9);
}

void MapWindow::setThreePlayersRegions()
{
	Regions.push_back(threePlayersMap_ui.groupBox_Territory1);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory2);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory3);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory4);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory5);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory6);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory7);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory8);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory9);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory10);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory11);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory12);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory13);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory14);
	Regions.push_back(threePlayersMap_ui.groupBox_Territory15);
}

void MapWindow::setFourPlayersRegions()
{
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory1);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory2);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory3);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory4);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory5);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory6);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory7);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory8);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory9);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory10);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory11);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory12);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory13);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory14);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory15);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory16);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory17);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory18);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory19);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory20);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory21);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory22);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory23);
	Regions.push_back(fourPlayersMap_ui.groupBox_Territory24);
}
