#pragma once
#include <crow.h>
#include <map>

#include "Game.h"
#include "Database.h"
#include "QuestionDatabase.h"
#include "..\Triviador-DLL\utils.h"
#include "..\Triviador-DLL\ServerStatus.h"

using numberQuestionResponse = std::pair<float, float>;

//compare functor for creating the leaderboard
struct compareNumberQuestionResponses
{
	bool operator() (const numberQuestionResponse& a, const numberQuestionResponse& b) const
	{
		auto& [response1, responseTime1] = a;
		auto& [response2, responseTime2] = b;

		if (response1 == response2)
		{
			return responseTime1 < responseTime2;
		}
		return response1 < response2;
	}
};
//

//AddResponseHandler
class AddResponseHandler {
public:
	AddResponseHandler(ServerStatus& status, Game& game, std::shared_ptr<Region> attackedRegion, std::map<numberQuestionResponse, std::shared_ptr<Player>, compareNumberQuestionResponses>& leaderboard);

	crow::response operator() (const crow::request& req) const;
private:
	std::shared_ptr<Region> attackedRegion;
	ServerStatus& status;
	Game& game;
	std::map<numberQuestionResponse, std::shared_ptr<Player>, compareNumberQuestionResponses>& leaderboard;
};
//

//GetLeaderboardHandler
class GetLeaderboardHandler {
public:
	GetLeaderboardHandler(ServerStatus& status, Game& game, std::map<numberQuestionResponse, std::shared_ptr<Player>, compareNumberQuestionResponses>& leaderboard);

	crow::json::wvalue operator() () const;
private:
	ServerStatus& status;
	Game& game;
	std::map<numberQuestionResponse, std::shared_ptr<Player>, compareNumberQuestionResponses>& leaderboard;
};
//

//SetAttackedTerritoryHandler
class SetAttackedTerritoryHandler {
public:
	SetAttackedTerritoryHandler(ServerStatus& status, Player& attacked, Game& game, std::shared_ptr<Region>& attackedRegion);

	crow::response operator() (const crow::request& req) const;
private:
	ServerStatus& status;
	Game& game;
	Player& attacked;
	std::shared_ptr<Region>& attackedRegion;
};
//

//StartGameHandler
class StartGameHandler {
public:
	StartGameHandler(ServerStatus& status, Game& game, std::vector<Player>& waitingRoomList, Storage& storage);

	crow::response operator() () const;
private:
	ServerStatus& status;
	Game& game;
	std::vector<Player>& waitingRoomList;
	Storage& storage;
};
//

//GetPlayersHandler
class GetPlayersHandler {
public:
	GetPlayersHandler(Game& game);

	crow::json::wvalue operator() () const;
private:
	Game& game;
};
//

//GetNumberQuestionHandler
class GetNumberQuestionHandler {
public:
	GetNumberQuestionHandler(ServerStatus& status, Game& game);

	crow::json::wvalue operator() () const;
private:
	ServerStatus& status;
	Game& game;
};
//

//GetMultipleChoiceQuestionHandler
class GetMultipleChoiceQuestionHandler {
public:
	GetMultipleChoiceQuestionHandler(ServerStatus& status, Player& attacker, Player& attacked, Game& game);

	crow::json::wvalue operator() (const crow::request& req) const;
private:
	Player& attacker;
	Player& attacked;
	ServerStatus& status;
	Game& game;
};
//