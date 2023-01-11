#pragma once
#include <crow.h>
#include <map>

#include "Game.h"
#include "Database.h"
#include "QuestionDatabase.h"
#include "utils.h"

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
	AddResponseHandler(Game& game, std::map<numberQuestionResponse, std::unique_ptr<Player>, compareNumberQuestionResponses>& leaderboard);

	crow::response operator() (const crow::request& req) const;
private:
	Game& game;
	std::map<numberQuestionResponse, std::unique_ptr<Player>, compareNumberQuestionResponses>& leaderboard;
};
//

//GetLeaderboardHandler
class GetLeaderboardHandler {
public:
	GetLeaderboardHandler(std::map<numberQuestionResponse, std::unique_ptr<Player>, compareNumberQuestionResponses>& leaderboard);

	crow::json::wvalue operator() () const;
private:
	std::map<numberQuestionResponse, std::unique_ptr<Player>, compareNumberQuestionResponses>& leaderboard;
};
//

//SetAttackedTerritoryHandler
class SetAttackedTerritoryHandler {
public:
	SetAttackedTerritoryHandler(Game& game, std::shared_ptr<Region> attackedRegion);

	crow::response operator() (const crow::request& req) const;
private:
	Game& game;
	std::shared_ptr<Region> attackedRegion;
};
//

//StartGameHandler
class StartGameHandler {
public:
	StartGameHandler(Game& game, std::vector<Player>& waitingRoomList, Storage& storage);

	crow::response operator() () const;
private:
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
	GetNumberQuestionHandler(Game& game);

	crow::json::wvalue operator() () const;
private:
	Game& game;
};
//

//GetMultipleChoiceQuestionHandler
class GetMultipleChoiceQuestionHandler {
public:
	GetMultipleChoiceQuestionHandler(Game& game);

	crow::json::wvalue operator() () const;
private:
	Game& game;
};
//