#include "Stootils.h"
#include "Utilities.h"

// TODO: REMOVE THIS
int GetCharsSinceSpace(const char* text, UInt32 offset)
{
	const char* barPos = text + offset;
	int numChars = 0;

	while (barPos != text && !isalnum(*--barPos)) numChars++;
	while (barPos >= text && isalnum(*barPos--)) numChars++;

	return numChars;
}

int GetCharsTillSpace(const char* text, UInt32 offset)
{
	const char* barPos = text + offset;

	int numChars = 0;

	while (isalnum(*++barPos)) numChars++;
	while (*barPos && !isalnum(*barPos++)) numChars++;

	return numChars;
}