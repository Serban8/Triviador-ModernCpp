#pragma once
#include <string>
#include <vector>

#include "Player.h"

struct PlayerDatabase
{
	PlayerDatabase() = default;

	PlayerDatabase(Player p) :
		m_username(p.GetUsername())
	{
		//empty
	}

	std::string m_username;
	std::string m_password = "tmpass123";

};

namespace database {
	template<class T>
	void static insertPlayer(T& storage, Player p)
	{
		PlayerDatabase pDB(p);
		//used .replace() instead of .insert() because we do not have autoincrement primary_key
		//in player table (primary key is usernam)
		storage.replace(pDB);
	}
}
