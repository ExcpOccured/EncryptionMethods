#pragma once

#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include <cctype>
#include <algorithm>

class string_helper
{
public:
	static std::wstring to_lower(std::wstring source)
	{
		std::transform(source.begin(), source.end(), source.begin(), std::tolower);
		return source;
	}

	static std::wstring to_upper(std::wstring source)
	{
		std::transform(source.begin(), source.end(), source.begin(), std::toupper);
		return source;
	}

	static std::wstring trim_start(std::wstring source, const std::wstring &trim_chars = L" \t\n\r\v\f")
	{
		return source.erase(0, source.find_first_not_of(trim_chars));
	}

	static std::wstring trim_end(std::wstring source, const std::wstring &trim_chars = L" \t\n\r\v\f")
	{
		return source.erase(source.find_last_not_of(trim_chars) + 1);
	}

	static std::wstring trim(std::wstring source, const std::wstring &trimChars = L" \t\n\r\v\f")
	{
		return trim_start(trim_end(std::move(source), trimChars), trimChars);
	}

	static std::wstring replace(std::wstring source, const std::wstring &find, const std::wstring &replace)
	{
		std::size_t pos = 0;
		while ((pos = source.find(find, pos)) != std::string::npos)
		{
			source.replace(pos, find.length(), replace);
			pos += replace.length();
		}
		return source;
	}

	static bool startsWith(const std::wstring &source, const std::wstring &value)
	{
		if (source.length() < value.length())
			return false;
		else
			return source.compare(0, value.length(), value) == 0;
	}

	static bool endsWith(const std::wstring &source, const std::wstring &value)
	{
		if (source.length() < value.length())
			return false;
		else
			return source.compare(source.length() - value.length(), value.length(), value) == 0;
	}

	static std::vector<std::wstring> split(const std::wstring &source, const wchar_t delimiter)
	{
		std::vector<std::wstring> output;
		std::wistringstream ss(source);
		std::wstring next_item;

		while (std::getline(ss, next_item, delimiter))
		{
			output.push_back(next_item);
		}

		return output;
	}

	template<typename T>
	static std::wstring to_string(const T &subject)
	{
		std::wostringstream ss;
		ss << subject;
		return ss.str();
	}

	template<typename T>
	static T from_string(const std::wstring &subject)
	{
		std::wistringstream basic_mainstream(subject);
		T target;
		basic_mainstream >> target;
		return target;
	}

	static bool is_empty_or_white_space(const std::wstring &source)
	{
		if (source.length() == 0)
			return true;
		for (auto index : source)
		{
			if (!std::isspace(index))
				return false;
		}

		return true;
	}

	template<typename T>
	static std::wstring format_simple(const std::wstring &input, T arg0)
	{
		std::wostringstream basic_mainstreaming;
		auto last_close_brace = std::string::npos;
		auto open_brace = std::string::npos;
		
		while ((open_brace = input.find(L'{', open_brace + 1)) != std::string::npos)
		{
			if (open_brace + 1 < input.length())
			{
				if (input[open_brace + 1] == L'{')
				{
					open_brace++;
					continue;
				}

				const auto close_brace = input.find(L'}', open_brace + 1);
				if (close_brace != std::string::npos)
				{
					basic_mainstreaming << input.substr(last_close_brace + 1, open_brace - last_close_brace - 1);
					last_close_brace = close_brace;

					auto index = trim(input.substr(open_brace + 1, close_brace - open_brace - 1));
					if (index == L"0")
						basic_mainstreaming << arg0;
					else
						throw std::runtime_error("Only simple positional format specifiers are handled by the 'formatSimple' helper method.");
				}
			}
		}

		if (last_close_brace + 1 < input.length())
			basic_mainstreaming << input.substr(last_close_brace + 1);

		return basic_mainstreaming.str();
	}

	template<typename T1, typename T2>
	static std::wstring format_simple(const std::wstring &input, T1 arg0, T2 arg1)
	{
		std::wostringstream basic_mainstreaming;
		auto last_close_brace = std::string::npos;
		auto open_brace = std::string::npos;
		
		while ((open_brace = input.find(L'{', open_brace + 1)) != std::string::npos)
		{
			if (open_brace + 1 < input.length())
			{
				if (input[open_brace + 1] == L'{')
				{
					open_brace++;
					continue;
				}

				const auto close_brace = input.find(L'}', open_brace + 1);
				if (close_brace != std::string::npos)
				{
					basic_mainstreaming << input.substr(last_close_brace + 1, open_brace - last_close_brace - 1);
					last_close_brace = close_brace;

					auto index = trim(input.substr(open_brace + 1, close_brace - open_brace - 1));
					if (index == L"0")
						basic_mainstreaming << arg0;
					else if (index == L"1")
						basic_mainstreaming << arg1;
					else
						throw std::runtime_error("Only simple positional format specifiers are handled by the 'formatSimple' helper method.");
				}
			}
		}

		if (last_close_brace + 1 < input.length())
			basic_mainstreaming << input.substr(last_close_brace + 1);

		return basic_mainstreaming.str();
	}

	template<typename T1, typename T2, typename T3>
	static std::wstring format_simple(const std::wstring &input, T1 arg0, T2 arg1, T3 arg2)
	{
		std::wostringstream basic_mainstreaming;
		auto last_close_brace = std::string::npos;
		auto open_brace = std::string::npos;
		
		while ((open_brace = input.find(L'{', open_brace + 1)) != std::string::npos)
		{
			if (open_brace + 1 < input.length())
			{
				if (input[open_brace + 1] == L'{')
				{
					open_brace++;
					continue;
				}

				const auto close_brace = input.find(L'}', open_brace + 1);
				if (close_brace != std::string::npos)
				{
					basic_mainstreaming << input.substr(last_close_brace + 1, open_brace - last_close_brace - 1);
					last_close_brace = close_brace;

					auto index = trim(input.substr(open_brace + 1, close_brace - open_brace - 1));
					if (index == L"0")
						basic_mainstreaming << arg0;
					else if (index == L"1")
						basic_mainstreaming << arg1;
					else if (index == L"2")
						basic_mainstreaming << arg2;
					else
						throw std::runtime_error("Only simple positional format specifiers are handled by the 'formatSimple' helper method.");
				}
			}
		}

		if (last_close_brace + 1 < input.length())
			basic_mainstreaming << input.substr(last_close_brace + 1);

		return basic_mainstreaming.str();
	}
};
