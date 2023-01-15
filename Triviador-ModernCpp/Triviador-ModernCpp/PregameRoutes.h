#pragma once
#include <crow.h>

#include "Game.h"
#include "Database.h"
#include "..\Triviador-DLL\utils.h"

const uint8_t maxPlayersPerGame = 4;

//AddToWaitingRoomHandler
class AddToWaitingRoomHandler {
public:
	AddToWaitingRoomHandler(std::vector<Player>& waitingRoomList, Storage& storage);

	crow::response operator() (const crow::request& req) const;
private:
	std::vector<Player>& waitingRoomList;
	Storage& storage;
};
//

//CheckWaitingRoomHandler
class CheckWaitingRoomHandler {
public:
	CheckWaitingRoomHandler(std::vector<Player>& waitingRoomList, std::vector<Player>& votesToStart);

	crow::json::wvalue operator() () const;
private:
	std::vector<Player>& waitingRoomList;
	std::vector<Player>& votesToStart;
};
//

//AddVoteHandler
class AddVoteHandler {
public:
	AddVoteHandler(std::vector<Player>& waitingRoomList, std::vector<Player>& votesToStart);

	crow::response operator() (const crow::request& req) const;
private:
	std::vector<Player>& waitingRoomList;
	std::vector<Player>& votesToStart;
};
//

//PlayerHandler
class PlayerHandler {
public:
	PlayerHandler(Storage& storage);

	crow::json::wvalue operator() (std::string username) const;
private:
	Storage& storage;
};
//

