#include "MorphPicker.hpp"
#include <numeric>

std::array<int, 4> pickMorphs(const std::vector<float>& weights)
{
	std::array<int, 4> retval{};
	retval.fill(-1);

	if(weights.size() <= 4)
	{
		for(size_t i = 0; i < weights.size(); ++i)
		{
			retval[i] = i;
		}
		return retval;
	}

	std::iota(retval.begin(), retval.end(), 0);
	std::sort(retval.begin(), retval.end(),
			[&](int i1, int i2) { return weights[i1] < weights[i2];}
	);

	for(std::size_t i = 4; i < weights.size(); ++i)
	{
		if(weights[retval[0]] < weights[i])
		{
			retval[0] = i;
			std::size_t curr_ind = 0;
			while(curr_ind < 3 && weights[retval[curr_ind]] > weights[retval[curr_ind+1]])
			{
				std::swap(retval[curr_ind], retval[curr_ind+1]);
				++curr_ind;
			}
		}
	}
	return retval;
}
