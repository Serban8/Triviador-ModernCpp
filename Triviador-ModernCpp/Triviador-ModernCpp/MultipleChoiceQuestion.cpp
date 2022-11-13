#include "MultipleChoiceQuestion.h"

std::array<std::string, 3> MultipleChoiceQuestion::GetIncorrectAnswers() const
{
    return m_incorrectAnswers;
}
