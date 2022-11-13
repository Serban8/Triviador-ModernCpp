#include <iostream>
#include <sqlite_orm/sqlite_orm.h>
#include "MultipleChoiceQuestion.h"
#include "QuestionGenerator.h"
#include "QuestionDatabase.h"
#include"YearQuestion.h"


void databaseTest()
{
	//creating the database
	using namespace sqlite_orm;
	auto storage = make_storage("TRIV", make_table("MultipleChoiceQuestions",
		make_column("id", &QuestionDatabase::m_id, autoincrement(), primary_key()),
		make_column("question", &QuestionDatabase::m_question),
		make_column("category", &QuestionDatabase::m_category),
		make_column("type", &QuestionDatabase::m_type),
		make_column("correctAnswer", &QuestionDatabase::m_correctAnswer),
		make_column("incorrectAnswer1", &QuestionDatabase::m_incorrectAnswer1),
		make_column("incorrectAnswer2", &QuestionDatabase::m_incorrectAnswer2),
		make_column("incorrectAnswer3", &QuestionDatabase::m_incorrectAnswer3)),
		make_table("YearQuestions",
			make_column("id", &QuestionDatabase::m_id, autoincrement(), primary_key()),
			make_column("question", &QuestionDatabase::m_question),
			make_column("category", &QuestionDatabase::m_category),
			make_column("type", &QuestionDatabase::m_type),
			make_column("correctAnswer", &QuestionDatabase::m_correctAnswer))
	);

	storage.sync_schema();
	storage.remove_all<QuestionDatabase>();
	//inserting questions in the database
	QuestionDatabase::insertQuestions(storage);
	//printing the contents
	auto allQuestions = storage.get_all<QuestionDatabase>();
	std::cout << "allQuestions (" << allQuestions.size() << "):" << std::endl;
	for (auto& q : allQuestions)
	{
		std::cout << storage.dump(q) << std::endl;
	}
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

	//QuestionGenerator GetQuestionsFromOnlineDatabase method test
	//QuestionGenerator qGen;
	//std::vector<Question> vect = qGen.GetQuestionsFromOnlineDatabase(); //make method GetQuestionsFromOnlineDatabase public if you want to test

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

	//QuestionGenerator GenerateQuestions method test
	/*QuestionGenerator qGen;
	std::vector<Question> vect = qGen.GenerateQuestions(123);

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
	databaseTest();
	return 0;
}
