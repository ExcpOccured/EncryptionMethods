#include "Headers\FeistelCipher.h"
#include "Domain/ExceptionFormatter.h"

#include <stdexcept>
#include <stdexcept>

using namespace encryption_methods::feistel_cipher;
using namespace domain;
using namespace std;


const std::wstring key_length_exception = L"Key length must be more then 8 symbols!";
const std::wstring incorrect_decipher_string_exception = L"Incorrect decipher string!";

std::vector<unsigned char> crypt_block(std::vector<unsigned char>& block_left_array, std::vector<unsigned char>& block_right_array,
	std::vector<unsigned char>& block_key_array, const bool is_last_element)
{
	const auto subblock_left_first_byte = block_left_array[0];
	const auto subblock_right_first_byte = block_right_array[0];
	const auto block_key_array_first_byte = block_key_array[0];

	auto subblock_left = static_cast<int>(subblock_left_first_byte);
	auto subblock_right = static_cast<int>(subblock_right_first_byte);
	auto subblock_key = static_cast<int>(block_key_array_first_byte);

	subblock_left ^= subblock_key;
	std::vector<unsigned char> temporary_allocated_buffer;

	if(temporary_allocated_buffer.size() < sizeof(subblock_left))
	{
		temporary_allocated_buffer.resize(sizeof(subblock_left));
	}

	std::memcpy(temporary_allocated_buffer.data(), &subblock_left, sizeof(subblock_left));
	
	block_left_array = temporary_allocated_buffer;

	auto bytes = std::vector<unsigned char>(2);
	std::copy_n(block_left_array.begin(), 2, bytes.begin());

	auto left_shifting = static_cast<int16_t>(bytes[0]);
	std::copy_n(block_left_array.begin() + 2, 2, bytes.begin());
	
	auto right_shifting = ToInt16(block_left_array, 2);

	subblock_right = change_cyclic_shift(left_shifting, right_shifting) ^ subblock_right;
	block_right_array = GetBytes(subblock_right);

	auto crypt_block = std::vector<unsigned char>(8);

	if (!is_last_element)
	{
		std::copy_n(block_right_array.begin(), 4, crypt_block.begin());
		std::copy_n(block_left_array.begin(), 4, crypt_block.begin() + 4);
	}
	else
	{
		std::copy_n(block_left_array.begin(), 4, crypt_block.begin());
		std::copy_n(block_right_array.begin(), 4, crypt_block.begin() + 4);
	}

	return crypt_block;
}

int change_cyclic_shift(short& left_shifting, short& right_shifting)
{
	auto leftPart = left_shifting << 7;
	auto rightPart = leftPart >> 16;
	left_shifting = static_cast<short>(leftPart + rightPart);

	leftPart = right_shifting >> 5;
	rightPart = leftPart << 11;
	right_shifting = static_cast<short>(leftPart + rightPart);

	const auto bitwise = left_shifting << 16;
	return bitwise + right_shifting;
}

std::vector<unsigned char> shift_key_bytes_to_left(std::vector<unsigned char>& keyByteArray, int& indexer)
{
	auto buffer = std::vector<unsigned char>(4);

	std::copy_n(keyByteArray.begin(), 4, buffer.begin());
	const auto left_shift = buffer[0];

	std::copy_n(keyByteArray.begin() + 4, 4, buffer.begin());
	const auto right_shift = buffer[0];

	const auto left_bitwise = left_shift << (indexer * 3);
	const auto right_bitwise = right_shift >> (32 - indexer * 3);

	auto bitwise = left_bitwise + right_bitwise;

	std::vector<unsigned char> bitwise_char_array;

    if(bitwise_char_array.size() < sizeof(bitwise))
    {
		bitwise_char_array.resize(sizeof(bitwise));
    }
	
	std::memcpy(bitwise_char_array.data(), &bitwise, sizeof(bitwise));

	return bitwise_char_array;
}

