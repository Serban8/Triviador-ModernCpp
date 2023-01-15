#include "MultipleChoiceQuestion.h"

std::string MultipleChoiceQuestion::GetCorrectAnswer() const
{
	return m_correctAnswer;
}

std::array<std::string, 3> MultipleChoiceQuestion::GetIncorrectAnswers() const
{
	return m_incorrectAnswers;
}
