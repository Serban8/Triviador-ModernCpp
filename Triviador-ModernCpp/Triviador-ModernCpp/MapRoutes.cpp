#include "MapRoutes.h"

//GetMapHandler
GetMapHandler::GetMapHandler(ServerStatus& status, Game& game) : status(status), game(game)
{
}

crow::json::wvalue GetMapHandler::operator() () const
{
	static uint8_t responseCounter = 0;

	std::vector<crow::json::wvalue> mapJson;
	Map routeMap = game.GetMap();
	for (uint8_t i = 0; i < routeMap.GetHeight(); i++) {
		for (uint8_t j = 0; j < routeMap.GetWidth(); j++) {

			std::string owner;
			//determine if region has owner
			if (routeMap[{i, j}].GetOwner().get()) {
				routeMap[{i, j}].GetOwner().get()->GetUsername();
			}
			else {
				owner = "none";
			}

			mapJson.push_back(crow::json::wvalue{
				{"line", i},
				{"column", j},
				{"owner", owner},
				{"score", routeMap[{i, j}].GetScore()},
				{"type", Region::RegionTypeToString(routeMap[{i,j}].GetType())}
				});
		}
	}

	++responseCounter;
	//make sure that all clients receive the new map
	if (responseCounter == game.GetActivePlayers().size()) {
		//if all clients received the map and game phase is DUEL, then start the new round
		if (status == ServerStatus::WAIT && game.GetPhase() == Game::Phase::DUEL) {
			CROW_LOG_INFO << "---";
			CROW_LOG_INFO << "Status has been set to START_ROUND";
			CROW_LOG_INFO << "---";
			status = ServerStatus::START_ROUND;
		}
		else {
			CROW_LOG_INFO << "---";
			CROW_LOG_INFO << "Status has been set to WAIT";
			CROW_LOG_INFO << "---";
			status = ServerStatus::WAIT;
		}
		responseCounter = 0;
	}

	return crow::json::wvalue{ mapJson };
};
//

//SetRegionOwnerHandler
SetRegionOwnerHandler::SetRegionOwnerHandler(Game& game) : game(game)
{
}
crow::response SetRegionOwnerHandler::operator()(const crow::request& req) const
{
	auto bodyArgs = parseRequestBody(req.body);
	auto bodyEnd = bodyArgs.end();
	auto lineIter = bodyArgs.find("line");
	auto columnIter = bodyArgs.find("column");
	auto ownerIter = bodyArgs.find("owner");

	if (lineIter != bodyEnd && columnIter != bodyEnd && ownerIter != bodyEnd) {
		size_t pos;
		auto& lineStr = lineIter->second;
		auto& columnStr = columnIter->second;
		auto& ownerStr = ownerIter->second;

		//try to convert line to int
		int line = std::stoi(lineStr, &pos);
		if (pos != lineStr.size()) {
			return crow::response(400, "BAD REQUEST");
		}

		//try to convert column to int
		int column = std::stoi(columnStr, &pos);
		if (pos != columnStr.size()) {
			return crow::response(400, "BAD REQUEST");
		}

		//set region owner
		game.ModifyRegion({ line, column }, std::make_shared<Player>(game[ownerStr]));
		//
	}
	else {
		return crow::response(400, "BAD REQUEST");
	}

	return crow::response(200);
}
//

