#include "QuestionGenerator.h"

QuestionGenerator::QuestionGenerator()
{
	m_token = GenerateToken();
}

std::vector<MultipleChoiceQuestion> QuestionGenerator::GenerateMultipleChoiceQuestions(int numberOfQuestions)
{
	std::vector<MultipleChoiceQuestion> questions;
	int numOfQuestionsToInsert = numberOfQuestions;

	//generate the question in batches of 50 - since that is the max onlinetdb.com can return for one request
	while (questions.size() < numberOfQuestions) {
		if (numOfQuestionsToInsert >= 50) {
			std::vector<MultipleChoiceQuestion> tmpQ = GetMultipleChoiceQuestions();
			questions.insert(std::end(questions), std::begin(tmpQ), std::end(tmpQ));
			numOfQuestionsToInsert -= 50;
		}
		else {
			std::vector<MultipleChoiceQuestion> tmpQ = GetMultipleChoiceQuestions(numOfQuestionsToInsert);
			questions.insert(std::end(questions), std::begin(tmpQ), std::end(tmpQ));
			numOfQuestionsToInsert = 0;
		}

	}
	return questions;
}

std::vector<std::variant<NumberQuestion<int>, NumberQuestion<float>>> QuestionGenerator::GenerateNumberAnswerQuestions(int numberOfQuestions)
{
	//we can get a maximum of 30 questions from this online database.
	if (numberOfQuestions < 30) {
		return GetNumberAnswerQuestions(numberOfQuestions);
	}
	else {
		return GetNumberAnswerQuestions();
	}
}

std::string QuestionGenerator::GenerateToken()
{
	//construct the url and run get request
	const std::string tokenGeneratePath = "api_token.php?command=request";
	const std::string tokenRequestUrl = m_baseMultipleChoiceQuestionDatabaseUrl + tokenGeneratePath;

	cpr::Response r = cpr::Get(cpr::Url{ tokenRequestUrl });

	//parse the response
	json tokenResponse = json::parse(r.text);
	uint8_t responseCode = tokenResponse.at("response_code").get<uint8_t>();
	if (responseCode) {
		throw std::runtime_error(tokenResponse["response_message"]);
	}

	return std::string(tokenResponse["token"]);
}

std::vector<MultipleChoiceQuestion> QuestionGenerator::GetMultipleChoiceQuestions(uint8_t numOfQuestions)
{
	//input validation
	if (numOfQuestions > 50) {
		throw std::runtime_error("Cannot generate more than 50 questions at a time");
	}
	std::vector<MultipleChoiceQuestion> generatedQuestions;

	//construct the url and get the json file with the questions
	const std::string questionGeneratePath = "api.php?amount=" + std::to_string(numOfQuestions) + "&type=multiple&token=" + m_token;
	const std::string questionRequestUrl = m_baseMultipleChoiceQuestionDatabaseUrl + questionGeneratePath;
	cpr::Response r = cpr::Get(cpr::Url{ questionRequestUrl });

	json response = json::parse(r.text);
	uint8_t responseCode = response.at("response_code").get<uint8_t>();

	//response validation
	if (responseCode) {
		throw std::runtime_error("Questions request threw an error");
	}

	//generate objects of type MultipleChoiceQuestion based on the result
	json questions = response["results"];

	for (auto& i : questions.items())
	{
		json question = i.value();
		json incorrectAnswers = question["incorrect_answers"];
		//todo: find if there is any method that automatically gets the array
		std::array<std::string, 3> incorrectAnswersArr = { incorrectAnswers[0], incorrectAnswers[1], incorrectAnswers[2] };

		generatedQuestions.push_back(
			MultipleChoiceQuestion(
				question["question"],
				question["category"],
				question["correct_answer"],
				incorrectAnswers));
	}

	return generatedQuestions;
}

