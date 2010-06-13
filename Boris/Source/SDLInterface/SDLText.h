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

#ifndef SDL_TEXT_H
#define SDL_TEXT_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include <SDLInterface/SDLFontFile.h>
#include <SDLInterface/TTFException.h>
#include <SDLInterface/SDLComponent.h>
#include <Helpers/IUncopyable.h>
#include <Helpers/SDLHelperFunctions.h>

namespace SDLInterfaceLibrary
{
	/*
		Class: SDLText

		This component, rather than having a static image, contains a string which it then
		converts to an image using the TTF library. This is useful for displaying
		and altering interactive and dynamic text in your application. For the purposes
		of this library however, it is treated as any other component.

		See Also:
			<SDLComponent>
	*/
	class SDLText: public SDLComponent, public IUncopyable
	{
		protected:
			string text;
			SDLFontFile* fontFile;

			SDL_Color textColour;
			//Returns the text as an SDL_Surface pointer.
			virtual SDL_Surface* GetTextImage() const;
		public:
			/*
				Constructor: SDLText

				name- The name of the component--This is used to identify which
							component has failed if an exception occurs.

				position - The position of the component on the parent, not the screen.

				text - The text which will be displayed on the screen.

				fontFile - Pointer to the SDLFontFile which will be used to create the text image.

				textColour - The colour of the text.

				See Also:
					<SDLFontFile>
			*/
			SDLText(const string& name, const Vector2D<int>& position, string text, SDLFontFile* fontFile,
				SDL_Color textColour);

			/*
				Function: GetText

				Returns:
					The text currently being displayed.
			*/
			const string& GetText() const;
			/*
				Function: SetText

				newText - The new text which will be displayed.
			*/
			virtual void SetText(const string& newText);
			/*
				Destructor: SDLText

				Unlike other components, the image displayed by this one is dynamic,
				and tied to it's lifetime. Therefore it must be removed from memory
				when this component is destroyed.
			*/
			virtual ~SDLText();


	};
}

#endif
