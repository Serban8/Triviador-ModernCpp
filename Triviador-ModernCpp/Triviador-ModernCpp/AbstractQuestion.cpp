#include "AbstractQuestion.h"

std::string AbstractQuestion::GetQuestion() const
{
	return m_question;
}

std::string AbstractQuestion::GetCategory() const
{
	return m_category;
}

void AbstractQuestion::PrintQuestion() const
{
	std::cout << m_question << std::endl;
}

std::ostream& operator<<(std::ostream& out, const AbstractQuestion& question)
{
	out << question.m_question << std::endl;
	return out;
}
