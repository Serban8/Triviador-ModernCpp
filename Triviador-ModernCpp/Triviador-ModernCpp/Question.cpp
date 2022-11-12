#include "Question.h"

std::string Question::GetQuestion() const
{
    return m_question;
}

std::string Question::GetCategory() const
{
    return m_category;
}

Question::Type Question::GetType() const
{
    return m_type;
}

std::string Question::GetCorrectAnswer() const
{
    return m_correctAnswer;
}

std::array<std::string, 3> Question::GetIncorrectAnswers() const
{
    return m_incorrectAnswers;
}
