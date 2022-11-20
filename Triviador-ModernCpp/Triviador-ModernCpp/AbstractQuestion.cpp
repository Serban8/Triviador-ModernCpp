#include "AbstractQuestion.h"

std::string AbstractQuestion::GetQuestion() const
{
	return m_question;
}

std::string AbstractQuestion::GetCategory() const
{
	return m_category;
}
