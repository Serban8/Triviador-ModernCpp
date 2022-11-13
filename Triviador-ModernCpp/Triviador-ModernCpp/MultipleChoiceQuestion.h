#pragma once
#include <string>
#include <array>
#include"AbstractQuestion.h"

class MultipleChoiceQuestion : public AbstractQuestion {

public:
    MultipleChoiceQuestion(int id, std::string question, std::string category, std::string correctAnswer, std::array<std::string, 3> incorrectAnswers) :
        AbstractQuestion(id, question, category, correctAnswer),
        m_incorrectAnswers(incorrectAnswers)
    {
        //empty
    }

    //getters
    std::array<std::string, 3> GetIncorrectAnswers() const;

private:

    std::array<std::string, 3> m_incorrectAnswers;

};
