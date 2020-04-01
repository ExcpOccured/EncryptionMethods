#pragma once

#include <string>

namespace encryption_methods
{
	namespace vigenere_cipher
	{
		class vigenere_cipher
		{
		public:
			std::wstring cipher(std::wstring& input_text, std::wstring& literal_key, bool is_encipher);
			 
			int get_derivative_mod(int& left_operand, int& right_operand);

			int remainder_operator(int& left_operand, int& right_operand);

			int multiplication_wit_byte_shift(int& left_operand, int& right_operand);
		};
	}
}

