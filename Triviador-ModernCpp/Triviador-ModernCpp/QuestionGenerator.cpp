#include "QuestionGenerator.h"

QuestionGenerator::QuestionGenerator()
{
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

	//std::cout << tokenResponse["token"];
	return std::string(tokenResponse["token"]);
}