//SetRegionTypeHandler
InitializeRegionHandler::InitializeRegionHandler(ServerStatus& status, Game& game) : status(status), game(game)
{
}
crow::response InitializeRegionHandler::operator()(const crow::request& req) const
{
	auto bodyArgs = parseRequestBody(req.body);
	auto bodyEnd = bodyArgs.end();
	auto lineIter = bodyArgs.find("line");
	auto columnIter = bodyArgs.find("column");

	//setting the new type
	auto typeIter = bodyArgs.find("type");
	auto usernameIter = bodyArgs.find("username");

	if (lineIter != bodyEnd && columnIter != bodyEnd && typeIter != bodyEnd && usernameIter != bodyEnd) {
		size_t pos;
		auto& lineStr = lineIter->second;
		auto& columnStr = columnIter->second;
		auto& usernameStr = usernameIter->second;
		auto& typeStr = typeIter->second;

		//try to convert line to int
		int line = std::stoi(lineStr, &pos);
		if (pos != lineStr.size()) {
			return crow::response(400, "BAD REQUEST");
		}

		//try to convert column to int
		int column = std::stoi(columnStr, &pos);
		if (pos != columnStr.size()) {
			return crow::response(400, "BAD REQUEST");
		}

		//set region type & owner
		game.ModifyRegion(
			{ line, column },
			Region::StringToRegionType(typeStr),
			std::make_shared<Player>(game[usernameStr])
		);

		if (game.GetPhase() == Game::Phase::CHOOSING_BASES) {
			CROW_LOG_INFO << "---";
			CROW_LOG_INFO << "Status has been set to CHOSE_BASE";
			CROW_LOG_INFO << "---";
			status = ServerStatus::CHOSE_BASE;
		}
		else if (game.GetPhase() == Game::Phase::CHOOSING_REGIONS) {
			CROW_LOG_INFO << "---";
			CROW_LOG_INFO << "Status has been set to CHOSE_REGION";
			CROW_LOG_INFO << "---";
			status = ServerStatus::CHOSE_REGION;
		}
		return crow::response(200);
	}

	return crow::response(400, "BAD REQUEST");
}
//

//IncreaseRegionScoreHandler
IncreaseRegionScoreHandler::IncreaseRegionScoreHandler(Game& game) : game(game)
{
}
crow::response IncreaseRegionScoreHandler::operator()(const crow::request& req) const
{
	auto bodyArgs = parseRequestBody(req.body);
	auto bodyEnd = bodyArgs.end();
	auto lineIter = bodyArgs.find("line");
	auto columnIter = bodyArgs.find("column");

	if (lineIter != bodyEnd && columnIter != bodyEnd) {
		size_t pos;
		auto& lineStr = lineIter->second;
		auto& columnStr = columnIter->second;

		//try to convert line to int
		int line = std::stoi(lineStr, &pos);
		if (pos != lineStr.size()) {
			return crow::response(400, "BAD REQUEST");
		}

		//try to convert column to int
		int column = std::stoi(columnStr, &pos);
		if (pos != columnStr.size()) {
			return crow::response(400, "BAD REQUEST");
		}

		//increase score
		game.IncreaseRegionScore({ line, column });
	}
	else {
		return crow::response(400, "BAD REQUEST");
	}

	return crow::response(200);
}
//

//DecreaseRegionScoreHandler
DecreaseRegionScoreHandler::DecreaseRegionScoreHandler(Game& game) : game(game)
{
}
crow::response DecreaseRegionScoreHandler::operator()(const crow::request& req) const
{
	auto bodyArgs = parseRequestBody(req.body);
	auto bodyEnd = bodyArgs.end();
	auto lineIter = bodyArgs.find("line");
	auto columnIter = bodyArgs.find("column");

	if (lineIter != bodyEnd && columnIter != bodyEnd) {
		size_t pos;
		auto& lineStr = lineIter->second;
		auto& columnStr = columnIter->second;

		//try to convert line to int
		int line = std::stoi(lineStr, &pos);
		if (pos != lineStr.size()) {
			return crow::response(400, "BAD REQUEST");
		}

		//try to convert column to int
		int column = std::stoi(columnStr, &pos);
		if (pos != columnStr.size()) {
			return crow::response(400, "BAD REQUEST");
		}

		//decrease score
		game.DecreaseRegionScore({ line, column });
	}
	else {
		return crow::response(400, "BAD REQUEST");
	}
	return crow::response(200);
}
//