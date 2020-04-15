#include "Headers\FeistelCipher.h"
#include "Domain/ExceptionFormatter.h"

#include <stdexcept>
#include <stdexcept>

using namespace encryption_methods::feistel_cipher;
using namespace domain;
using namespace std;


const std::wstring key_length_exception = L"Key length must be more then 8 symbols!";
const std::wstring incorrect_decipher_string_exception = L"Incorrect decipher string!";

std::vector<unsigned char> allocate_int_byte_vector(int& input_value)
{
	size_t input_value_size = input_value;

	std::vector<unsigned char> allocated_vector;
	allocated_vector.clear();

	if (input_value == 0)
	{
		allocated_vector.push_back(0);
	}

	while (input_value_size > 0)
	{
		auto current_part = static_cast<unsigned char>(input_value % 256);
		allocated_vector.push_back(current_part);
		input_value_size /= 256;
	}

	return allocated_vector;
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
	const auto left_bitwise = static_cast<int>(buffer[0]);

	std::copy_n(keyByteArray.begin() + 4, 4, buffer.begin());
	const auto right_bitwise = static_cast<int>(buffer[0]);

	const auto leftBitwise = left_bitwise << (indexer * 3);
	const auto rightBitwise = right_bitwise >> (32 - indexer * 3);

	auto bitwise = leftBitwise + rightBitwise;

	std::vector<unsigned char> temporary_allocated_buffer;

	if (temporary_allocated_buffer.size() < sizeof(bitwise))
	{
		temporary_allocated_buffer.resize(sizeof(bitwise));
	}

	std::memcpy(temporary_allocated_buffer.data(), &bitwise, sizeof(bitwise));

	return temporary_allocated_buffer;
}


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

	if (temporary_allocated_buffer.size() < sizeof(subblock_left))
	{
		temporary_allocated_buffer.resize(sizeof(subblock_left));
	}

	std::memcpy(temporary_allocated_buffer.data(), &subblock_left, sizeof(subblock_left));

	block_left_array = temporary_allocated_buffer;

	auto bytes = std::vector<unsigned char>(2);
	std::copy_n(block_left_array.begin(), 2, bytes.begin());

	auto left_shifting = static_cast<int16_t>(bytes[0]);
	std::copy_n(block_left_array.begin() + 2, 2, bytes.begin());


	auto right_shifting = static_cast<int16_t>(block_left_array[2]);

	subblock_right = change_cyclic_shift(left_shifting, right_shifting) ^ subblock_right;

	block_right_array = allocate_int_byte_vector(subblock_right);

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
		auto crypted_block = std::vector<unsigned char>(8);
		std::copy_n(allocated_plain_text_bytes.begin() + index, 8, crypted_block.begin());

		for (auto j_index = 0; j_index <= 9; j_index++)
		{
			auto subblock_left_array = std::vector<unsigned char>(4);
			std::copy_n(crypted_block.begin(), 4, subblock_left_array.begin());

			auto subblock_right_array = std::vector<unsigned char>(4);
			std::copy_n(crypted_block.begin() + 4, 4, subblock_right_array.begin());

			auto subblockKeyArray = std::vector<unsigned char>(4);
			std::copy_n(allocated_literal_key_bytes.begin(), 4, subblockKeyArray.begin());

			subblockKeyArray = shift_key_bytes_to_left(allocated_literal_key_bytes, j_index);

			crypted_block = j_index != 9 ? crypt_block(subblock_left_array, subblock_right_array, subblockKeyArray, false)
				: crypt_block(subblock_left_array, subblock_right_array, subblockKeyArray, true);
		}

		std::copy_n(crypted_block.begin(), crypted_block.size(), cipher_byte_array.begin() + index);
	}

	std::vector<unsigned char> ciphered_message(cipher_byte_array.begin(), cipher_byte_array.end());

	std::wstringstream  buffer_result;

	for (auto indexer : ciphered_message)
	{
		buffer_result << indexer;
	}

	return buffer_result.str();
}


std::vector<unsigned char> decrypt_block(std::vector<unsigned char>& blockLeftArray, std::vector<unsigned char>& blockRightArray,
	std::vector<unsigned char>& blockKeyArray, bool isLastElement)
{
	auto subblockLeft = static_cast<int32_t>(blockLeftArray[0]);
	auto subblockRight = static_cast<int32_t>(blockRightArray[0]);
	auto subblockKey = static_cast<int32_t>(blockKeyArray[0]);

	auto bytes = std::vector<unsigned char>(2);

	std::copy_n(blockLeftArray.begin(), 2, bytes.begin());
	auto leftShift = static_cast<int16_t>(bytes[0]);
	std::copy_n(blockLeftArray.begin() + 2, 2, bytes.begin());

	auto rightShift = static_cast<int16_t>(blockLeftArray[2]);

	subblockRight = change_cyclic_shift(leftShift, rightShift) ^ subblockRight;

	subblockLeft ^= subblockKey;

	blockLeftArray = allocate_int_byte_vector(subblockLeft);

	blockRightArray = allocate_int_byte_vector(subblockRight);

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


std::wstring decipher(std::wstring& cipher_text, std::wstring& literal_key)
{
	if (literal_key.length() < 8)
	{
		throw std::runtime_error(Exception_formatter() << key_length_exception >> Exception_formatter::to_str);
	}

	std::vector<unsigned char> allocated_cipher_text_bytes(cipher_text.begin(), cipher_text.end());
	allocated_cipher_text_bytes.push_back('\0');

	std::vector<unsigned char> allocated_key_bytes(literal_key.begin(), literal_key.end());
	allocated_key_bytes.push_back('\0');

	auto decipherByteArray = std::vector<unsigned char>(allocated_cipher_text_bytes.size());

	auto byteArrayRationLength = allocated_cipher_text_bytes.size() % 8;

	if (byteArrayRationLength != 0)
	{
		throw std::runtime_error(Exception_formatter() << incorrect_decipher_string_exception >> Exception_formatter::to_str);
	}

	for (auto index = allocated_cipher_text_bytes.size() - 8; index >= 0; index -= 8)
	{
		auto decryptBlock = std::vector<unsigned char>(8);
		std::copy_n(allocated_cipher_text_bytes.begin() + index, 8, decryptBlock.begin());


		for (auto jIndex = 9; jIndex >= 0; jIndex--)
		{
			auto subblock_left_array = std::vector<unsigned char>(4);
			std::copy_n(decryptBlock.begin(), 4, subblock_left_array.begin());
			auto subblock_right_array = std::vector<unsigned char>(4);
			std::copy_n(decryptBlock.begin() + 4, 4, subblock_right_array.begin());

			auto subblock_key_array = std::vector<unsigned char>(4);
			std::copy_n(allocated_key_bytes.begin(), 4, subblock_key_array.begin());

			subblock_key_array = shift_key_bytes_to_left(allocated_key_bytes, jIndex);

			decryptBlock = jIndex != 0 ? decrypt_block(subblock_left_array, subblock_right_array, subblock_key_array, false)
				: decrypt_block(subblock_left_array, subblock_right_array, subblock_key_array, true);
		}

		std::copy_n(decryptBlock.begin(), decryptBlock.size(), decipherByteArray.begin() + index);
	}

	std::wstringstream  buffer_result;

	for (auto indexer : decipherByteArray)
	{
		buffer_result << indexer;
	}

	return buffer_result.str();
}

