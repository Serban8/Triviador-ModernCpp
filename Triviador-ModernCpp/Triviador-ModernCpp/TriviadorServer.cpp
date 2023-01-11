#include <sqlite_orm/sqlite_orm.h>
#include <crow.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>

#include "utils.h"

#include "MultipleChoiceQuestion.h"
#include "QuestionGenerator.h"
#include "QuestionDatabase.h"
#include "NumberQuestion.h"

#include "Game.h"
#include "Player.h"
#include "Map.h"

#include "Database.h"
#include "PlayerDatabase.h"
#include "QuestionDatabase.h"

#include "AppManager.h"

void databaseTest()
{
	//creating the database
	auto storage = createStorage("TRIV");
	storage.sync_schema();
	storage.remove_all<QuestionDatabase>();

	//inserting questions in the database
	database::generateAndInsertQuestions(storage);
	auto questions = storage.get_all<QuestionDatabase>();
	for (auto& q : questions)
	{
		std::cout << q.m_id << " " << q.m_question << std::endl;
	}

	//inserting players into database
	storage.replace(PlayerDatabase(Player("Flo")));
	storage.replace(PlayerDatabase(Player("Dany")));
	storage.replace(PlayerDatabase(Player("Jimmy")));
	auto players = storage.get_all<PlayerDatabase>();
	for (auto& p : players)
	{
		std::cout << p.m_username << std::endl;
	}
	auto p = storage.get<PlayerDatabase>("Flo");
	std::cout << p.m_username << std::endl;

	//insertin games into database
	storage.insert(GameDatabase("Dany", 5));
	storage.insert(GameDatabase("Jimmy", 6));
	storage.insert(GameDatabase("Flo", 4));

	auto g1 = storage.get<GameDatabase>(1);
	auto g2 = storage.get<GameDatabase>(2);
	auto g3 = storage.get<GameDatabase>(3);
	std::cout << g1.m_id << " " << g1.m_winner << std::endl;
	std::cout << g2.m_id << " " << g2.m_winner << std::endl;
	std::cout << g3.m_id << " " << g3.m_winner << std::endl;

	std::vector<MultipleChoiceQuestion> resultedQ;
	auto tmpQ = database::getMultipleChoiceQuestions(storage);
	resultedQ.insert(resultedQ.end(), tmpQ.begin(), tmpQ.end());
	tmpQ = database::getMultipleChoiceQuestions(storage);
	resultedQ.insert(resultedQ.end(), tmpQ.begin(), tmpQ.end());

	for (auto q : resultedQ) {
		std::cout << q.GetQuestion() << " " << q.GetCorrectAnswer() << std::endl;
	}

	std::vector<std::variant<NumberQuestion<int>, NumberQuestion<float>>> resultedNQ;
	auto tmpNQ = database::getNumberQuestions(storage);
	resultedNQ.insert(resultedNQ.end(), tmpNQ.begin(), tmpNQ.end());
	tmpNQ = database::getNumberQuestions(storage);
	resultedNQ.insert(resultedNQ.end(), tmpNQ.begin(), tmpNQ.end());

	for (auto q : resultedNQ) {
		if (std::holds_alternative< NumberQuestion<int>>(q)) {
			NumberQuestion<int> intQ = std::get<NumberQuestion<int>>(q);
			std::cout << std::endl << intQ.GetCorrectAnswer() << " " << intQ.GetQuestion();
		}
		else {
			NumberQuestion<float> floatQ = std::get<NumberQuestion<float>>(q);
			std::cout << std::endl << floatQ.GetCorrectAnswer() << " " << floatQ.GetQuestion();
		}
	}
	storage.sync_schema();
}
void questionTest()
{
	QuestionGenerator qg;
	auto questions = qg.GenerateNumberAnswerQuestions();

	std::cout << "\n\n--------------\n\n";

	std::cout << questions.size() << std::endl;

	for (auto q : questions) {
		if (std::holds_alternative< NumberQuestion<int>>(q)) {
			NumberQuestion<int> intQ = std::get<NumberQuestion<int>>(q);
			std::cout << std::endl << intQ.GetCorrectAnswer() << " " << intQ.GetQuestion();
		}
		else {
			NumberQuestion<float> floatQ = std::get<NumberQuestion<float>>(q);
			std::cout << std::endl << floatQ.GetCorrectAnswer() << " " << floatQ.GetQuestion();
		}
	}

	std::array<int, 3> numericIncorrectAnswers = { 2000, 2001, 2002 };
	NumberQuestion<int> nq("What was the year Max Verstappen won his first WDC ? ", "Sports", 2021, numericIncorrectAnswers);
	std::cout << std::endl << nq.GetQuestion() << " " << nq.GetCorrectAnswer();
}
void connectionTest()
{
	auto storage = createStorage("TRIV");
	storage.sync_schema();
	std::vector<MultipleChoiceQuestion> resultedQ;
	auto tmpQ = database::getMultipleChoiceQuestions(storage);
	crow::SimpleApp app;
	CROW_ROUTE(app, "/question")([&storage]() {
		std::vector<crow::json::wvalue> questionsJson;
		for (const auto& q : storage.iterate<QuestionDatabase>())
		{
			questionsJson.push_back(crow::json::wvalue{
				{"Question", q.m_question},
				{"Correct Answer", q.m_correctAnswer},
				{"Type", q.m_type}
				});
		}
		return crow::json::wvalue(questionsJson);
		});

	CROW_ROUTE(app, "/addnewplayer")
		.methods(crow::HTTPMethod::PUT)([&storage](const crow::request& req) {
		return database::addNewPlayer(storage, req);
			});

	app.port(18080).multithreaded().run();
}

