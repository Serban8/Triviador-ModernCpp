#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include<crow.h>
#include "Player.h"
#include "utils.h"

struct PlayerDatabase
{
	PlayerDatabase() = default;

	PlayerDatabase(Player p) :
		m_username(p.GetUsername())
	{
		//empty
	}

	std::string m_username;
	std::string m_password;

};

//using Storage = decltype(database::createStorage(""));
//
//class AddPlayerHandler
//{
//public:
//	AddPlayerHandler(Storage& storage) : m_db(storage)
//	{
//		//empty
//	}
//
//	crow::response operator()(const crow::request& req) const
//	{
//		auto bodyArgs = parseRequestBody(req.body);  //id=2&quantity=3&...
//		auto end = bodyArgs.end();
//		auto userIter = bodyArgs.find("username");
//		auto passwordIter = bodyArgs.find("password");
//
//		if (userIter != end && passwordIter != end)
//		{
//			PlayerDatabase pDB;
//			pDB.m_username = std::stoi(userIter->second);
//			pDB.m_password = std::stoi(passwordIter->second);
//			//if (std::to_string(m_db.get<PlayerDatabase>(userIter->second).m_username)==userIter->second)
//			//{
//			//	return crow::response(409);
//			//}
//			m_db.replace(pDB);
//			//to sync schemas after insertions
//		}
//		return crow::response(200); // row was created and added
//	}
//
//private:
//	Storage& m_db;
//
//};

namespace database {
	template<class T>
	void static insertPlayer(T& storage, Player p)
	{
		PlayerDatabase pDB(p);
		//used .replace() instead of .insert() because we do not have autoincrement primary_key
		//in player table (primary key is usernam)
		storage.replace(pDB);
	}
	template<class T>
	crow::response static addNewPlayer(T& storage, const crow::request& req)
	{
		auto bodyArgs = parseRequestBody(req.body);  //id=2&quantity=3&...
		auto end = bodyArgs.end();
		auto userIter = bodyArgs.find("username");
		auto passwordIter = bodyArgs.find("password");

		if (userIter != end && passwordIter != end)
		{
			PlayerDatabase pDB;
			pDB.m_username = userIter->second;
			pDB.m_password = passwordIter->second;
			storage.replace(pDB);
			//to sync schemas after insertions
		}
		return crow::response(200); // row was created and added
	}
}

