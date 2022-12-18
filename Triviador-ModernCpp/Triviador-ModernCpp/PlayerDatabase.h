#pragma once
#include <crow.h>
#include <sqlite_orm/sqlite_orm.h>

#include "Player.h"
#include "utils.h"

namespace sql = sqlite_orm;

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
	crow::response static addNewPlayer(T& storage, const crow::request& req)
	{
		auto bodyArgs = parseRequestBody(req.body);
		auto bodyEnd = bodyArgs.end();

		PlayerDatabase pDB;
		auto userIter = bodyArgs.find("username");
		pDB.m_username = userIter->second;
		auto passwordIter = bodyArgs.find("password");
		pDB.m_password = passwordIter->second;

		//check for bad request
		if (userIter != bodyEnd && passwordIter != bodyEnd) {
			//check if username already exists in DB, and 
			//	return 409 conflict if yes 
			//	insert new player if no
			if (storage.get_pointer<PlayerDatabase>(pDB.m_username)) {
				CROW_LOG_INFO << "Found player with username \"" << pDB.m_username << "\" in database";
				return crow::response(409, "CONFLICT");
			}

			storage.replace(pDB);
		}
		else {
			return crow::response(400, "BAD REQUEST");
		}
		return crow::response(200); // row was created and added
	}
	template<class T>
	crow::response static checkPlayer(T& storage, const crow::request& req)
	{
		auto bodyArgs = parseRequestBody(req.body);
		auto bodyEnd = bodyArgs.end();

		auto userIter = bodyArgs.find("username");
		auto& username = userIter->second;
		auto passwordIter = bodyArgs.find("password");
		auto& password = passwordIter->second;

		//check for bad request
		if (userIter != bodyEnd && passwordIter != bodyEnd)
		{
			//get all players with username that matches and
			//	if player.size() != 1 return 404 NOT FOUND
			//	if player.size() == 1 but players[0].m_password != password return 401 UNAUTHORIZED
			auto player = storage.get_pointer<PlayerDatabase>(username);
			if (!player)
			{
				CROW_LOG_INFO << "Did not find player with username \"" << username << "\" in database";
				return crow::response(404, "NOT FOUND");
			}
			if (player->m_password != password)
			{
				CROW_LOG_INFO << "Incorrect password";
				return crow::response(401, "UNAUTHORIZED");
			}
		}
		else {
			return crow::response(400, "BAD REQUEST");
		}
		return crow::response(200);
	}
}

