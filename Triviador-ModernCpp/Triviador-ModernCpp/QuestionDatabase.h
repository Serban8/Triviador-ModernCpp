#pragma once
#include <iostream>
#include <random>
#include <unordered_set>

#include "Database.h"

#include "utils.h"

#include "MultipleChoiceQuestion.h"
#include "NumberQuestion.h"
#include "QuestionGenerator.h"

namespace database {
	//Generate 100 multiple choice questions and 100 number answear questions and insert them into the DB
	void generateAndInsertQuestions(Storage& storage)
	{
		QuestionGenerator qGen;
		std::vector<MultipleChoiceQuestion> multipleChoiceQuestions = qGen.GenerateMultipleChoiceQuestions();
		std::vector<std::variant<NumberQuestion<int>, NumberQuestion<float>>> numberQuestions = qGen.GenerateNumberAnswerQuestions();

		std::vector<QuestionDatabase> vectDB;
		//adding multiple choice questions
		for (const auto& q : multipleChoiceQuestions)
		{
			vectDB.push_back(QuestionDatabase(q));
		}
		//adding number questions
		for (const auto& q : numberQuestions)
		{
			vectDB.push_back(QuestionDatabase(q));
		}
		for (const auto& q : vectDB)
		{
			storage.insert(q);
		}
	}

	bool populateDatabaseWithQuestions(Storage& storage)
	{
		//value should be defined according to the requirements - something like 100 multiple choice & 30 number answer questions should be the minimum
		if (storage.count<QuestionDatabase>() < 15)
		{
			storage.remove_all<QuestionDatabase>();
			database::generateAndInsertQuestions(storage);
			storage.sync_schema();
			return true;
		}
		return false;
	}

	std::vector<MultipleChoiceQuestion> getMultipleChoiceQuestions(Storage& storage, int numberOfQuestions = 5)
	{
		namespace sql = sqlite_orm;
		static std::unordered_set<int> usedIndexes;
		//used to remember which questions were already asked

		//??????
		// We could implement a solution that does not require having all questions in memory:
		// First we insert n multiple choice questions, then we insert m number answer question. 
		// That means that multiple choice question have ids from 1 to n and
		// number answer questions have ids from n + 1 to n + m.
		// So if we know how many multiple choice questions there are, then we can find out the ids of number choice questions
		//??????

		// count for multiple choice questions
		//auto multipleChoiceCount = storage.count<QuestionDatabase>(sql::where(sql::c(&QuestionDatabase::m_type) = "MultipleChoice"));
		//

		auto allMultipleChoice = storage.get_all<QuestionDatabase>(sql::where(sql::c(&QuestionDatabase::m_type) = "MultipleChoice"));
		std::vector<MultipleChoiceQuestion> resultedQuestions;

		//get the desired number of questions
		for (int i = 0; i < numberOfQuestions; ++i) {
			//select a random index that has not been used before
			int randIndex;
			do {
				randIndex = getRandNum(0, allMultipleChoice.size() - 1);
			} while (usedIndexes.contains(randIndex));
			//get the question at pos randIndex
			auto selectedQuestion = allMultipleChoice[randIndex];
			//transform the question into an object of type MultipleChoiceQuestion and add it to the returned vector
			resultedQuestions.push_back(MultipleChoiceQuestion(
				selectedQuestion.m_question,
				selectedQuestion.m_category,
				selectedQuestion.m_correctAnswer,
				std::array<std::string, 3> {selectedQuestion.m_incorrectAnswer1, selectedQuestion.m_incorrectAnswer2, selectedQuestion.m_incorrectAnswer3}));
			//add the index we used to the set of used indexes
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

	std::vector<std::variant<NumberQuestion<int>, NumberQuestion<float>>> getNumberQuestions(Storage& storage, int numberOfQuestions = 5)
	{
		namespace sql = sqlite_orm;
		static std::unordered_set<int> usedIndexes;

		auto allNumberQ = storage.get_all<QuestionDatabase>(sql::where(sql::c(&QuestionDatabase::m_type) = "Number"));
		std::vector<std::variant<NumberQuestion<int>, NumberQuestion<float>>> resultedQuestions;

		for (int i = 0; i < numberOfQuestions; ++i) {
			int randIndex;
			do {
				randIndex = getRandNum(0, allNumberQ.size() - 1);
			} while (usedIndexes.contains(randIndex));
			auto selectedQuestion = allNumberQ[randIndex];

			QuestionGenerator qg;
			bool isValid;
			//assuming selected question is valid, since validation has been made when inserting the questions into the database
			//for future: encapsulate in try-catch block
			auto generatedQuestion = qg.GenerateQuestion(
				selectedQuestion.m_question,
				selectedQuestion.m_category,
				selectedQuestion.m_correctAnswer,
				std::array<std::string, 3> {selectedQuestion.m_incorrectAnswer1, selectedQuestion.m_incorrectAnswer2, selectedQuestion.m_incorrectAnswer3},
				isValid
			);
			if (isValid)
			{
				resultedQuestions.push_back(generatedQuestion);
			}
			else {
				//for future: if the question can't be converted it should be removed from database
				throw std::runtime_error("Invalid question stored in database");
				i--;
			}
			usedIndexes.insert(randIndex);
		}

		return resultedQuestions;
	}
}