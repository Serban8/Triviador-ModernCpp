#pragma once
#include "AbstractQuestion.h"
#include<iostream>
#include<sstream>
template<class N>
class NumberQuestion : public AbstractQuestion
{
public:

	NumberQuestion() {}

	NumberQuestion(std::string question, std::string category, std::string correctAnswer, std::array<std::string, 3> incorrectAnswers) :
		AbstractQuestion(question, category)
	{
		std::stringstream correctAns(correctAnswer);
		std::stringstream incorrectAns1(incorrectAnswers[0]);
		std::stringstream incorrectAns2(incorrectAnswers[1]);
		std::stringstream incorrectAns3(incorrectAnswers[2]);
		correctAns >> m_correctAnswer;
		incorrectAns1 >> m_incorrectAnswers[0];
		incorrectAns2 >> m_incorrectAnswers[1];
		incorrectAns3 >> m_incorrectAnswers[2];
	}

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