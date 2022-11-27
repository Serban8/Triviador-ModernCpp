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
		void static insertPlayer(T& storage, GameDatabase game, PlayerDatabase player)
		{
			PlayerGameDatabase playerGameDB(game, player);
			storage.insert(playerGameDB);
		}
	}
}