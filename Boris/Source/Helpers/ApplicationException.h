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

#ifndef APPLICATION_EXCEPTION_H
#define APPLICATION_EXCEPTION_H

#include <exception>
#include <string>

using std::exception;
using std::string;

namespace Helpers
{
	/*
	Class: ApplicationException

	This is a basic wrapper class to the standard C++ exception class,
	which allows you to specify the error message inline.

	Example:

	throw ApplicationException("You have made an error, please fix it");
	*/
	class ApplicationException: public exception
	{
		private:
			string errorMessage;
		public:
			/*
			Constructor: ApplicationException

			Parameters:
				errorMessage - The error message you would like to display to the user.
			*/
			ApplicationException(const char* errorMessage);

			/*
			Function: what

			This function is inherited from the base exception class.

			Returns:
				The error message previously entered when intializing this class.
			*/
            const char* what() const throw();

            ~ApplicationException() throw() {};
	};
}
#endif
