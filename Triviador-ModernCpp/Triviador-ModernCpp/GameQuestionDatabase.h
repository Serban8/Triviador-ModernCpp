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
