#pragma once
#include<iostream>
#include "MultipleChoiceQuestion.h"
#include"YearQuestion.h"
struct QuestionDatabase
{
	QuestionDatabase() {}
	QuestionDatabase(MultipleChoiceQuestion q) :
		m_id(q.GetId()),
		m_question(q.GetQuestion()),
		m_category(q.GetCategory()),
		m_type("MultipleChoice"),
		m_correctAnswer(q.GetCorrectAnswer()),
		m_incorrectAnswer1(q.GetIncorrectAnswers()[0]),
		m_incorrectAnswer2(q.GetIncorrectAnswers()[1]),
		m_incorrectAnswer3(q.GetIncorrectAnswers()[2])
	{

	}
	QuestionDatabase(YearQuestion yq) :
		m_id(yq.GetId()),
		m_question(yq.GetQuestion()),
		m_category(yq.GetCategory()),
		m_type("Year"),
		m_correctAnswer(yq.GetCorrectAnswer())
	{ }
	int m_id;
	std::string m_question;
	std::string m_category;
	std::string m_type;
	std::string m_correctAnswer;
	std::string m_incorrectAnswer1;
	std::string m_incorrectAnswer2;
	std::string m_incorrectAnswer3;

};

