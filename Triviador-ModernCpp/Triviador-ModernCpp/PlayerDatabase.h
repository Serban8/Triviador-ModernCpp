#pragma once
#include<string>
#include<vector>
#include"Player.h"
struct PlayerDatabase
{
	PlayerDatabase() {}

	PlayerDatabase (Player p) :
		m_id(p.GetId()),
		//m_numberOfWins(p.GetNumberOfWins()),
		//m_numberOfLosses(p.GetNumberOfLosses()),
		m_username(p.GetUsername()),
		m_password(p.GetPassword())
	{
		//empty
	}

	template<class T> void static insertPlayer(T& storage, Player p)
	{
		PlayerDatabase pDB(p);
		storage.insert(pDB);
	}

	int m_id;
	//int m_numberOfWins;
	//int m_numberOfLosses;

	std::string m_username;
	std::string m_password;

	//TODO: create table GamesPlayers to solve many to many relationship between player table and game table.


};

