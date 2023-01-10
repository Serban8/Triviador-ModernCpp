#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <cstdint>
#include <random>

//Maybe use unnamed namespace (https://stackoverflow.com/questions/357404/why-are-unnamed-namespaces-used-and-what-are-their-benefits)
//Basically this means that if another symbol with the same name is defined elsewhere there will not be a violation of the One Definition Rule (ODR) -> does not thow compilation error.

std::vector<std::string> split(const std::string& str, const std::string& delim);

std::unordered_map<std::string, std::string> parseRequestBody(const std::string& urlArgs);

int getRandNum(int min, int max);
