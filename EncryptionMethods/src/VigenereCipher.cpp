#include "Headers\pch.h"

#include <list>
#include <string>
#include <vector>

#include "Headers/RectangularVectors.h"
#include "Headers/TranspositionCipher.h"
#include "Headers/stringbuilder.h"

namespace vigenere_cipher
{
	int vigenere_cipher::MultiplicationWitByteShift(int& leftOperand, int& rightOperand)
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

	int vigenere_cipher::RemainderOperator(int& left_operand, int& right_operand)
	{
		auto divisor_left_operand = left_operand / right_operand;

		return (left_operand - MultiplicationWitByteShift(right_operand, divisor_left_operand));
	}
	
	int vigenere_cipher::get_derivative_mod(int& leftOperand, int& right_operand)
	{
		const auto mod_right_operand = RemainderOperator(leftOperand, right_operand);

		auto adderValue = mod_right_operand + right_operand;

		return RemainderOperator(adderValue, right_operand);
	}
}