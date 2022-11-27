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

	std::string m_username;
	std::string m_password = "tmpass123";

	//TODO: create table GamesPlayers to solve many to many relationship between player table and game table.

};

namespace database {
	namespace player {
		template<class T>
		void static insertPlayer(T& storage, Player p)
		{
			PlayerDatabase pDB(p);
			//used .replace() instead of .insert() because we do not have autoincrement primary_key
			//in pl
			storage.replace(pDB); 
		}
		template<class T>
		std::vector<PlayerDatabase> static getAllPlayers(T& storage)
		{
			auto allPlayers = storage.get_all<PlayerDatabase>();
			//for testing
			for (auto& p : allPlayers)
			{
				std::cout << storage.dump(p) << std::endl;
			}
			//
			return allPlayers;
		}
		template<class T>
		PlayerDatabase static getPlayer(T& storage, std::string username)
		{
			auto player = storage.get<PlayerDatabase>(username);
			//for testing
				std::cout << storage.dump(player) << std::endl;
			//
			return player;
		}
	}
}

