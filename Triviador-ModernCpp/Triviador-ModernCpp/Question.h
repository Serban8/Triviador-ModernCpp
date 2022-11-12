#pragma once
#include <string>
#include <array>

    class Question {
    public:
        enum class Type : uint8_t {
            MultipleChoice,
            Year,
            None

        };
    public:
        Question(int id, std::string question, std::string category, Type type, std::string correctAnswer, std::array<std::string, 3> incorrectAnswers) :
            m_id(id),
            m_question(question),
            m_category(category),
            m_type(type),
            m_correctAnswer(correctAnswer),
            m_incorrectAnswers(incorrectAnswers)
        {
            //empty
        }

        //getters
        std::string GetQuestion() const;
        std::string GetCategory() const;
        Type GetType() const;
        std::string GetCorrectAnswer() const;
        std::array<std::string, 3> GetIncorrectAnswers() const;

    private:
        int m_id;
        std::string m_question;
        std::string m_category;
        Type m_type;
        std::string m_correctAnswer;
        std::array<std::string, 3> m_incorrectAnswers;

    };