std::wstring encipher(std::wstring& plain_text, std::wstring& literal_key)
{
	if (literal_key.length() < 8)
	{
		throw std::runtime_error(Exception_formatter() << key_length_exception >> Exception_formatter::to_str);
	}

	std::vector<unsigned char> allocated_plain_text_bytes(plain_text.begin(), plain_text.end());
	allocated_plain_text_bytes.push_back('\0');

	std::vector<unsigned char> allocated_literal_key_bytes(literal_key.begin(), literal_key.end());
	allocated_literal_key_bytes.push_back('\0');

	const auto byte_array_ration_length = allocated_plain_text_bytes.size() % 8;

	if (byte_array_ration_length != 0)
	{
		auto buffer = std::vector<unsigned char>(allocated_plain_text_bytes.size() + (8 - byte_array_ration_length));
		std::copy_n(allocated_plain_text_bytes.begin(), allocated_plain_text_bytes.size(), buffer.begin());
		
		allocated_plain_text_bytes = buffer;
	}
	
	auto cipher_byte_array = std::vector<unsigned char>(allocated_plain_text_bytes.size());

	for (auto index = 0; index < allocated_plain_text_bytes.size(); index += 8)
	{
		auto crypt_block = std::vector<unsigned char>(8);
		std::copy_n(allocated_plain_text_bytes.begin() + index, 8, crypt_block.begin());

		for (auto j_index = 0; j_index <= 9; j_index++)
		{
			auto subblock_left_array = std::vector<unsigned char>(4);
			std::copy_n(crypt_block.begin(), 4, subblock_left_array.begin());
			
			auto subblock_right_array = std::vector<unsigned char>(4);
			std::copy_n(crypt_block.begin() + 4, 4, subblock_right_array.begin());

			auto subblockKeyArray = std::vector<unsigned char>(4);
			std::copy_n(allocated_literal_key_bytes.begin(), 4, subblockKeyArray.begin());
			
			subblockKeyArray =  shift_key_bytes_to_left(allocated_literal_key_bytes, j_index);

			crypt_block = j_index != 9 ? crypt_block(subblock_left_array, subblock_right_array, subblockKeyArray, false)
			: crypt_block(subblock_left_array, subblock_right_array, subblockKeyArray, true);
		}

		std::copy_n(crypt_block.begin(), crypt_block.size(), cipher_byte_array.begin() + index);
	}

	std::vector<unsigned char> ciphered_message(cipher_byte_array.begin(), cipher_byte_array.end());
	return ciphered_message;
}

std::wstring decipher(std::wstring& cipherText, std::wstring& literalKey)
{
	if (literalKey.length() < 8)
	{
		throw std::runtime_error(key_length_exception);
	}

	auto allocatedCipherTextBytes = Encoding::Unicode->GetBytes(cipherText);
	auto allocatedKeyBytes = Encoding::Unicode->GetBytes(literalKey);
	auto decipherByteArray = std::vector<unsigned char>(allocatedCipherTextBytes.size());

	auto byteArrayRationLength = allocatedCipherTextBytes.size() % 8;
	if (byteArrayRationLength != 0)
	{
		throw std::runtime_error(Exception_formatter() << incorrect_decipher_string_exception >> Exception_formatter::to_str);
	}

	for (auto index = allocatedCipherTextBytes.size() - 8; index >= 0; index -= 8)
	{
		auto decryptBlock = std::vector<unsigned char>(8);
		std::copy_n(allocatedCipherTextBytes.begin() + index, 8, decryptBlock.begin());


		for (auto jIndex = 9; jIndex >= 0; jIndex--)
		{
			auto subblock_left_array = std::vector<unsigned char>(4);
			std::copy_n(decryptBlock.begin(), 4, subblock_left_array.begin());
			auto subblock_right_array = std::vector<unsigned char>(4);
			std::copy_n(decryptBlock.begin() + 4, 4, subblock_right_array.begin());

			auto subblock_key_array = std::vector<unsigned char>(4);
			std::copy_n(allocatedKeyBytes.begin(), 4, subblock_key_array.begin());
			
			subblock_key_array = ShiftKeyBytesToLeft(allocatedKeyBytes, jIndex);

			decryptBlock = jIndex != 0 ? decrypt_block(subblock_left_array, subblock_right_array, subblock_key_array, false) 
			: decrypt_block(subblock_left_array, subblock_right_array, subblock_key_array, true);
		}

		std::copy_n(decryptBlock.begin(), decryptBlock.size(), decipherByteArray.begin() + index);
	}

	return Unicode->GetString(decipherByteArray);
}

std::vector<unsigned char> decrypt_block(std::vector<unsigned char>& blockLeftArray, std::vector<unsigned char>& blockRightArray, std::vector<unsigned char>& blockKeyArray, bool isLastElement)
{
	auto subblockLeft = ToInt32(blockLeftArray, 0);
	auto subblockRight = ToInt32(blockRightArray, 0);
	auto subblockKey = ToInt32(blockKeyArray, 0);

	auto bytes = std::vector<unsigned char>(2);

	std::copy_n(blockLeftArray.begin(), 2, bytes.begin());
	auto leftShift = ToInt16(bytes, 0);
	std::copy_n(blockLeftArray.begin() + 2, 2, bytes.begin());
	auto rightShift = ToInt16(blockLeftArray, 2);

	subblockRight = ChangeCyclicShift(leftShift, rightShift) ^ subblockRight;

	subblockLeft ^= subblockKey;
	blockLeftArray = GetBytes(subblockLeft);

	blockRightArray = GetBytes(subblockRight);

	auto decryptBlock = std::vector<unsigned char>(8);

	if (!isLastElement)
	{
		std::copy_n(blockRightArray.begin(), 4, decryptBlock.begin());
		std::copy_n(blockLeftArray.begin(), 4, decryptBlock.begin() + 4);
	}
	else
	{
		std::copy_n(blockLeftArray.begin(), 4, decryptBlock.begin());
		std::copy_n(blockRightArray.begin(), 4, decryptBlock.begin() + 4);
	}

	return decryptBlock;
}
