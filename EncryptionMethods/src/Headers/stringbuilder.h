#pragma once

#include <string>
#include <sstream>

/// Implementation of string pool allocator 
class string_builder
{
private:
	std::wstring private_string_;

public:
	string_builder()
		= default;

	explicit string_builder(const std::wstring& initial_string)
	{
		private_string_ = initial_string;
	}

	string_builder(const std::size_t capacity)
	{
		ensure_capacity(capacity);
	}

	string_builder(const std::wstring& initial_string, const std::size_t capacity)
	{
		private_string_ = initial_string;
		ensure_capacity(capacity);
	}

	wchar_t operator [](const std::size_t index)
	{
		return private_string_[index];
	}

	string_builder* append(const std::wstring& to_append)
	{
		private_string_ += to_append;

		return this;
	}

	template<typename T>
	string_builder* append(const T& to_append)
	{
		private_string_ += to_string(to_append);
		return this;
	}

	string_builder* append_line()
	{
		private_string_ += L"\r\n";
		return this;
	}

	string_builder* append_line(const std::wstring& to_append)
	{
		private_string_ += to_append + L"\r\n";
		return this;
	}

	string_builder* insert(std::size_t position, const std::wstring& toInsert)
	{
		private_string_.insert(position, toInsert);
		return this;
	}

	template<typename T>
	string_builder* insert(std::size_t position, const T& to_insert)
	{
		private_string_.insert(position, to_string(to_insert));
		return this;
	}

	std::wstring to_string() const
	{
		return private_string_;
	}

	std::wstring to_string(const std::size_t start, const std::size_t length) const
	{
		return private_string_.substr(start, length);
	}

	std::size_t length() const
	{
		return private_string_.length();
	}

	void set_length(const std::size_t new_length)
	{
		private_string_.resize(new_length);
	}

	std::size_t capacity() const
	{
		return private_string_.capacity();
	}

	void ensure_capacity(const std::size_t minimum_capacity)
	{
		private_string_.reserve(minimum_capacity);
	}

	std::size_t max_capacity() const
	{
		return private_string_.max_size();
	}

	void clear()
	{
		private_string_.clear();
	}

	string_builder* remove(const std::size_t start, const std::size_t length)
	{
		private_string_.erase(start, length);

		return this;
	}

	string_builder* replace(const std::wstring& old_string, const std::wstring& new_string)
	{
		std::size_t pos = 0;
		while ((pos = private_string_.find(old_string, pos)) != std::wstring::npos)
		{
			private_string_.replace(pos, old_string.length(), new_string);
			pos += new_string.length();
		}
		return this;
	}

private:
	template<typename T>
	static std::wstring to_string(const T& subject)
	{
		std::wostringstream mainstreaming;
		mainstreaming << subject;
		return mainstreaming.str();
	}
};
