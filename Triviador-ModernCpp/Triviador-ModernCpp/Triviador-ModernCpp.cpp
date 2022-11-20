#include <iostream>
//#include <sqlite_orm/sqlite_orm.h>
#include "MultipleChoiceQuestion.h"
#include "QuestionGenerator.h"
#include "QuestionDatabase.h"
#include "PlayerDatabase.h"
#include "NumberQuestion.h"
#include "Game.h"
#include "Player.h"
#include "Map.h"

//void databaseTest()
//{
//	//creating the database
//	using namespace sqlite_orm;
//	auto storage = make_storage("TRIV", make_table("MultipleChoiceQuestions",
//		make_column("id", &QuestionDatabase::m_id, autoincrement(), primary_key()),
//		make_column("question", &QuestionDatabase::m_question),
//		make_column("category", &QuestionDatabase::m_category),
//		make_column("type", &QuestionDatabase::m_type),
//		make_column("correctAnswer", &QuestionDatabase::m_correctAnswer),
//		make_column("incorrectAnswer1", &QuestionDatabase::m_incorrectAnswer1),
//		make_column("incorrectAnswer2", &QuestionDatabase::m_incorrectAnswer2),
//		make_column("incorrectAnswer3", &QuestionDatabase::m_incorrectAnswer3)),
//		make_table("Players",
//			make_column("id", &PlayerDatabase::m_id, autoincrement(), primary_key()),
//			make_column("username", &PlayerDatabase::m_username),
//			make_column("password", &PlayerDatabase::m_password)
//		));
//
//	storage.sync_schema();
//	storage.remove_all<QuestionDatabase>();
//	//inserting questions in the database
//	database::question::insertQuestions(storage);
//	//printing the contents
//	auto allQuestions = storage.get_all<QuestionDatabase>();
//	std::cout << "allQuestions (" << allQuestions.size() << "):" << std::endl;
//	for (auto& q : allQuestions)
//	{
//		std::cout << storage.dump(q) << std::endl;
//	}
//	//inserting players into database
//	database::player::insertPlayer(storage, Player("Marceel"));
//	database::player::insertPlayer(storage, Player("Mircea"));
//	auto allPlayers = storage.get_all<PlayerDatabase>();
//	for (auto& p : allPlayers)
//	{
//		std::cout << storage.dump(p) << std::endl;
//	}
//}

void playerTest() {
	Player p("marcel");
	//use debugger
	p.useAdvantage(Player::Advatage::FiftyFifty);
}

void gameTest() {

	
	QuestionGenerator qg;
	std::vector<MultipleChoiceQuestion> mq = qg.GenerateMultipleChoiceQuestions(10);
	
	std::vector<AbstractQuestion*> aq;

	for (auto& question : mq) {

		aq.push_back(static_cast<AbstractQuestion*>(&question));
	}

	/*for (auto& question : aq) {

		std::cout << question->GetQuestion()<<"\n";
	}*/
	Player p("marcel");
	std::vector<Player> player = { p,p };
	Game topG(player);
	topG.PlayGame();
}

void questionTest()
{
	std::array<std::string, 3> numericIncorrectAnswers = { "2000","2001","2002" };
	NumberQuestion<int> nq("What was the year Max Verstappen won his first WDC ? ", "Sports", "2021", numericIncorrectAnswers);
	std::cout << std::endl << nq.GetCorrectAnswer() << " " << nq.GetQuestion();
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
	//Question test
	//Question q(0, "How cool are dogs?", "General_culture", Question::Type::MultipleChoice, "Extremely cool", std::array<std::string, 3>{"Not cool", "Cool", "Pretty cool"});
	//std::cout << q.GetCategory() << std::endl <<
	//	q.GetCorrectAnswer() << std::endl <<
	//	q.GetIncorrectAnswers()[0] << std::endl <<
	//	q.GetIncorrectAnswers()[1] << std::endl <<
	//	q.GetIncorrectAnswers()[2] << std::endl <<
	//	q.GetQuestion() << std::endl <<
	//	static_cast<int>(q.GetType()) << std::endl;

	//QuestionGenerator GetMultipleChoiceQuestions method test
	//QuestionGenerator qGen;
	//std::vector<Question> vect = qGen.GetMultipleChoiceQuestions(); //make method GetMultipleChoiceQuestions public if you want to test

	//for (const Question& q : vect) {
	//	std::cout <<std::endl<< "NEXT QUESTIONNNN" << std::endl;
	//	std::cout << q.GetCategory() << std::endl <<
	//		q.GetCorrectAnswer() << std::endl <<
	//		q.GetIncorrectAnswers()[0] << std::endl <<
	//		q.GetIncorrectAnswers()[1] << std::endl <<
	//		q.GetIncorrectAnswers()[2] << std::endl <<
	//		q.GetQuestion() << std::endl <<
	//		static_cast<int>(q.GetType()) << std::endl;
	//}

	//QuestionGenerator GenerateMultipleChoiceQuestions method test
	/*QuestionGenerator qGen;
	std::vector<Question> vect = qGen.GenerateMultipleChoiceQuestions(123);

	std::cout << "Num of questions: " << vect.size() << std::endl;

	for (const Question& q : vect) {
		std::cout <<std::endl<< "NEXT QUESTIONNNN" << std::endl;
		std::cout << q.GetCategory() << std::endl <<
			q.GetCorrectAnswer() << std::endl <<
			q.GetIncorrectAnswers()[0] << std::endl <<
			q.GetIncorrectAnswers()[1] << std::endl <<
			q.GetIncorrectAnswers()[2] << std::endl <<
			q.GetQuestion() << std::endl <<
			static_cast<int>(q.GetType()) << std::endl;
	}*/

	//testing database adding of questions
	//databaseTest();

	//QuestionGenerator qg;
	//qg.GenerateNumberAnswerQuestions();
	//playerTest();
	gameTest();
	//questionTest();
	//mapTest();
	return 0;
}
