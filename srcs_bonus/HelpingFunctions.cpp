#include "General.hpp"

bool    isValidLimit(const std::string& limit)
{
	if (limit.size() == 0)
		return false;
	for (size_t i = 0; i < limit.size(); ++i)
	{
		if (!std::isdigit(limit[i]))
			return false;
		if (i == 9 && i + 1 != limit.size())
			return false;
		if (i == 9 && i + 1 == limit.size() && limit > "4294967295")
			return false;
		if (i == 10)
			return false;
	}
	return true;
}
