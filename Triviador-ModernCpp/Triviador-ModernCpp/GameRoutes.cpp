#include "GameRoutes.h"

//AddResponseHandler
AddResponseHandler::AddResponseHandler(ServerStatus& status, Game& game, std::shared_ptr<Region> attackedRegion, std::map<numberQuestionResponse, std::shared_ptr<Player>, compareNumberQuestionResponses>& leaderboard) :
	status(status), game(game), attackedRegion(attackedRegion), leaderboard(leaderboard)
{
}
crow::response AddResponseHandler::operator()(const crow::request& req) const
{
	static uint8_t responseCounter = 0;
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
		leaderboard.emplace(std::make_pair(response, time), std::make_shared<Player>(game[username]));
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

	++responseCounter;
	bool resetCounter = false;
	
	if (game.GetPhase() == Game::Phase::CHOOSING_BASES || game.GetPhase() == Game::Phase::CHOOSING_REGIONS) {
		if (responseCounter == game.GetActivePlayers().size()) {
			resetCounter = true;
		}
	}
	else {
		if (responseCounter == 2) {
			attackedRegion.get()->UpdateRegion(leaderboard.begin()->second);
			resetCounter = true;
		}
	}
	
	if (resetCounter)
	{
		CROW_LOG_INFO << "---";
		CROW_LOG_INFO << "Status has been set to DISPLAY_LEADERBOARD";
		CROW_LOG_INFO << "---";
		status = ServerStatus::DISPLAY_LEADERBOARD;
		responseCounter = 0;
	}

	return crow::response(200);
}
//

//GetMultipleChoiceQuestionHandler
GetMultipleChoiceQuestionHandler::GetMultipleChoiceQuestionHandler(ServerStatus& status, Player& attacker, Player& attacked, Game& game) :
	status(status), attacker(attacker), attacked(attacked), game(game)
{
}

crow::json::wvalue GetMultipleChoiceQuestionHandler::operator() (const crow::request& req) const
{

	static uint8_t requestCounter = 0;
	static MultipleChoiceQuestion question = game.GetMultipleChoiceQuestion();
	crow::json::wvalue questionJson;
	auto bodyArgs = parseRequestBody(req.body);
	auto bodyEnd = bodyArgs.end();
	auto usernameIter = bodyArgs.find("username");

	if (usernameIter != bodyEnd) {
		auto& username = usernameIter->second;
		bool allowAnswer;
		if (username == attacker.GetUsername() || username == attacked.GetUsername()) {
			allowAnswer = true;
		}
		else {
			allowAnswer = false;
		}
		questionJson = crow::json::wvalue{
			{"question", question.GetQuestion()},
			{"category", question.GetCategory()},
			{"correctAnswer", question.GetCorrectAnswer()},
			{"incorrectAnswer1", question.GetIncorrectAnswers()[0]},
			{"incorrectAnswer2", question.GetIncorrectAnswers()[1]},
			{"incorrectAnswer3", question.GetIncorrectAnswers()[2]},
			{"allowAnswer", allowAnswer}
		};

		++requestCounter;
		if (requestCounter == game.GetActivePlayers().size()) {
			question = game.GetMultipleChoiceQuestion();
			CROW_LOG_INFO << "---";
			CROW_LOG_INFO << "Status has been set to DISPLAY_QUESTION";
			CROW_LOG_INFO << "---";
			status = ServerStatus::DISPLAY_QUESTION;
			requestCounter = 0;
		}
		return crow::json::wvalue{ questionJson };
	}
	else {
		//return 500 error
	}
}
//

//GetNumberQuestionHandler
GetNumberQuestionHandler::GetNumberQuestionHandler(ServerStatus& status, Game& game) : status(status), game(game)
{
}

