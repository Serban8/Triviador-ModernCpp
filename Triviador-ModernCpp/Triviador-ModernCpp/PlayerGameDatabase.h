#pragma once
#include "Game.h"
#include"GameDatabase.h"
#include"Player.h"
#include"PlayerDatabase.h"
//TO VERIFY: for tables made for N:M relations if we use Database class or game classes
struct PlayerGameDatabase{

	PlayerGameDatabase(){}
	PlayerGameDatabase(GameDatabase game, PlayerDatabase player):
		m_gameId(game.m_id),
		m_playerId(player.m_username)
	{
		//empty
	}

	int m_gameId;
	std::string m_playerId;
};
namespace database {
	namespace playerGame {
		template <class T>
		void static insertPlayerGame(T& storage, GameDatabase game, PlayerDatabase player)
		{
			PlayerGameDatabase playerGameDB(game, player);
			storage.insert(playerGameDB);
		}
		template <class T>
		std::vector<PlayerGameDatabase> static getAllPlayerGames(T& storage)
		{
			auto allPlayerGames = storage.get_all<PlayerGameDatabase>();
			//for testing
			for (auto& p : allPlayerGames)
			{
				std::cout << storage.dump(p) << std::endl;
			}
			//
			return allPlayerGames;
		}
	}
}