#pragma once
#include <string>
#include <vector>
#include "Player.h"
struct PlayerDatabase
{
	PlayerDatabase() {}

	PlayerDatabase(Player p) :
		m_username(p.GetUsername())
	{
		//empty
	}

	int m_id;

	std::string m_username;
	std::string m_password = "tmpass123";

	//TODO: create table GamesPlayers to solve many to many relationship between player table and game table.

};

namespace database {
	namespace player {
		template<class T> void static insertPlayer(T& storage, Player p)
		{
			PlayerDatabase pDB(p);
			storage.insert(pDB);
		}
	}
}

