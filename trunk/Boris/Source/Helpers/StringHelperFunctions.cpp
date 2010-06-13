/*
	BORIS - Copyright (c) 2010 Robert Vella - robert.r.h.vella@gmail.com

	This software is provided 'as-is', without any express or
	implied warranty. In no event will the authors be held
	liable for any damages arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute
	it freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented;
	   you must not claim that you wrote the original software.
	   If you use this software in a product, an acknowledgment
	   in the product documentation would be appreciated but
	   is not required.

	2. Altered source versions must be plainly marked as such,
	   and must not be misrepresented as being the original software.

	3. This notice may not be removed or altered from any
	   source distribution.
*/

#include <Helpers/StringHelperFunctions.h>

string Helpers::ToLower(const string& stringToConvert)
{
	string convertedString = stringToConvert;

	for(unsigned int i = 0; i < stringToConvert.size(); i++)
	{
		char convertedCharacter =  tolower(stringToConvert[i]);
		convertedString[i] = convertedCharacter;
	}

	return convertedString;
}

string Helpers::ToStringList(const vector<string>& stringVector)
{
	string stringList = "";

	for(vector<string>::const_iterator currentString = stringVector.begin();
		currentString != stringVector.end();
		currentString++)
	{
		stringList += *currentString;

		//Append a comma after all the members, except for the last one.
		if(currentString != stringVector.end() - 1)
		{
			stringList += ", ";
		}
	}

	return stringList;
}


string Helpers::CropToMaxLength(const char* textToCrop, const unsigned int maxLength)
{
	string result = textToCrop;

	if(result.size() > maxLength)
	{
		result.erase(result.end() - (result.size() - maxLength), result.end());
	}

	return result;
}
