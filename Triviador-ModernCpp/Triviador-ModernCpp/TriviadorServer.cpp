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
	storage.remove_all<GameDatabase>();
	//storage.remove_all<QuestionDatabase>();

	//inserting questions in the database
	/*database::generateAndInsertQuestions(storage);
	auto questions = storage.get_all<QuestionDatabase>();
	for (auto& q : questions)
	{
		std::cout << q.m_id << " " << q.m_question << std::endl;
	}*/

	//inserting players into database
	//storage.replace(PlayerDatabase(Player("Flo")));
	//storage.replace(PlayerDatabase(Player("Dany")));
	//storage.replace(PlayerDatabase(Player("Jimmy")));
	//auto players = storage.get_all<PlayerDatabase>();
	//for (auto& p : players)
	//{
	//	std::cout << p.m_username << std::endl;
	//}
	//auto p = storage.get<PlayerDatabase>("Flo");
	//std::cout << p.m_username << std::endl;

	//insertin games into database
	//storage.insert(GameDatabase("Dany", 5));
	//storage.insert(GameDatabase("Jimmy", 6));
	GameDatabase g = GameDatabase("p1", 5);
	auto insertedId = storage.insert(g);
	std::string pl = "p1";
	auto player = storage.get_pointer<PlayerDatabase>("p1");
	std::cout << g.m_id << " " << player.get()->m_username << std::endl;
	PlayerGameDatabase pgd = PlayerGameDatabase(insertedId, player.get()->m_username);
	std::cout << *pgd.m_gameId << " " << *pgd.m_playerId<< std::endl;
	storage.replace(pgd);
	
	/*auto g1 = storage.get<GameDatabase>(1);
	auto g2 = storage.get<GameDatabase>(2);
	auto g3 = storage.get<GameDatabase>(3);
	std::cout << g1.m_id << " " << g1.m_winner << std::endl;
	std::cout << g2.m_id << " " << g2.m_winner << std::endl;
	std::cout << g3.m_id << " " << g3.m_winner << std::endl;*/

	/*std::vector<MultipleChoiceQuestion> resultedQ;
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
	}*/
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
	//databaseTest();
	//TO TEST ADD PLAYER
	AppManager appManager(app, storage);

	//make player order
	////route that gives the player all available territories to attack

	app.port(18080).multithreaded().run();

	return 0;
}