std::vector<std::variant<NumberQuestion<int>, NumberQuestion<float>>> QuestionGenerator::GetNumberAnswerQuestions(uint8_t numOfQuestions)
{
	if (numOfQuestions > 30) {
		throw std::runtime_error("Cannot generate more than 30 questions");
	}
	std::vector<std::variant<NumberQuestion<int>, NumberQuestion<float>>> generatedQuestions;

	//construct the url and get the json file with the questions
	const std::string questionGeneratePath = "api/questions?limit=" + std::to_string(numOfQuestions) + "&tags=numbers";
	const std::string questionRequestUrl = m_baseNumberAnswerQuestionDatabaseUrl + questionGeneratePath;
	cpr::Response r = cpr::Get(cpr::Url{ questionRequestUrl });

	json questions = json::parse(r.text);

	for (auto& it : questions.items()) {
		json question = it.value();

		std::string correctAns = question["correctAnswer"];
		std::array<std::string, 3> incorrectAnswers = { question["incorrectAnswers"][0], question["incorrectAnswers"][1], question["incorrectAnswers"][2] };
		//determine if answer is float, int or neither
		try {
			bool isValid = true;
			//try with int
			NumberQuestion<int> QInt = GenerateIntQuestion(question["question"], question["category"], correctAns, incorrectAnswers, isValid);
			if (!isValid) {
				//try with float if int didnt work
				NumberQuestion<float> QFloat = GenerateFloatQuestion(question["question"], question["category"], correctAns, incorrectAnswers, isValid);
				if (isValid) {
					generatedQuestions.push_back(QFloat);
				}
			}
			else {
				generatedQuestions.push_back(QInt);
			}
		}
		catch (std::invalid_argument const& ex) //catches stoi or stof exception - catches exception when string cannot be converted to int/float
		{
			continue;
		}
		catch (std::out_of_range const& ex) //catches stoi or stof exception
		{
			std::cout << "std::out_of_range::what(): " << ex.what() << '\n';
		}
	}

	return generatedQuestions;
}

NumberQuestion<int> QuestionGenerator::GenerateIntQuestion(std::string question, std::string category, std::string correctAnswer, std::array<std::string, 3> incorrectAnswers, bool& isValid)
{
	isValid = true;
	size_t pos;
	const int correctAnsInt = std::stoi(correctAnswer, &pos);
	if (pos != correctAnswer.size()) {
		isValid = false;
	}
	if (isValid) {
		std::array<int, 3> incorrectAnswersInt;
		uint8_t index = 0;
		for (const std::string& incorrectAns : incorrectAnswers) {
			const int incorrectAnsInt = std::stoi(incorrectAns, &pos);
			if (pos != incorrectAns.size()) {
				isValid = false;
				break;
			}
			else {
				incorrectAnswersInt[index++] = incorrectAnsInt;
			}
		}
		if (isValid) {
			return NumberQuestion<int>(
				question,
				category,
				correctAnsInt,
				incorrectAnswersInt
				);
		}
	}
	//if invalid question isValid is set to false and function returns empty question 
	return NumberQuestion<int>();
}


NumberQuestion<float> QuestionGenerator::GenerateFloatQuestion(std::string question, std::string category, std::string correctAnswer, std::array<std::string, 3> incorrectAnswers, bool& isValid)
{
	size_t pos;
	isValid = true;
	const float correctAnsFloat = std::stof(correctAnswer, &pos);
	if (pos != correctAnswer.size()) {
		isValid = false;
	}
	if (isValid) {
		std::array<float, 3> incorrectAnswersFloat;
		uint8_t index = 0;
		for (const auto& incorrectAns : incorrectAnswers) {
			const float incorrectAnsFloat = std::stof(incorrectAns, &pos);
			if (pos != incorrectAns.size()) {
				isValid = false;
				break;
			}
			else {
				incorrectAnswersFloat[index++] = incorrectAnsFloat;
			}
		}
		if (isValid) {
			return NumberQuestion<float>(
				question,
				category,
				correctAnsFloat,
				incorrectAnswersFloat
				);
		}
	}
	//if invalid question isValid is set to false and function returns empty question 
	return NumberQuestion<float>();
}
