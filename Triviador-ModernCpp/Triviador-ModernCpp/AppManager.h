#pragma once
#include <crow.h>

#include "Game.h"

#include "PlayerDatabase.h"

#include "MapRoutes.h"
#include "PregameRoutes.h"
#include "GameRoutes.h"

class AppManager
{
public:
	AppManager(crow::SimpleApp& app, Storage& storage);

private:
	std::vector<Player> m_waitingRoomList = { Player("Gigi"), Player("Marci"), Player("Luci"), Player("Cici") }; //initialization list for testing only
	std::vector<Player> m_votesToStart;
	std::map<numberQuestionResponse, std::unique_ptr<Player>, compareNumberQuestionResponses> m_leaderboard;

	Game m_game;
	std::shared_ptr<Region> m_attackedRegion;
private:
	crow::SimpleApp& m_app;
	Storage& m_storage;
};

