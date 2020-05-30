#include "MorphPicker.hpp"

std::array<int, 4> pickMorphs(const std::vector<float>& weights)
{
	std::array<int, 4> selected{};
	selected.fill(-1);

	//if less or equal 4 weights fill with all available indices
	if(weights.size() <= 4)
	{
		for(std::size_t i = 0; i < weights.size(); ++i)
		{
			selected[i] = i;
		}
		return selected;
	}

	for(std::size_t i = 0; i < 4; ++i)
	{
		selected[i] = i;
	}

	std::sort(selected.begin(), selected.end(),
	          [&](int i1, int i2) { return weights[i1] < weights[i2];}
	);

	//Keep the selected value indices in ascending order by value. While checking new values, compare with the smallest
	//of selected. If new value is bigger than previous smallest of biggest, put new value as smallest of biggest
	//and start shifting to valid position.
	//Computational complexity is O(n), because we do at most 4 shifts for every value.

	for(std::size_t i = 4; i < weights.size(); ++i)
	{
		if(weights[selected[0]] < weights[i])
		{
			selected[0] = i;
			std::size_t curr_ind = 0;
			while(curr_ind < 3 && weights[selected[curr_ind]] > weights[selected[curr_ind + 1]])
			{
				std::swap(selected[curr_ind], selected[curr_ind + 1]);
				++curr_ind;
			}
		}
	}
	return selected;
}
