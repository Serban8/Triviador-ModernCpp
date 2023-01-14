#pragma once
#include <crow.h>

#include "Game.h"
#include "utils.h"

#include "ServerStatus.h"

//GetMapHandler
class GetMapHandler {
public:
	GetMapHandler(ServerStatus& status, Game& game);

	crow::json::wvalue operator() () const;
private:
	ServerStatus& status;
	Game& game;
};
//

//SetRegionOwnerHandler
class SetRegionOwnerHandler {
public:
	SetRegionOwnerHandler(Game& game);

	crow::response operator() (const crow::request& req) const;
private:
	Game& game;
};
//

//SetRegionTypeHandler
class InitializeRegionHandler {
public:
	InitializeRegionHandler(ServerStatus& status, Game& game);

	crow::response operator() (const crow::request& req) const;
private:
	ServerStatus& status;
	Game& game;
};
//

//IncreaseRegionScoreHandler
class IncreaseRegionScoreHandler {
public:
	IncreaseRegionScoreHandler(Game& game);

	crow::response operator() (const crow::request& req) const;
private:
	Game& game;
};
//

//DecreaseRegionScoreHandler
class DecreaseRegionScoreHandler {
public:
	DecreaseRegionScoreHandler(Game& game);

	crow::response operator() (const crow::request& req) const;
private:
	Game& game;
};
//