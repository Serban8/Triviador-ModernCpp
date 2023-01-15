#include "Question.h"

std::string Question::GetQuestion() const
{
	return m_question;
}

std::string Question::GetCategory() const
{
	return m_category;
}

void Question::PrintQuestion() const
{
	std::cout << m_question << std::endl;
}

TRIVIADOR_API std::ostream& operator<<(std::ostream& out, const Question& question)
{
	out << question.m_question << std::endl;
	return out;
}
