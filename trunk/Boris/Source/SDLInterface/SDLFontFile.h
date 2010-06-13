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

#ifndef SDL_FONT_FILE_H
#define SDL_FONT_FILE_H

#include <SDL/SDL_ttf.h>

#include <SDLInterface/TTFException.h>
#include <Helpers/IUncopyable.h>

namespace SDLInterfaceLibrary
{

	/*
	Class: SDLFontFile

	Specifies a TTF_Font with a specific size and path.
	*/
	class SDLFontFile: public IUncopyable
	{
		private:
			TTF_Font* font;
		public:

			/*
				Constructor: SDLFontFile

				Creates a TTF_Font based on the font file path, and text size passed.

				Parameters:
					fontFilePath - The path of the tff font file which will be used to create the font.
					textSize - The size of the font.
			*/
			SDLFontFile(const char* fontFilePath, int textSize);
			/*
				Function: GetFont

				Returns:
					A pointer to the TTF_Font created in this class.
			*/
			TTF_Font* GetFont() const;
			/*
				Destructor: SDLFontFile

				Removes the TTF_Font from memory.
			*/
			~SDLFontFile();
	};
}

#endif
