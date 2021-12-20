#include "Helper.h"

uint32_t Helper::deserializeColor(String s)
{
	s.replace("#", "");
	return strtoul(s.c_str(), NULL, 16);
}

uint32_t Helper::deserializeInt(String s)
{
	return strtoul(s.c_str(), NULL, 10);
}

uint32_t Helper::deserializeColor(byte *bytes, unsigned int length)
{
	char *copy = new char[length + 1];
	memcpy(copy, bytes, length);
	copy[length] = 0;

	char *dst = copy;
	if (dst[0] == '#')
		++dst;

	uint32_t i = strtoul(dst, NULL, 16);
	delete[] copy;

	return i;
}

uint32_t Helper::deserializeInt(byte *bytes, unsigned int length)
{
	char *copy = new char[length + 1];
	memcpy(copy, bytes, length);
	copy[length] = 0;

	uint32_t i = strtoul(copy, NULL, 10);
	delete[] copy;

	return i;
}

String Helper::serializeColor(uint32_t value)
{
	String s = "000000000" + String(value, HEX);
	return String("#") + s.substring(s.length() - 6);
}

String Helper::serializeInt(uint32_t value)
{
	return String(value, DEC);
}
