#pragma once
#include <iostream>
#include <string>

#include "Question.h"

template<class N>
class TRIVIADOR_API NumberQuestion : public Question
{
public:
	NumberQuestion() = default;
	NumberQuestion(std::string question, std::string category, N correctAnswer, std::array<N, 3> incorrectAnswers) :
		Question(question, category), m_correctAnswer(correctAnswer), m_incorrectAnswers(incorrectAnswers)
	{}

	//getters
	std::array<N, 3> GetIncorrectAnswers() const;
	N GetCorrectAnswer() const;

private:
	N m_correctAnswer;
	std::array<N, 3> m_incorrectAnswers;
};

template<typename N>
N NumberQuestion<N>::GetCorrectAnswer() const
{
	return m_correctAnswer;
}

template<typename N>
std::array<N, 3> NumberQuestion<N>::GetIncorrectAnswers() const
{
	return m_incorrectAnswers;
}