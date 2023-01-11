#include "GameRoutes.h"

//AddResponseHandler
AddResponseHandler::AddResponseHandler(Game& game, std::map<numberQuestionResponse, std::unique_ptr<Player>, compareNumberQuestionResponses>& leaderboard) : game(game), leaderboard(leaderboard)
{
}
crow::response AddResponseHandler::operator()(const crow::request& req) const
{
	auto bodyArgs = parseRequestBody(req.body);
	auto bodyEnd = bodyArgs.end();
	auto usernameIter = bodyArgs.find("username");
	auto responseIter = bodyArgs.find("response");
	auto timeIter = bodyArgs.find("time");

	if (usernameIter != bodyEnd && responseIter != bodyEnd && timeIter != bodyEnd)
	{
		size_t pos;
		auto& username = usernameIter->second;

		//try to convert response to float
		auto& res = responseIter->second;
		float response = stof(res, &pos);
		if (pos != res.size()) {
			return crow::response(400, "BAD REQUEST");
		}

		//try to convert time to float
		auto& t = timeIter->second;
		float time = stof(t, &pos);
		if (pos != t.size()) {
			return crow::response(400, "BAD REQUEST");
		}
		CROW_LOG_INFO << "Receieved answer from player: " << username << "; distance from the correct response: " << response << "; time took to answer: " << time;
		leaderboard.emplace(std::make_pair(response, time), std::make_unique<Player>(game[username]));
	}
	else
	{
		return crow::response(400, "BAD REQUEST");
	}

	//TESTING
	for (const auto& player : leaderboard) {
		auto& [key, value] = player;
		auto& [response, time] = key;
		CROW_LOG_INFO << "RESPONSE: " << response << " TIME: " << time << " USERNAME: " << value.get()->GetUsername();
	}
	//
	return crow::response(200);
}
//

//GetMultipleChoiceQuestionHandler
GetMultipleChoiceQuestionHandler::GetMultipleChoiceQuestionHandler(Game& game) : game(game)
{
}

crow::json::wvalue GetMultipleChoiceQuestionHandler::operator() () const
{
	static uint8_t requestCounter = 0;
	static MultipleChoiceQuestion question;
	crow::json::wvalue questionJson;

	//if all players have received the question (or is first question) get the next question.
	if (requestCounter == game.GetActivePlayers().size() || requestCounter == 0) {
		question = game.GetMultipleChoiceQuestion();
		requestCounter = 0;
	}
	questionJson = crow::json::wvalue{
		{"question", question.GetQuestion()},
		{"category", question.GetCategory()},
		{"correctAnswer", question.GetCorrectAnswer()},
		{"incorrectAnswer1", question.GetIncorrectAnswers()[0]},
		{"incorrectAnswer2", question.GetIncorrectAnswers()[1]},
		{"incorrectAnswer3", question.GetIncorrectAnswers()[2]}
	};

	requestCounter++;
	return crow::json::wvalue{ questionJson };
}
//

//GetNumberQuestionHandler
GetNumberQuestionHandler::GetNumberQuestionHandler(Game& game) : game(game)
{
}

crow::json::wvalue GetNumberQuestionHandler::operator() () const
{
	static uint8_t requestCounter = 0;
	static std::variant<NumberQuestion<int>, NumberQuestion<float>> question;
	crow::json::wvalue questionJson;

	if (requestCounter == game.GetActivePlayers().size() || requestCounter == 0) {
		question = game.GetNumberQuestion();
		requestCounter = 0;
	}
	if (std::holds_alternative<NumberQuestion<int>>(question))
	{
		auto qInt = std::get<NumberQuestion<int>>(question);
		questionJson = crow::json::wvalue{
			{"question", qInt.GetQuestion()},
			{"category", qInt.GetCategory()},
			{"correctAnswer", qInt.GetCorrectAnswer()},
			{"incorrectAnswer1", qInt.GetIncorrectAnswers()[0]},
			{"incorrectAnswer2", qInt.GetIncorrectAnswers()[1]},
			{"incorrectAnswer3", qInt.GetIncorrectAnswers()[2]},
		};
	}
	else {
		auto qFloat = std::get<NumberQuestion<float>>(question);
		questionJson = crow::json::wvalue{
			{"question", qFloat.GetQuestion()},
			{"category", qFloat.GetCategory()},
			{"correctAnswer", qFloat.GetCorrectAnswer()},
			{"incorrectAnswer1", qFloat.GetIncorrectAnswers()[0]},
			{"incorrectAnswer2", qFloat.GetIncorrectAnswers()[1]},
			{"incorrectAnswer3", qFloat.GetIncorrectAnswers()[2]},
		};
	}

	requestCounter++;
	return crow::json::wvalue{ questionJson };
}
//

//GetPlayersHandler
GetPlayersHandler::GetPlayersHandler(Game& game) : game(game)
{
}

crow::json::wvalue GetPlayersHandler::operator() () const
{
	std::vector<crow::json::wvalue> players_json;
	std::vector<Player> players = game.GetActivePlayers();
	for (auto& p : players)
	{
		players_json.push_back(crow::json::wvalue{
			{"username", p.GetUsername()}
			});
	}
	return crow::json::wvalue{ players_json };
}
//

//StartGameHandler
StartGameHandler::StartGameHandler(Game& game, std::vector<Player>& waitingRoomList, Storage& storage) : game(game), waitingRoomList(waitingRoomList), storage(storage)
{
}

crow::response StartGameHandler::operator() () const
{
	if (!waitingRoomList.empty()) {
		game = Game(waitingRoomList, database::getNumberQuestions(storage, 15), database::getMultipleChoiceQuestions(storage, 30));
		return crow::response(200);
	}
	return crow::response(500, "INTERNAL SERVER ERROR");
}
//

//SetAttackedTerritoryHandler
SetAttackedTerritoryHandler::SetAttackedTerritoryHandler(Game& game, std::shared_ptr<Region> attackedRegion) : game(game), attackedRegion(attackedRegion)
{
}

crow::response SetAttackedTerritoryHandler::operator()(const crow::request& req) const
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
		uint8_t line = std::stoi(lineStr, &pos);
		if (pos != lineStr.size()) {
			return crow::response(400, "BAD REQUEST");
		}

		//try to convert column to int
		uint8_t column = std::stoi(columnStr, &pos);
		if (pos != columnStr.size()) {
			return crow::response(400, "BAD REQUEST");
		}

		//attackedRegion = game.GetRegion({ line, column });
		if (!attackedRegion) {
			return crow::response(400, "BAD REQUEST");
		}
	}
}
//

//GetLeaderboardHandler
GetLeaderboardHandler::GetLeaderboardHandler(std::map<numberQuestionResponse, std::unique_ptr<Player>, compareNumberQuestionResponses>& leaderboard) : leaderboard(leaderboard)
{
}

crow::json::wvalue GetLeaderboardHandler::operator() () const
{
	std::vector<crow::json::wvalue> players_json;
	for (const auto& player : leaderboard)
	{
		players_json.push_back(crow::json::wvalue{
			{"username", player.second.get()->GetUsername()}
			});
	}
	//Reset leaderboard
	leaderboard.clear();
	return crow::json::wvalue{ players_json };
}
//
