#pragma once
#include <crow.h>

#include "Game.h"

#include "PlayerDatabase.h"

#include "MapRoutes.h"
#include "PregameRoutes.h"
#include "GameRoutes.h"

#include "utils.h"
#include "..\Triviador-DLL\ServerStatus.h"
/*
Game flow:
	when we enter the game we call /startgame
	then we enter a loop (QTimer) where /checkstatus is called continously until the end of the game

	For CHOOSING BASES phase each client does:
	 /getnumberquestion
	 /addresponse -> sends the response and the time it took
	 /getleaderboard
	 /setregiontype -> gives the server the location(?) of the region to be updated
	 /getmap

	For CHOOSING TERRITORIES phase each client does:
	while regions_left
		/getnumberquestion
		/addresponse -> sends the response and the time it took
		/getleaderboard
		while his turn
			/setregiontype -> gives the server the location(?) of the region to be updated
		end while
		while others not finished
			wait
		end while
		/getmap
	end while

	For DUEL phase each client does:
	while !game_over do
		->startround
		attacker client:
			/setattackedterritory
			/getmultiplechoicequestion
			/addresponse -> sends the response and the time it took
			/getleaderboard
		attacked client:
			/getmultiplechoicequestion
			/addresponse -> sends the response and the time it took
			/getleaderboard
		other clients:
			/getmultiplechoicequestion (no option to respond)
			/getleaderboard
			wait for the rest of the time
		/getmap
		->endround
	end while
*/
class AppManager
{
public:
	AppManager(crow::SimpleApp& app, Storage& storage);

private:
	void GenereatePlayerOrder();
	
private:
	ServerStatus m_status; //maybe should be static

	std::vector<Player> m_waitingRoomList = { Player("Gigi"), Player("Marci"), Player("Luci") }; //initialization list for testing only
	std::vector<Player> m_votesToStart;
	std::map<numberQuestionResponse, std::shared_ptr<Player>, compareNumberQuestionResponses> m_leaderboard; //maybe better with queue?
	//maybe will not work with shared_ptr

	std::queue<std::string> m_playerOrder;
	Game m_game;
	Player m_attacker, m_attacked; //should be pointers
	std::shared_ptr<Region> m_attackedRegion;
private:
	crow::SimpleApp& m_app;
	Storage& m_storage;
};

