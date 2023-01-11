#include "MapRoutes.h"

//GetMapHandler
GetMapHandler::GetMapHandler(Game& game) : game(game)
{
}

crow::json::wvalue GetMapHandler::operator() () const
{
	std::vector<crow::json::wvalue> mapJson;
	Map routeMap = game.GetMap();
	for (uint8_t i = 0; i < routeMap.GetHeight(); i++) {
		for (uint8_t j = 0; j < routeMap.GetWidth(); j++) {
			mapJson.push_back(crow::json::wvalue{
				{"line", i},
				{"column", j},
				{"owner", routeMap[{i, j}].GetOwner().GetUsername()},
				{"score", routeMap[{i, j}].GetScore()},
				{"type", Region::RegionTypeToString(routeMap[{i,j}].GetType())}
				});
		}
	}
	return crow::json::wvalue{ mapJson };
};
//

//SetRegionOwnerHandler
SetRegionOwnerHandler::SetRegionOwnerHandler(Game& game): game(game)
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
		game.ModifyRegion({ line, column }, game[ownerIter->second]);
		//
	}
	else {
		return crow::response(400, "BAD REQUEST");
	}

	return crow::response(200);
}
//

//SetRegionTypeHandler
SetRegionTypeHandler::SetRegionTypeHandler(Game& game): game(game)
{
}
crow::response SetRegionTypeHandler::operator()(const crow::request& req) const
{
	auto bodyArgs = parseRequestBody(req.body);
	auto bodyEnd = bodyArgs.end();
	//getting the line and column
	auto lineIter = bodyArgs.find("line");
	auto columnIter = bodyArgs.find("column");

	//setting the new type
	auto typeIter = bodyArgs.find("type");
	if (lineIter != bodyEnd && columnIter != bodyEnd && typeIter != bodyEnd) {
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

		//set region type
		game.ModifyRegion({ line, column }, Region::StringToRegionType(typeIter->second));
	}
	else {
		return crow::response(400, "BAD REQUEST");
	}
	return crow::response(200);
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
	//getting the line and column
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
DecreaseRegionScoreHandler::DecreaseRegionScoreHandler(Game& game): game(game)
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