#include <iostream>
#include "Question.h"
#include "QuestionGenerator.h"

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
	QuestionGenerator qGen;
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
	}

	return 0;
}
