#pragma once
#include "AbstractQuestion.h"
class NumberQuestion : public AbstractQuestion
{
public:

	NumberQuestion(int id, std::string question, std::string category, std::string correctAnswer) :
		AbstractQuestion(id, question, category, correctAnswer) {}
};
