#pragma once
#include <string>
#include <array>
#include <iostream>

class AbstractQuestion {

public:
	AbstractQuestion() {}
	AbstractQuestion(std::string question, std::string category) :
		m_question(question),
		m_category(category)
	{
		//empty
	}

	//getters
	virtual std::string GetQuestion() const;
	virtual std::string GetCategory() const;
	//operator afisare
	friend  std::ostream& operator<<(std::ostream& out, const AbstractQuestion& question);
	virtual void PrintQuestion() const;

protected:
	std::string m_question;
	std::string m_category;
};
