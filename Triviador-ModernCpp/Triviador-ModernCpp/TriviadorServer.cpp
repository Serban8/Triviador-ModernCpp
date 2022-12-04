#include <iostream>
#include <sqlite_orm/sqlite_orm.h>
#include "MultipleChoiceQuestion.h"
#include "QuestionGenerator.h"
#include "QuestionDatabase.h"
#include "PlayerDatabase.h"
#include "NumberQuestion.h"
#include "Game.h"
#include "Player.h"
#include "Map.h"
#include "GameDatabase.h"
#include"GameQuestionDatabase.h"
#include"PlayerGameDatabase.h"

inline auto createStorage(const std::string& filename)
{
	using namespace sqlite_orm;
	return make_storage(
		filename,
		make_table("Players",
			make_column("username", &PlayerDatabase::m_username, primary_key()),//or player, need to see
			make_column("password", &PlayerDatabase::m_password)
			//instead of an column we will make a query to get no. of  games played
		),
		make_table("Questions",
			make_column("id", &QuestionDatabase::m_id, autoincrement(), primary_key()),
			make_column("question", &QuestionDatabase::m_question),
			make_column("category", &QuestionDatabase::m_category),
			make_column("type", &QuestionDatabase::m_type),
			make_column("correctAnswer", &QuestionDatabase::m_correctAnswer),
			make_column("incorrectAnswer1", &QuestionDatabase::m_incorrectAnswer1),
			make_column("incorrectAnswer2", &QuestionDatabase::m_incorrectAnswer2),
			make_column("incorrectAnswer3", &QuestionDatabase::m_incorrectAnswer3)
		),
		make_table("Game",
			make_column("id", &GameDatabase::m_id, autoincrement(), primary_key()),
			make_column("winner", &GameDatabase::m_winner),
			make_column("rounds", &GameDatabase::m_rounds),
			make_column("date", &GameDatabase::m_date),
			foreign_key(&GameDatabase::m_winner).references(&PlayerDatabase::m_username)
		),
		make_table("GameQuestion",
			make_column("gameId", &GameQuestionDatabase::m_gameId),
			make_column("questionId", &GameQuestionDatabase::m_questionId),
			primary_key(&GameQuestionDatabase::m_gameId, &GameQuestionDatabase::m_questionId),
			foreign_key(&GameQuestionDatabase::m_gameId).references(&GameDatabase::m_id),
			foreign_key(&GameQuestionDatabase::m_questionId).references(&QuestionDatabase::m_id)
		),
		make_table("PlayerGames",
			make_column("game", &PlayerGameDatabase::m_gameId),
			make_column("username", &PlayerGameDatabase::m_playerId),
			primary_key(&PlayerGameDatabase::m_gameId, &PlayerGameDatabase::m_playerId),
			foreign_key(&PlayerGameDatabase::m_gameId).references(&GameDatabase::m_id),
			foreign_key(&PlayerGameDatabase::m_playerId).references(&PlayerDatabase::m_username)
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
	std::cout<< p.m_username << std::endl;
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
		std::cout<< q.GetQuestion() << " " << q.GetCorrectAnswer() << std::endl;
	}
	storage.sync_schema();
}

void playerTest() {
	Player p("marcel");
	//use debugger
	p.useAdvantage(Player::Advatage::FiftyFifty);
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

	//testing database adding of questions
	//databaseTest();

	//QuestionGenerator qg;
	//qg.GenerateNumberAnswerQuestions();
	//playerTest();
	//gameTest();
	questionTest();
	//mapTest();
	return 0;
}