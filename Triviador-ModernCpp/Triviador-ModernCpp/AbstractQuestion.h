#pragma once
#include <string>
#include <array>

class AbstractQuestion {

public:
	AbstractQuestion(std::string question, std::string category) :
		m_question(question),
		m_category(category)
	{
		//empty
	}

	//getters
	std::string GetQuestion() const;
	std::string GetCategory() const;

protected:
	std::string m_question;
	std::string m_category;
};
