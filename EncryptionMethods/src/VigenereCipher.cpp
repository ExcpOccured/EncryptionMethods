#include "Headers\pch.h"

#include "Headers/VigenereCipher.h"

using namespace encryption_methods::vigenere_cipher;

int multiplication_wit_byte_shift(int& leftOperand, int& rightOperand)
{
	auto operationResult = 0;
	auto execution_count = 0;

	while (rightOperand > 0)
	{
		if (rightOperand % 2 == 1)
		{
			operationResult += leftOperand << execution_count;
		}

		execution_count++;
		rightOperand /= 2;
	}

	return operationResult;
}

int remainder_operator(int& left_operand, int& right_operand)
{
	auto divisor_left_operand = left_operand / right_operand;

	return (left_operand - multiplication_wit_byte_shift(right_operand, divisor_left_operand));
}

int get_derivative_mod(int& leftOperand, int& right_operand)
{
	const auto mod_right_operand = remainder_operator(leftOperand, right_operand);

	auto adderValue = mod_right_operand + right_operand;

	return remainder_operator(adderValue, right_operand);
}