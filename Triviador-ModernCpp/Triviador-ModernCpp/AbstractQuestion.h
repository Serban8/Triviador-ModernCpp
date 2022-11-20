#pragma once
#include <string>
#include <array>
#include <iostream>

class AbstractQuestion {

public:
	AbstractQuestion(std::string question, std::string category) :
		m_question(question),
		m_category(category)
	{
		//empty
	}

	//getters
	virtual std::string GetQuestion() const;
	virtual std::string GetCategory() const;

	virtual void PrintQuestion() const;

protected:
	std::string m_question;
	std::string m_category;
};