crow::json::wvalue GetNumberQuestionHandler::operator() () const
{
	static uint8_t requestCounter = 0;
	static std::variant<NumberQuestion<int>, NumberQuestion<float>> question = game.GetNumberQuestion();
	crow::json::wvalue questionJson;

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

	++requestCounter;
	if (requestCounter == game.GetActivePlayers().size()) {
		question = game.GetNumberQuestion();
		CROW_LOG_INFO << "---";
		CROW_LOG_INFO << "Status has been set to DISPLAY_QUESTION";
		CROW_LOG_INFO << "---";
		status = ServerStatus::DISPLAY_QUESTION;
		requestCounter = 0;
	}

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
StartGameHandler::StartGameHandler(ServerStatus& status, Game& game, std::vector<Player>& waitingRoomList, Storage& storage) :
	status(status), game(game), waitingRoomList(waitingRoomList), storage(storage)
{
}

crow::response StartGameHandler::operator() () const
{
	static uint8_t requestCounter = 0;
	static bool initDone = false;  //retriving questions from the DB takes some time, so we need to make sure that the game has been propperly initialized
	++requestCounter;
	CROW_LOG_INFO << "---";
	CROW_LOG_INFO << "REQ_C: " << (int)requestCounter;
	CROW_LOG_INFO << "---";
	if (!waitingRoomList.empty() && requestCounter == 1) { //only initialize game when the first request is received
		game = Game(waitingRoomList, database::getNumberQuestions(storage, 15), database::getMultipleChoiceQuestions(storage, 30));
		CROW_LOG_INFO << "---";
		CROW_LOG_INFO << "Game has been initialized with " << waitingRoomList.size() << " players";
		CROW_LOG_INFO << "---";
		initDone = true;
	}

	if (requestCounter == waitingRoomList.size() && initDone) {
		CROW_LOG_INFO << "---";
		CROW_LOG_INFO << "Status has been set to GET_NUMBER_QUESTION";
		CROW_LOG_INFO << "---";
		status = ServerStatus::GET_NUMBER_QUESTION;
		requestCounter = 0;
		return crow::response(200);
	}

	return crow::response(500, "INTERNAL SERVER ERROR");
}
//

//SetAttackedTerritoryHandler
SetAttackedTerritoryHandler::SetAttackedTerritoryHandler(ServerStatus& status, Player& attacked, Game& game, std::shared_ptr<Region>& attackedRegion) :
	status(status), attacked(attacked), game(game), attackedRegion(attackedRegion)
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

		attackedRegion = game.GetRegion({ line, column });
		if (!attackedRegion) {
			return crow::response(400, "BAD REQUEST");
		}
		CROW_LOG_INFO << "---";
		CROW_LOG_INFO << "Status has been set to ATTACKER_CHOSE_REGION";
		CROW_LOG_INFO << "---";
		status = ServerStatus::ATTACKER_CHOSE_REGION;
		attacked = *attackedRegion.get()->GetOwner(); //te be pointer in future

		return crow::response(200);
	}
}
//

//GetLeaderboardHandler
GetLeaderboardHandler::GetLeaderboardHandler(ServerStatus& status, Game& game, std::map<numberQuestionResponse, std::shared_ptr<Player>, compareNumberQuestionResponses>& leaderboard) :
	status(status), game(game), leaderboard(leaderboard)
{
}

crow::json::wvalue GetLeaderboardHandler::operator() () const
{
	static uint8_t requestCounter = 0;

	std::vector<crow::json::wvalue> players_json;
	
	int place = 1;
	for (const auto& player : leaderboard)
	{
		players_json.push_back(crow::json::wvalue{
			{"place" + std::to_string(place++), player.second.get()->GetUsername()}
			});
	}

	++requestCounter;
	if (requestCounter == game.GetActivePlayers().size()) {
		requestCounter = 0;
		CROW_LOG_INFO << "---";
		CROW_LOG_INFO << "Status has been set to WAIT";
		CROW_LOG_INFO << "---";
		status = ServerStatus::WAIT;
	}

	//Reset leaderboard
	//leaderboard.clear();
	return crow::json::wvalue{ players_json };
}
//
