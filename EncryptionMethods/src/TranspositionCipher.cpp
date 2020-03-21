#include "Headers\pch.h"

#include "TranspositionCipher.h"
#include "RectangularVectors.h"
#include "stringhelper.h"
#include "stringbuilder.h"
#include <list>
#include <string>
#include <vector>

namespace transposition_cipher
{

	static std::wstring pad_right(std::wstring basic_string, const int max, const char pad_char) {
		const int str_len = basic_string.length();

		if (str_len < max) {
			const auto pad_len = max - str_len;
			for (auto i = 0; i < pad_len; ++i) basic_string += pad_char;
		}

		return basic_string;
	}

	typedef struct {
		int key;
		wchar_t value;
	} key_value_pair;

	static bool compare_key_value_structure_keys(const key_value_pair& left_key_value_pair,
		const key_value_pair& right_key_value_pair)
	{
		return left_key_value_pair.value < right_key_value_pair.value;
	}

	std::vector<int> get_unique_key_indexes(std::wstring& literal_key)
	{
		const int key_length = literal_key.length();

		std::vector<int> unique_indexes(key_length);

		std::list<key_value_pair> sorted_key_value_pairs;

		for (auto index = 0; index < key_length; ++index)
			sorted_key_value_pairs.push_back({ index, literal_key[index] });

		sorted_key_value_pairs.sort(compare_key_value_structure_keys);

		auto buffer = 0;

		for (auto key_value_pair : sorted_key_value_pairs)
		{
			unique_indexes.at(key_value_pair.key) = buffer;
			++buffer;
		}

		return unique_indexes;
	}

	std::vector<std::vector<wchar_t>> swap2d_char_array_empty_char_to_special_symbol(std::vector<std::vector<wchar_t>>& chars, wchar_t& special_symbol)
	{
		for (auto index = 0; index < chars.size(); index++)
		{
			for (auto j_index = 0; j_index < (chars.size() == 0 ? 0 : chars[0].size()); j_index++)
			{
				if (chars[index][j_index] == L'\0' || static_cast<int>(chars[index][j_index]) == 32)
				{
					chars[index][j_index] = special_symbol;
				}
			}
		}

		return chars;

	}
	
	std::tuple<std::wstring, std::vector<std::vector<wchar_t>>> encipher(std::wstring& plain_text, std::wstring& literal_key, const char pad_char, wchar_t special_symbol)
	{
		const auto total_chars = plain_text.length();
		const int key_length = literal_key.length();

		plain_text = (plain_text.length() % literal_key.length() == 0)
			? plain_text
			: pad_right(plain_text, total_chars - (total_chars % key_length) + key_length, pad_char);

		auto stringBuilder = new string_builder();

		const auto total_columns = literal_key.length();
		const auto total_rows = static_cast<int>(std::ceil(static_cast<double>(total_chars) / total_columns));

		auto row_chars = rectangular_vectors::rectangular_wchar_t_vector(total_rows, total_columns);

		auto col_chars = rectangular_vectors::rectangular_wchar_t_vector(total_columns, total_rows);

		auto sorted_char_columns = rectangular_vectors::rectangular_wchar_t_vector(total_columns, total_rows);

		int current_row, current_column, row_index, column_index;

		auto unique_key_indexes = get_unique_key_indexes(plain_text);

		for (row_index = 0; row_index < total_chars; ++row_index)
		{
			current_row = row_index / total_columns;
			current_column = row_index % total_columns;
			row_chars[current_row][current_column] = plain_text[row_index];
		}

		for (row_index = 0; row_index < total_rows; ++row_index)
		{
			for (column_index = 0; column_index < total_columns; ++column_index)
			{
				col_chars[column_index][row_index] = row_chars[row_index][column_index];
			}
		}


		for (row_index = 0; row_index < total_columns; ++row_index)
		{
			for (column_index = 0; column_index < total_rows; ++column_index)
			{
				sorted_char_columns[unique_key_indexes[row_index]][column_index] = col_chars[row_index][column_index];
			}
		}

		for (row_index = 0; row_index < total_chars; ++row_index)
		{
			current_row = row_index / total_rows;
			current_column = row_index % total_rows;
			stringBuilder->append(sorted_char_columns[current_row][current_column]);
		}

		sorted_char_columns = transposition_cipher::swap2d_char_array_empty_char_to_special_symbol(sorted_char_columns, special_symbol);

		delete stringBuilder;
		return std::tuple<std::wstring, std::vector<std::vector<wchar_t>>>(stringBuilder->to_string(), sorted_char_columns);
	}

	std::wstring decipher(std::wstring& cipher_text, std::wstring& literal_key)
	{
		auto stringBuilder = new string_builder();

		const auto total_chars = cipher_text.length();
		const auto totalColumns = static_cast<int>(std::ceil(static_cast<double>(total_chars) / literal_key.length()));

		const auto totalRows = literal_key.length();

		auto row_chars = rectangular_vectors::rectangular_wchar_t_vector(totalRows, totalColumns);

		auto col_chars = rectangular_vectors::rectangular_wchar_t_vector(totalColumns, totalRows);

		auto unsorted_col_chars = rectangular_vectors::rectangular_wchar_t_vector(totalColumns, totalRows);
		int current_row, current_column, i, j;

		const auto unique_key_indexes = get_unique_key_indexes(literal_key);

		for (i = 0; i < total_chars; ++i)
		{
			current_row = i / totalColumns;
			current_column = i % totalColumns;
			row_chars[current_row][current_column] = cipher_text[i];
		}

		for (i = 0; i < totalRows; ++i)
		{
			for (j = 0; j < totalColumns; ++j)
			{
				col_chars[j][i] = row_chars[i][j];
			}
		}


		for (i = 0; i < totalColumns; ++i)
		{
			for (j = 0; j < totalRows; ++j)
			{
				unsorted_col_chars[i][j] = col_chars[i][unique_key_indexes[j]];
			}
		}

		for (i = 0; i < total_chars; ++i)
		{
			current_row = i / totalRows;
			current_column = i % totalRows;
			stringBuilder->append(unsorted_col_chars[current_row][current_column]);
		}

		delete stringBuilder;
		return stringBuilder->to_string();
	}
}
