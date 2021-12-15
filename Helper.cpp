#include "Helper.h"

String Helper::rawDataToString(byte *raw, unsigned int length)
{
	char *copy = new char[length + 1];
	memcpy(raw, copy, length);
	copy[length] = 0;

	String s = String(copy);
	delete[] copy;

	return s;
}

uint32_t Helper::deserializeColor(String s)
{
	s.replace("#", "");
	return strtoul(s.c_str(), NULL, 16);
}

uint32_t Helper::deserializeInt(String s)
{
	return strtoul(s.c_str(), NULL, 10);
}

String Helper::serializeColor(uint32_t value)
{
	String s = "000000000" + String(value, HEX);
	return s.substring(s.length() - 6);
}

String Helper::serializeInt(uint32_t value)
{
	return String(value, DEC);
}
