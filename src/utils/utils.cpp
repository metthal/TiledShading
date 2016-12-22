#include "utils/utils.h"

bool endsWith(const std::string& str, const std::string& suffix)
{
	auto pos = str.rfind(suffix);
	if (pos == std::string::npos)
		return false;

	return pos + suffix.length() == str.length();
}