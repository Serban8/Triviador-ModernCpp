#pragma once
#include <crow.h>
#include <sqlite_orm/sqlite_orm.h>

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

namespace database {
	template<class T>
	void static insertPlayer(T& storage, Player p)
	{
		PlayerDatabase pDB(p);
		//used .replace() instead of .insert() because we do not have autoincrement primary_key
		//in player table (primary key is username)
		storage.replace(pDB);
	}
	template<class T>
	crow::response static addNewPlayer(T& storage, const crow::request& req)
	{
		namespace sql = sqlite_orm;
		auto bodyArgs = parseRequestBody(req.body);  //id=2&quantity=3&...
		auto end = bodyArgs.end();
		auto userIter = bodyArgs.find("username");
		auto passwordIter = bodyArgs.find("password");
		auto playersCount = storage.get_all<PlayerDatabase>(sql::where(sql::c(&PlayerDatabase::m_username) = userIter->second));
		if (playersCount.size() == 1)
		{
			return crow::response(409, "CONFLICT");
		}
		if (userIter != end && passwordIter != end)
		{
			PlayerDatabase pDB;
			pDB.m_username = userIter->second;
			pDB.m_password = passwordIter->second;
			storage.replace(pDB);
		}
		return crow::response(200); // row was created and added
	}
	template<class T>
	crow::response static checkPlayer(T& storage, const crow::request& req)
	{
		namespace sql = sqlite_orm;
		auto bodyArgs = parseRequestBody(req.body);  //id=2&quantity=3&...
		auto end = bodyArgs.end();
		auto userIter = bodyArgs.find("username");
		auto passwordIter = bodyArgs.find("password");
		if (userIter != end && passwordIter != end)
		{
			auto playersCount = storage.get_all<PlayerDatabase>(sql::where(sql::c(&PlayerDatabase::m_username) = userIter->second));
			if (playersCount.size() != 1)
			{
				return crow::response(404, "NOT FOUND");
			}
			if (playersCount[0].m_password != passwordIter->second)
			{
				return crow::response(401, "UNAUTHORIZED");
			}
		}
		else if (passwordIter == end)
		{
			return crow::response(401, "UNAUTHORIZED");
		}
		else {
			return crow::response(404, "NOT FOUND");
		}
		return crow::response(200);
	}
}

