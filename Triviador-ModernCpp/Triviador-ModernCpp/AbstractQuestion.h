#pragma once
#include <string>
#include <array>

class AbstractQuestion {

public:

	//getters
	AbstractQuestion(int id, std::string question, std::string category) :
		m_id(id),
		m_question(question),
		m_category(category)
	{
		//empty
	}
	AbstractQuestion(int id, std::string question, std::string category, std::string correctAnswer) :
		m_id(id),
		m_question(question),
		m_category(category),
		m_correctAnswer(correctAnswer)
	{
		//empty
	}
	int GetId() const;
	std::string GetQuestion() const;
	std::string GetCategory() const;
	std::string GetCorrectAnswer() const;

protected:
	int m_id;
	std::string m_question;
	std::string m_category;
	std::string m_correctAnswer;

};
