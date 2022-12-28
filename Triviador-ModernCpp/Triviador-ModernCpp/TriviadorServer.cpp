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
#include "PlayerDatabase.h"
#include "NumberQuestion.h"
#include "Game.h"
#include "Player.h"
#include "Map.h"
#include "GameDatabase.h"
#include "PlayerGameDatabase.h"

namespace sql = sqlite_orm;

const uint8_t maxPlayersPerGame = 4;

inline auto createStorage(const std::string& filename)
{
	return sql::make_storage(
		filename,
		sql::make_table("Players",
			sql::make_column("username", &PlayerDatabase::m_username, sql::primary_key()),//or player, need to see
			sql::make_column("password", &PlayerDatabase::m_password)
			//instead of an column we will make a query to get no. of  games played
		),
		sql::make_table("Questions",
			sql::make_column("id", &QuestionDatabase::m_id, sql::autoincrement(), sql::primary_key()),
			sql::make_column("question", &QuestionDatabase::m_question),
			sql::make_column("category", &QuestionDatabase::m_category),
			sql::make_column("type", &QuestionDatabase::m_type),
			sql::make_column("correctAnswer", &QuestionDatabase::m_correctAnswer),
			sql::make_column("incorrectAnswer1", &QuestionDatabase::m_incorrectAnswer1),
			sql::make_column("incorrectAnswer2", &QuestionDatabase::m_incorrectAnswer2),
			sql::make_column("incorrectAnswer3", &QuestionDatabase::m_incorrectAnswer3)
		),
		sql::make_table("Game",
			sql::make_column("id", &GameDatabase::m_id, sql::autoincrement(), sql::primary_key()),
			sql::make_column("winner", &GameDatabase::m_winner),
			sql::make_column("rounds", &GameDatabase::m_rounds),
			sql::make_column("date", &GameDatabase::m_date),
			sql::foreign_key(&GameDatabase::m_winner).references(&PlayerDatabase::m_username)
		),
		sql::make_table("PlayerGames",
			sql::make_column("game", &PlayerGameDatabase::m_gameId),
			sql::make_column("username", &PlayerGameDatabase::m_playerId),
			sql::primary_key(&PlayerGameDatabase::m_gameId, &PlayerGameDatabase::m_playerId),
			sql::foreign_key(&PlayerGameDatabase::m_gameId).references(&GameDatabase::m_id),
			sql::foreign_key(&PlayerGameDatabase::m_playerId).references(&PlayerDatabase::m_username)
		)
	);
}


void databaseTest()
{
	//creating the database
	using namespace sqlite_orm;
	auto storage = createStorage("TRIV");
	storage.sync_schema();
	storage.remove_all<QuestionDatabase>();

	//inserting questions in the database
	database::insertQuestions(storage);
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

	//database::game::getAllGames(storage);
	//
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

void playerTest() {
	Player p("marcel");
	//use debugger
	p.UseAdvantage(Player::Advantage::FiftyFifty);
}

void gameTest() {

	QuestionGenerator qg;
	std::vector<MultipleChoiceQuestion> mq = qg.GenerateMultipleChoiceQuestions(10);

	std::vector<Question*> aq;

	for (auto& question : mq) {

		aq.push_back(static_cast<Question*>(&question));
	}

	Player p1("marcel");
	Player p2("gigel");
	Player p3("costel");
	Player p4("cosmeenel");
	p1.SetPoints(400);
	p2.SetPoints(100);
	p3.SetPoints(500);
	p4.SetPoints(800);
	std::vector<Player> player = { p1,p2,p3,p4 };
	Game topG(player);
	topG.PlayGame();
	topG.DetermineWinners();
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
void mapTest()
{
	Map m1(2);
	Map m2(3);
	Map m3(4);
	std::cout << m3;
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

	crow::SimpleApp app;
	auto storage = createStorage("TRIV");
	storage.sync_schema();
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
	std::vector<Player> waitingRoomList = { Player("Gigi"), Player("Marci"), Player("Luci") }; //initialization list for testing only
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
	//testing purposes only initialization
	Game game(waitingRoomList);
	//initializing the game
	CROW_ROUTE(app, "/startgame")([&game, &waitingRoomList] {
		if (!waitingRoomList.empty()) {
			game = Game(waitingRoomList); //to be initialized with waiting players list
			return crow::response(200); //OK
		}
		return crow::response(500); //internal server error
		});
	CROW_ROUTE(app, "/getplayers")([&game] {
		std::vector<crow::json::wvalue> players_json;
		std::vector<Player> players = game.GetPlayers();
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

		if (requestCounter == game.GetPlayers().size() || requestCounter == 0) {
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
		static MultipleChoiceQuestion question("", "", "", std::array<std::string, 3>{"", "", ""});
		crow::json::wvalue questionJson;

		if (requestCounter == game.GetPlayers().size() || requestCounter == 0) {
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

	app.port(18080).multithreaded().run();

	return 0;
}