#pragma once
#include <vector>

// Algebraic structure of a two-dimensional space
class rectangular_vectors
{
public:
	static std::vector<std::vector<wchar_t>> rectangular_wchar_t_vector(const int lower_bound, const int right_bound)
	{
		std::vector<std::vector<wchar_t>> new_vector(lower_bound);
		for (auto vector1 = 0; vector1 < lower_bound; vector1++)
		{
			new_vector[vector1] = std::vector<wchar_t>(right_bound);
		}

		return new_vector;
	}
};
