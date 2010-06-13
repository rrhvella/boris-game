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

#ifndef STRING_HELPER_FUNCTIONS_H
#define STRING_HELPER_FUNCTIONS_H

#include <string>
#include <vector>

using std::string;
using std::vector;


/*
File: StringHelperFunctions.h

Contains a collection of basic string formatting functions.
*/
namespace Helpers
{
	/*
		Function: ToLower

		Returns:
			A copy of the string, stringToConvert, with all the characters within it formatted
			to a lower case.

			Example:

			"Hello World!" becomes "hello world!"
	*/
	extern string ToLower(const string& stringToConvert);

	/*
		Function: ToStringList

		Returns:
			Returns a comma seperated string containing all the members of stringVector.

			Example:

			{"Hello", "World"} becomes "Hello, World"
	*/
	extern string ToStringList(const vector<string>& stringVector);

	/*
		Function: CropToMaxLength

		Returns:
			A copy of the string, textToCrop, cropped to the length specified in
			maxLength.
	*/
	extern string CropToMaxLength(const char* textToCrop, const unsigned int maxLength);
}

#endif
