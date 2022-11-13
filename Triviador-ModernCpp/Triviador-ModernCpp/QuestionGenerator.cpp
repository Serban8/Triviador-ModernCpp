#include "QuestionGenerator.h"

QuestionGenerator::QuestionGenerator()
{
	GenerateToken();
}

std::string QuestionGenerator::GenerateToken()
{
	//construct the url and run get request
	const std::string tokenGeneratePath = "api_token.php?command=request";
	const std::string tokenRequestUrl = m_baseOnlineDatabaseUrl + tokenGeneratePath;

	cpr::Response r = cpr::Get(cpr::Url{ tokenRequestUrl });
	
	//parse the response
	json tokenResponse = json::parse(r.text);
	uint8_t responseCode = tokenResponse.at("response_code").get<uint8_t>();
	if (responseCode) {
		throw std::runtime_error(tokenResponse["response_message"]);
	}

	m_token = tokenResponse["token"];
	//std::cout << tokenResponse["token"];
	return std::string(tokenResponse["token"]);
}

std::vector<Question> QuestionGenerator::GetQuestionsFromOnlineDatabase(uint8_t numOfQuestions)
{
	//input validation
	if (numOfQuestions > 50) {
		throw std::runtime_error("Cannot generate more than 50 questions at a time");
	}
	std::vector<Question> generatedQuestions;

	//construct the url and get the json file with the questions
	const std::string questionGeneratePath = "api.php?amount=" + std::to_string(numOfQuestions) +  "&type=multiple&token=" + m_token;
	const std::string questionRequestUrl = m_baseOnlineDatabaseUrl + questionGeneratePath;
	cpr::Response r = cpr::Get(cpr::Url{ questionRequestUrl });

	json response = json::parse(r.text);
	uint8_t responseCode = response.at("response_code").get<uint8_t>();

	//response validation
	if (responseCode) {
		throw std::runtime_error("Questions request threw an error.");
	}

	//generate objects of type Question based on the result
	json questions = response["results"];

	for (auto& i : questions.items())
	{
		json question = i.value();
		json incorrectAnswers = question["incorrect_answers"];
		//todo: find if there is any method that automatically gets the array
		std::array<std::string, 3> incorrectAnswersArr = { incorrectAnswers[0], incorrectAnswers[1], incorrectAnswers[2] };

		generatedQuestions.push_back(
			Question(-1, 
				question["question"], 
				question["category"], 
				Question::Type::MultipleChoice, 
				question["correct_answer"], 
				incorrectAnswers));
	}

	return generatedQuestions;
}
