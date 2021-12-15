#ifndef INCLUDE_SRC_UTIL_H
#define INCLUDE_SRC_UTIL_H

#include <Arduino.h>

class Helper
{
	public:

		static uint32_t deserializeColor(String s);
		static uint32_t deserializeInt(String s);
		static uint32_t deserializeColor(byte *bytes, unsigned int length);
		static uint32_t deserializeInt(byte *bytes, unsigned int length);
		static String serializeColor(uint32_t value);
		static String serializeInt(uint32_t value);
};

#endif
