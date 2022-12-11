#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

std::vector<std::string> split(const std::string& str, const std::string& delim);

std::unordered_map<std::string, std::string> parseRequestBody(const std::string& urlArgs);