int main()
{
	///TESTS
		//databaseTest();
		//playerTest();
		//questionTest();
		//mapTest();
		//gameTest();
		//connectionTest();
	///

	crow::SimpleApp app; //transmis si retinut in camp prinn referinta in clasa cu rutele
	auto storage = createStorage("TRIV");
	storage.sync_schema();
	database::populateDatabaseWithQuestions(storage);

	//login-related routes
	CROW_ROUTE(app, "/addnewplayer")
		.methods(crow::HTTPMethod::PUT)([&storage](const crow::request& req) {
		return database::addNewPlayer(storage, req);
			});
	CROW_ROUTE(app, "/checkplayer")
		.methods(crow::HTTPMethod::PUT)([&storage](const crow::request& req) {
		return database::checkPlayer(storage, req);
			});

	//waiting room & related routes
	std::vector<Player> waitingRoomList /*= { Player("Gigi"), Player("Marci"), Player("Luci"), Player("Cici")}*/; //initialization list for testing only
	std::vector<Player> votesToStart;

	CROW_ROUTE(app, "/addtowaitingroom")
		.methods(crow::HTTPMethod::PUT)([&waitingRoomList, &storage](const crow::request& req) {

		auto bodyArgs = parseRequestBody(req.body);
		auto bodyEnd = bodyArgs.end();
		auto usernameIter = bodyArgs.find("username");
		auto& username = usernameIter->second;
		if (usernameIter != bodyEnd)
		{
			waitingRoomList.push_back(Player(username));
		}
		else
		{
			return crow::response(400, "BAD REQUEST");
		}
		return crow::response(200);
			});

	CROW_ROUTE(app, "/checkwaitingroom")([&waitingRoomList, &votesToStart]() {
		std::vector<crow::json::wvalue> waitingRoomList_json;
		bool startGame = false;

		//check if conditions for starting the game are fulfilled
		if (waitingRoomList.size() == maxPlayersPerGame || votesToStart.size() == waitingRoomList.size()) {
			startGame = true;
		}

		//write the response json
		for (const auto& player : waitingRoomList) {
			bool votedToStart = false;
			//check if current player has voted to start
			if (std::find(votesToStart.begin(), votesToStart.end(), player) != votesToStart.end()) {
				votedToStart = true;
			}
			//write the json for the current player
			waitingRoomList_json.push_back(crow::json::wvalue{
				{"username", player.GetUsername()}, //todo: insert here other different stats: w/l ratio, number of games played, etc..
				{"votedToStart", votedToStart ? "true" : "false"}
				});
		}
		//add the startGame flag to the response
		waitingRoomList_json.push_back(crow::json::wvalue{
			{"startGame", startGame ? "true" : "false"}
			});

		return crow::json::wvalue{ waitingRoomList_json };
		});

	CROW_ROUTE(app, "/addvote")
		.methods(crow::HTTPMethod::PUT)([&waitingRoomList, &votesToStart](const crow::request& req) {

		auto bodyArgs = parseRequestBody(req.body);
		auto bodyEnd = bodyArgs.end();

		auto usernameIter = bodyArgs.find("username");
		auto& username = usernameIter->second;

		//check if it is a bad request
		if (usernameIter != bodyEnd)
		{
			auto playerIter = std::find(waitingRoomList.begin(), waitingRoomList.end(), Player(username));
			//check if the player is in the waiting room
			if (playerIter != waitingRoomList.end())
			{
				votesToStart.push_back(*playerIter);
			}
			else
			{
				CROW_LOG_INFO << "Did not find player with username \"" << username << "\" in waiting room";
				return crow::response(404, "NOT FOUND");
			}
		}
		else
		{
			return crow::response(400, "BAD REQUEST");
		}
		return crow::response(200);
			});

	//game history
	CROW_ROUTE(app, "/player/<string>")([&storage](std::string username)
		{
			std::vector<crow::json::wvalue> games_json;
			auto playerGame = storage.get_all<PlayerGameDatabase>(sqlite_orm::where(sqlite_orm::c(&PlayerGameDatabase::m_playerId) = username));
			for (const auto& pg : playerGame)
			{
				GameDatabase g = storage.get<GameDatabase>(pg.m_gameId);
				games_json.push_back(crow::json::wvalue{
					{"date", g.m_date },
					{"rounds", std::to_string(g.m_rounds)},
					{"winner", g.m_winner}
					});
			}
			return crow::json::wvalue{ games_json };
		});

	//game logic related routes

	//initializing the game
	Game game;
	CROW_ROUTE(app, "/startgame")([&game, &waitingRoomList, &storage] {
		if (!waitingRoomList.empty()) {
			game = Game(waitingRoomList, database::getNumberQuestions(storage, 15), database::getMultipleChoiceQuestions(storage, 30)); //to be initialized with waiting players list
			return crow::response(200);
		}
		return crow::response(500, "INTERNAL SERVER ERROR");
		});
	CROW_ROUTE(app, "/getplayers")([&game] {
		std::vector<crow::json::wvalue> players_json;
		std::vector<Player> players = game.GetActivePlayers();
		for (auto& p : players)
		{
			players_json.push_back(crow::json::wvalue{
				{"username", p.GetUsername()}
				});
		}
		return crow::json::wvalue{ players_json };
		});

	CROW_ROUTE(app, "/getnumberquestion")([&game]() {
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
		});

	CROW_ROUTE(app, "/getmultiplechoicequestion")([&game]() {
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
		});

	CROW_ROUTE(app, "/getmap")([&game]() {
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
		});

	CROW_ROUTE(app, "/setregionowner")
		.methods(crow::HTTPMethod::PUT)([&game](const crow::request& req) {
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
			});

	CROW_ROUTE(app, "/setregiontype")
		.methods(crow::HTTPMethod::PUT)([&game](const crow::request& req) {
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
			});

	CROW_ROUTE(app, "/increaseregionscore")
		.methods(crow::HTTPMethod::PUT)([&game](const crow::request& req) {
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
			});

	CROW_ROUTE(app, "/decreaseregionscore")
		.methods(crow::HTTPMethod::PUT)([&game](const crow::request& req) {
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
			});

	std::map<numberQuestionResponse, std::unique_ptr<Player>, compareNumberQuestionResponses> leaderboard;
	
	CROW_ROUTE(app, "/addresponse")
		.methods(crow::HTTPMethod::PUT)([&leaderboard, &game](const crow::request& req) {

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
			});
	
	CROW_ROUTE(app, "/getleaderboard")([&leaderboard]() {
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
		});

	//make player order
	////route that gives the player all available territories to attack

	app.port(18080).multithreaded().run();

	return 0;
}