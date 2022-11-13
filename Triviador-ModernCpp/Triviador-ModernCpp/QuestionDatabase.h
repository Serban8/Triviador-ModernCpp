#pragma once
#include<iostream>
#include "MultipleChoiceQuestion.h"
#include"YearQuestion.h"
#include "QuestionGenerator.h"

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
	template<class T> void static insertQuestions(T& storage)
	{
		QuestionGenerator qGen;
		std::vector<MultipleChoiceQuestion> multipleChoiceQuestions = qGen.GenerateQuestions();
		std::vector<YearQuestion> yearQuestions;
		//manually added questions for testing
		YearQuestion yq(-1, "What year is it today?", "news", "2022");
		yearQuestions.push_back(yq);
		YearQuestion yq1(-1, "What was the year Max Verstappen won his first WDC?", "Sports", "2021");
		yearQuestions.push_back(yq1);
		YearQuestion yq2(-1, "When did Michael Schumacher win is last WDC?", "Sports", "2004");
		yearQuestions.push_back(yq2);
		//

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
		//adding year questions
		for (const auto& q : yearQuestions)
		{
			vectDB.push_back(QuestionDatabase(q));
		}
		for (const auto& q : vectDB)
		{
			storage.insert(q);
		}
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

