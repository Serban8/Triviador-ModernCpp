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
	/*database::question::getQuestion(storage,3102);
	database::question::getQuestion(storage,3167);
	database::question::getQuestion(storage,3169);
	database::question::getQuestion(storage,3133);*/

	//inserting players into database
	database::insertPlayer(storage, Player("Flo"));
	database::insertPlayer(storage, Player("Dany"));
	database::insertPlayer(storage, Player("Jimmy"));
	auto players = storage.get_all<PlayerDatabase>();
	for (auto& p : players)
	{
		std::cout << p.m_username << std::endl;
	}
	auto p = storage.get<PlayerDatabase>("Flo");
	std::cout << p.m_username << std::endl;
	//storage.get<PlayerDatabase>("Mirkea");

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
	//insert playerGames into database
	//database::playerGame::insertPlayer(storage, storage.get<GameDatabase>(where(c(&GameDatabase::m_winner)=="Flo")), PlayerDatabase(Player("Flo")));
	//database::playerGame::insertPlayer(storage, storage.get<GameDatabase>(where(c(&GameDatabase::m_winner)=="Jimmy")), PlayerDatabase(Player("Jimmy")));
	//database::playerGame::insertPlayer(storage, GameDatabase("Jimmy", 6), PlayerDatabase(Player("Jimmy")));

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

	/*for (auto& question : aq) {

		std::cout << question->GetQuestion()<<"\n";
	}*/
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

	/*CROW_ROUTE(app, "/addnewplayer/<string>")([&storage](const crow::request& req) {
		char* username_chr = req.url_params.get("username");
		char* password_chr = req.url_params.get("password");
		PlayerDatabase pDB(Player(username_chr), password_chr);
		try  {
			storage.replace(pDB);
			return crow::response(200);
		}
		catch (std::system_error e) {
			std::cout << e.what() << std::endl;
			return crow::response(409);
		}
		catch (...) {
			std::cout << "unknown exeption" << std::endl;
			return crow::response(409);
		}
	});*/

	CROW_ROUTE(app, "/addnewplayer")
		.methods(crow::HTTPMethod::PUT)([&storage](const crow::request& req) {
		return database::addNewPlayer(storage, req);
			});
	//addNewPlayerPut(database::addNewPlayer(storage);

	app.port(18080).multithreaded().run();
}
void mapTest()
{
	Map m1(2);
	Map m2(3);
	Map m3(4);
	//Map m6(6);
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

		//for testing
	std::vector<Player> playersTest = { Player("Gigi"), Player("Marci"), Player("Luci") };

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
		else {
			if (passwordIter == end)
			{
				return crow::response(401, "UNAUTHORIZED");
			}
			else {
				return crow::response(404, "NOT FOUND");
			}
		}
		return crow::response(200);
			});
	//game logic related routes

	Game game;
	//initializing the game
	CROW_ROUTE(app, "/startgame")([&game, &playersTest] {
		if (!playersTest.empty()) {
			game = Game(playersTest); //to be initialized with waiting players list
			return crow::response(200); //OK
		}
		return crow::response(500); //internal server error
		});

	app.port(18080).multithreaded().run();

	return 0;
}