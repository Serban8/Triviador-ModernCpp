#pragma once
#include"Game.h"
#include"NumberQuestion.h"
#include"MultipleChoiceQuestion.h"
#include"QuestionDatabase.h"
#include"GameDatabase.h"

//TO VERIFY: for tables made for N:M relations if we use Database class or game classes
struct GameQuestionDatabase
{
	GameQuestionDatabase() {}
	GameQuestionDatabase(GameDatabase game,QuestionDatabase q):
		m_questionId(q.m_id),
		m_gameId(game.m_id)
	{
		//empty
	}
	int m_gameId;
	int m_questionId;
};

namespace database {
	namespace gameQuestion {
		template <class T>
		void static insertGameQuestion(T& storage, GameDatabase game, QuestionDatabase question)
		{
			GameQuestionDatabase gameQuestionDB(game, question);
			storage.insert(gameQuestionDB);
		}
		template <class T>
		std::vector<GameQuestionDatabase> static getAllGameQuestions(T& storage)
		{
			auto allGameQuestions = storage.get_all<GameQuestionDatabase>();
			//for testing
			for (auto& p : allGameQuestions)
			{
				std::cout << storage.dump(p) << std::endl;
			}
			//
			return allGameQuestions;
		}
	}
}