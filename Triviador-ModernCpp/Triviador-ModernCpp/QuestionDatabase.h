#pragma once
#include <iostream>
#include "MultipleChoiceQuestion.h"
#include "NumberQuestion.h"
#include "QuestionGenerator.h"

struct QuestionDatabase
{

	QuestionDatabase() {}
	QuestionDatabase(MultipleChoiceQuestion q) :
		m_question(q.GetQuestion()),
		m_category(q.GetCategory()),
		m_type("MultipleChoice"),
		m_correctAnswer(q.GetCorrectAnswer()),
		m_incorrectAnswer1(q.GetIncorrectAnswers()[0]),
		m_incorrectAnswer2(q.GetIncorrectAnswers()[1]),
		m_incorrectAnswer3(q.GetIncorrectAnswers()[2])
	{

	}
	//used int for testing we will need to use std::variant
	QuestionDatabase(NumberQuestion<int> yq) :
		m_question(yq.GetQuestion()),
		m_category(yq.GetCategory()),
		m_type("Year")
	{
	}

	int m_id;
	std::string m_question;
	std::string m_category;
	std::string m_type;
	std::string m_correctAnswer;
	std::string m_incorrectAnswer1;
	std::string m_incorrectAnswer2;
	std::string m_incorrectAnswer3;

};

namespace database {
	namespace question {
		template<class T> 
		void static insertQuestions(T& storage)
		{
			QuestionGenerator qGen;
			std::vector<MultipleChoiceQuestion> multipleChoiceQuestions = qGen.GenerateMultipleChoiceQuestions();
			//std::vector<NumberQuestion> numberQuestions = qGen.GenerateNumberQuestions();

			std::vector<QuestionDatabase> vectDB;
			//adding multiple choice questions
			for (const auto& q : multipleChoiceQuestions)
			{
				vectDB.push_back(QuestionDatabase(q));
			}
			for (const auto& q : vectDB)
			{
				storage.insert(q);
			}
			//adding number questions
			/*for (const auto& q : numberQuestions)
			{
				vectDB.push_back(QuestionDatabase(q));
			}
			for (const auto& q : vectDB)
			{
				storage.insert(q);
			}*/
		}

		template <class T>
		std::vector<QuestionDatabase> static getAllQuestions(T& storage)
		{
			//printing the contents
			auto allQuestions = storage.get_all<QuestionDatabase>();
			//for testing
			std::cout << "allQuestions (" << allQuestions.size() << "):" << std::endl;
			for (auto& q : allQuestions)
			{
				std::cout << storage.dump(q) << std::endl;
			}
			//
			return allQuestions;
		}
		template<class T>
		QuestionDatabase static getQuestion(T& storage, int id)
		{
			//printing the contents
			auto question = storage.get<QuestionDatabase>(id);
			//for testing
			std::cout << storage.dump(question) << std::endl;
			//
			return question;
		}
	}
}