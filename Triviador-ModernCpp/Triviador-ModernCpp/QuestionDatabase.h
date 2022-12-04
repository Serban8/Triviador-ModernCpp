#pragma once
#include <iostream>
#include <random>
#include <unordered_set>

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
	QuestionDatabase(NumberQuestion<int> nq) :
		m_question(nq.GetQuestion()),
		m_category(nq.GetCategory()),
		m_type("Number")
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

int getRandNum(int min, int max) {
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<> distr(min, max);
	return distr(eng);
}

namespace database {
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
		storage.insert(QuestionDatabase(NumberQuestion<int>("Cati ani are Cosmin?", "General Culture", 20, std::array<int, 3> {1, 2, 3})));
		storage.insert(QuestionDatabase(NumberQuestion<int>("Cati ani are Gigel?", "General Culture", 10, std::array<int, 3> {1, 2, 3})));
	}
	template<class T>
	std::vector<MultipleChoiceQuestion>  getMultipleChoiceQuestions(T& storage, int numberOfQuestions = 5)
	{
		namespace sql = sqlite_orm;
		static std::unordered_set<int> usedIndexes;
		//??????
		// We could implement a solution that does not require having all questions in memory:
		// First we insert n multiple choice questions, then we insert m number answer question. 
		// That means that multiple choice question have ids from 1 to n and
		// number answer questions have ids from n + 1 to n + m.
		// So if we know how many multiple choice questions there are, then we can find out the ids of number choice questions
		//??????
		// 
		// count for multiple choice questions
		//auto multipleChoiceCount = storage.count<QuestionDatabase>(sql::where(sql::c(&QuestionDatabase::m_type) = "MultipleChoice"));
		//

		auto allMultipleChoice = storage.get_all<QuestionDatabase>(sql::where(sql::c(&QuestionDatabase::m_type) = "MultipleChoice"));
		std::vector<MultipleChoiceQuestion> resultedQuestions;
		
		for (int i = 0; i < numberOfQuestions; ++i) {
			int randIndex;
			do {
				randIndex = getRandNum(0, allMultipleChoice.size() - 1);
			} while (usedIndexes.contains(randIndex));
			auto selectedQuestion = allMultipleChoice[randIndex];
			resultedQuestions.push_back(MultipleChoiceQuestion(
				selectedQuestion.m_question, 
				selectedQuestion.m_category, 
				selectedQuestion.m_correctAnswer, 
				std::array<std::string, 3> {selectedQuestion.m_incorrectAnswer1, selectedQuestion.m_incorrectAnswer2, selectedQuestion.m_incorrectAnswer3}));
			usedIndexes.insert(randIndex);
		}

		//testing
		//std::copy(usedIndexes.begin(),
		//	usedIndexes.end(),
		//	std::ostream_iterator<int>(std::cout, " "));
		//std::cout << std::endl;
		//

		return resultedQuestions;
	}
}