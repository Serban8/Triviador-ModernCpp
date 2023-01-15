#include "PregameRoutes.h"

//PlayerHandler
PlayerHandler::PlayerHandler(Storage& storage) : storage(storage)
{
}
crow::json::wvalue PlayerHandler::operator()(std::string username) const
{
	std::vector<crow::json::wvalue> games_json;
	auto playerGame = storage.get_all<PlayerGameDatabase>(sqlite_orm::where(sql::c(&PlayerGameDatabase::m_playerId) = username));
	for (const auto& pg : playerGame)
	{
		GameDatabase g = storage.get<GameDatabase>(*pg.m_gameId);
		games_json.push_back(crow::json::wvalue{
			{"date", g.m_date },
			{"rounds", std::to_string(g.m_rounds)},
			{"winner", *g.m_winner}
			});
	}
	return crow::json::wvalue{ games_json };
}
//

//AddVoteHandler
AddVoteHandler::AddVoteHandler(std::vector<Player>& waitingRoomList, std::vector<Player>& votesToStart) : waitingRoomList(waitingRoomList), votesToStart(votesToStart)
{
}
crow::response AddVoteHandler::operator()(const crow::request& req) const
{
	auto bodyArgs = parseRequestBody(req.body);
	auto bodyEnd = bodyArgs.end();

	auto usernameIter = bodyArgs.find("username");

	if (usernameIter != bodyEnd)
	{
		auto username = usernameIter->second;
		auto playerIter = std::find(waitingRoomList.begin(), waitingRoomList.end(), Player(username));
		//check if the player is in the waiting room
		if (playerIter != waitingRoomList.end()) {
			votesToStart.push_back(*playerIter);
		}
		else {
			CROW_LOG_INFO << "Did not find player with username \"" << username << "\" in waiting room";
			return crow::response(404, "NOT FOUND");
		}
	}
	else
	{
		return crow::response(400, "BAD REQUEST");
	}
	return crow::response(200);
}
//

//CheckWaitingRoomHandler
CheckWaitingRoomHandler::CheckWaitingRoomHandler(std::vector<Player>& waitingRoomList, std::vector<Player>& votesToStart) : waitingRoomList(waitingRoomList), votesToStart(votesToStart)
{
}
crow::json::wvalue CheckWaitingRoomHandler::operator() () const
{
	std::vector<crow::json::wvalue> waitingRoomList_json;
	bool startGame = false;

	//check if conditions for starting the game are fulfilled
	if (waitingRoomList.size() == maxPlayersPerGame || (votesToStart.size() == waitingRoomList.size() && waitingRoomList.size() > 1)) {
		startGame = true;
	}

	//write the response json
	for (const auto& player : waitingRoomList) {
		bool votedToStart = false;
		//check if current player has voted to start
		if (std::find(votesToStart.begin(), votesToStart.end(), player) != votesToStart.end()) {
			votedToStart = true;
		}
		//write the json for the current player
		waitingRoomList_json.push_back(crow::json::wvalue{
			{"username", player.GetUsername()}, //todo: insert here other different stats: w/l ratio, number of games played, etc..
			{"votedToStart", votedToStart ? "true" : "false"}
			});
	}
	//add the startGame flag to the response
	waitingRoomList_json.push_back(crow::json::wvalue{
		{"startGame", startGame ? "true" : "false"}
		});

	if (startGame)
		CROW_LOG_INFO << "GAME CAN START";
	else
		CROW_LOG_INFO << "GAME CAN NOT START YET";

	return crow::json::wvalue{ waitingRoomList_json };
}
//

//AddToWaitingRoomHandler
AddToWaitingRoomHandler::AddToWaitingRoomHandler(std::vector<Player>& waitingRoomList, Storage& storage) : waitingRoomList(waitingRoomList), storage(storage)
{
}
crow::response AddToWaitingRoomHandler::operator()(const crow::request& req) const
{
	auto bodyArgs = parseRequestBody(req.body);
	auto bodyEnd = bodyArgs.end();
	auto usernameIter = bodyArgs.find("username");

	if (usernameIter != bodyEnd)
	{
		auto username = usernameIter->second;
		waitingRoomList.push_back(Player(username));
		return crow::response(200);
	}

	return crow::response(400, "BAD REQUEST");

}
//