#pragma once
#include "AbstractQuestion.h"
class YearQuestion : public AbstractQuestion
{
public:

	YearQuestion(int id, std::string question, std::string category, std::string correctAnswer) :
		AbstractQuestion(id, question, category, correctAnswer) {}
};