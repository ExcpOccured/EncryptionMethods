#pragma once

#include <string>
#include <vector>
#include <tuple>
#include "stringbuilder.h"

namespace encryption_methods
{
	namespace transposition_cipher
	{
		class transposition_cipher
		{
		public:
			std::tuple<std::wstring, std::vector<std::vector<wchar_t>>> encipher(std::wstring& plain_text, std::wstring& literal_key, const char& padding_characters = L'\0', wchar_t special_symbol = L'*');

			std::wstring decipher(std::wstring& cipher_text, std::wstring& literal_key);
			
			std::vector<std::vector<wchar_t>> swap2d_char_array_empty_char_to_special_symbol(std::vector<std::vector<wchar_t>>& chars, wchar_t& special_symbol);
		};
	}
}
