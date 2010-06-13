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

#ifndef SDL_EXCEPTION_H
#define SDL_EXCEPTION_H

#include <SDL/SDL.h>

#include <Helpers/ApplicationException.h>

using namespace Helpers;

namespace SDLInterfaceLibrary
{
	/*
	Class: SDLException

	General exception class, to indicate that the error occured while calling SDL
	*/
	class SDLException: public ApplicationException
	{
		public:
			SDLException():
				ApplicationException(SDL_GetError())
				{
				}
	};
}
#endif
