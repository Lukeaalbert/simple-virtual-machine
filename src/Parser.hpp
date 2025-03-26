#pragma once
#include <string>
#include <vector>
#include <tuple>

inline std::vector<std::string> Split(const std::string& str)
{
	const char DELIM = ',';
	std::vector<std::string> retVal;

	size_t start = 0;
	size_t delimLoc = str.find_first_of(DELIM, start);
	while (delimLoc != std::string::npos)
	{
		retVal.emplace_back(str.substr(start, delimLoc - start));

		start = delimLoc + 1;
		delimLoc = str.find_first_of(DELIM, start);
	}

	retVal.emplace_back(str.substr(start));
	return retVal;
}

template <typename T, typename... Args>
std::tuple<T, Args...> ParseStrHelper(std::vector<std::string>& params);

// ParseElem
template <typename T>
std::tuple<T> ParseElem(const std::string& param)
{}

// ParseElem int
template <>
inline std::tuple<int> ParseElem<int>(const std::string& param)
{
	return std::make_tuple(std::stoi(param));
}

// ParseElem string
template <>
inline std::tuple<std::string> ParseElem<std::string>(const std::string& param)
{
	return std::make_tuple(param);
}

// ParseStr Default
template <typename... Args>
std::tuple<Args...> ParseStr(std::vector<std::string>& params) 
{
	return ParseStrHelper<Args...>(params);
}

// ParseStr: no types passed (base case)
template <>
inline std::tuple<> ParseStr<>(std::vector<std::string>& params) 
{
	return std::make_tuple();
}

// ParseStrHelper
template <typename T, typename... Args>
std::tuple<T, Args...> ParseStrHelper(std::vector<std::string>& params)
{
	std::string elem = params.back();
	params.pop_back();

	// Recursion to form tuple to be returned
	return std::tuple_cat(ParseElem<T>(elem), ParseStr<Args...>(params));
}