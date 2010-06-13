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

#ifndef SDL_TEXT_BOX_H
#define SDL_TEXT_BOX_H

#include <string>
#include <map>

#include <SDLInterface/SDLText.h>
#include <SDLInterface/SDLFontFile.h>
#include <SDLInterface/SDLException.h>
#include <EventHandling/KeyEventComponent.h>
#include <EventHandling/InputEventHandler.h>
#include <Helpers/StringHelperFunctions.h>

using std::string;
using std::map;
using namespace EventHandling;

namespace SDLInterfaceLibrary
{
	/*
		Class: SDLTextBox

		This component is a child of both SDLText, and the EventHandling base class KeyEventComponent.
		This means that this component can capture key strokes from an SDLForm if focus is set on it.
		It translates these key strokes into ASCII characters which it adds to it's own internal string.
		This component is extremely useful for creating interactive input in your application.

		See Also:
			<SDLText>
			<SDLForm>
			<SDLForm::SetFocus>
			<SDLTextBox::HandleInput>
	*/
	class SDLTextBox: public SDLText, public KeyEventComponent
	{
		private:
			InputEventHandlerCollection textInputHandler;
			//The shift map specifies the character a keysymbol should be converted to if
			//shift is pressed.
			map<char, char> shiftMap;
			unsigned int maxLength;

		public:
			/*
				Constructor: SDLTextBox

				name- The name of the component--This is used to identify which
							component has failed if an exception occurs.

				position - The position of the component on the parent, not the screen.

				text - The text which will be displayed on the screen.

				fontFile - Pointer to the SDLFontFile which will be used to create the text image.

				maxLength - The maximum number of characters the user can enter.

				textColour - The colour of the text.

				See Also:
					<SDLFontFile>
			*/
			SDLTextBox(const string& name, const Vector2D<int>& position, const char* text,
				SDLFontFile* fontFile, unsigned int maxLength, SDL_Color textColour);

			/*
				Function: HandleInput

				This method is an event handler which is called whenever a key is pressed.
				To put it simply:

				- This method takes a key stroke from SDL.
				- If shift is pressed, this method converts that key stroke to the proper ascii character.
				- The resulting character is then added to this component's internal string, and displayed to
				the user.

				The backspace character has also been implemented. Arrow keys, delete, home and end haven't.
				Pressing enter will raise and input handler event.

				Some other limitations to take note of:

				- SDL does not recognize the following keys when they are pressed: [, ], \.
				- This function is mapped to a standard American keyboard layout, and as such no other
			      layout is yet supported.

				See Also:
					<SDLTextBox::GetTextInputHandler>
			*/
			void HandleInput(int keySymbol);
			/*
				Function: GetTextInputHandler

				Returns:
					A read/write reference to the input event handler collection. These
					functions will be called when the user presses enter while this component
					has focus. The internal string is then passed to these functions.
			*/
			InputEventHandlerCollection& GetTextInputHandler();
			/*
				Function: SetText

				newText - The new text which will be displayed. Strings which
				are passed to this function are cropped to the max length.
			*/
			void SetText(string& newText);

	};
}

#endif
