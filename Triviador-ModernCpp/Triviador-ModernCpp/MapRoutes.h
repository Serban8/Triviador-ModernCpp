#pragma once
#include <crow.h>

#include "Game.h"
#include "utils.h"

//GetMapHandler
class GetMapHandler {
public:
	GetMapHandler(Game& game);

	crow::json::wvalue operator() () const;
private:
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
class SetRegionTypeHandler {
public:
	SetRegionTypeHandler(Game& game);

	crow::response operator() (const crow::request& req) const;
private:
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