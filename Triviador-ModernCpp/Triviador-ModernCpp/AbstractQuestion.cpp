#include "AbstractQuestion.h"


int AbstractQuestion::GetId() const
{
	return m_id;
}

std::string AbstractQuestion::GetQuestion() const
{
	return m_question;
}

std::string AbstractQuestion::GetCategory() const
{
	return m_category;
}

std::string AbstractQuestion::GetCorrectAnswer() const
{
	return m_correctAnswer;
}
