#pragma once
#include <string>
#include <array>

#include "AbstractQuestion.h"

//needed in client
class MultipleChoiceQuestion : public AbstractQuestion
{
public:
	MultipleChoiceQuestion(std::string question, std::string category, std::string correctAnswer, std::array<std::string, 3> incorrectAnswers) :
		AbstractQuestion(question, category),
		m_correctAnswer(correctAnswer),
		m_incorrectAnswers(incorrectAnswers)
	{
	}

	//getters
	std::string GetCorrectAnswer() const;
	std::array<std::string, 3> GetIncorrectAnswers() const;

private:
	std::string m_correctAnswer;
	std::array<std::string, 3> m_incorrectAnswers;

};
