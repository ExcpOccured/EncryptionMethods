#pragma once

#include <string>
#include <vector>


namespace encryption_methods
{
	namespace feistel_cipher
	{
		class feistel_cipher_type
		{
		private:
			static const std::wstring key_length_exception_message;

			static const std::wstring incorrect_decipher_string_exception_message;

		public:
			std::wstring encipher(std::wstring& plain_text, std::wstring& literal_key);

			std::wstring decipher(std::wstring& cipher_text, std::wstring& literal_key);

		private:
			std::vector<unsigned char> crypt_block(std::vector<unsigned char>& block_left_array, std::vector<unsigned char>& block_right_array, std::vector<unsigned char>& block_key_array, bool is_last_element);

			std::vector<unsigned char> decrypt_block(std::vector<unsigned char>& block_left_array, std::vector<unsigned char>& block_right_array, std::vector<unsigned char>& block_key_array, bool is_last_element);

			int change_cyclic_shift(short& left_shifting, short& right_shifting);

			std::vector<unsigned char> shift_key_bytes_to_left(std::vector<unsigned char>& key_byte_array, int& indexer);
		};
	}
}
