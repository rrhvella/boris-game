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

#include <SDLInterface/SDLTextBox.h>

using namespace SDLInterfaceLibrary;
using namespace EventHandling;

SDLTextBox::SDLTextBox(const string& name, const Vector2D<int>& position, const char* text,
		SDLFontFile* fontFile, unsigned int maxLength, SDL_Color textColour):
	SDLText(name, position, CropToMaxLength(text, maxLength), fontFile, textColour)
{
	keyDownHandlers.AddCppEventHandler<SDLTextBox>(this, &SDLTextBox::HandleInput);
	this->maxLength = maxLength;

	shiftMap['1'] = '!';
	shiftMap['2'] = '@';
	shiftMap['3'] = '#';
	shiftMap['4'] = '$';
	shiftMap['5'] = '%';
	shiftMap['6'] = '^';
	shiftMap['7'] = '&';
	shiftMap['8'] = '*';
	shiftMap['9'] = '(';
	shiftMap['0'] = ')';
	shiftMap['`'] = '~';
	shiftMap['-'] = '_';
	shiftMap['='] = '+';
	shiftMap[';'] = ':';
	shiftMap['\''] = '"';
	shiftMap[','] = '<';
	shiftMap['.'] = '>';
	shiftMap['/'] = '?';
}

void SDLTextBox::HandleInput(int keySymbol)
{
	if(keySymbol >= SDLK_a && keySymbol <= SDLK_z ||
		keySymbol >= SDLK_SPACE && keySymbol <= SDLK_AT)
	{
		if(text.size() < maxLength)
		{
			char convertedSymbol = keySymbol;

			if(SDL_GetKeyState(NULL)[SDLK_LSHIFT] || SDL_GetModState() & KMOD_CAPS)
			{
				if(keySymbol >= SDLK_a && keySymbol <= SDLK_z)
				{
					if(SDL_GetKeyState(NULL)[SDLK_LSHIFT])
					{
						if(SDL_GetModState() | KMOD_CAPS)
						{
							convertedSymbol = keySymbol - 32;
						}
					}
					else if(SDL_GetModState() & KMOD_CAPS)
					{
						convertedSymbol = keySymbol - 32;
					}
				}
				else if(SDL_GetKeyState(NULL)[SDLK_LSHIFT])
				{
					map<char, char>::iterator foundSymbol = shiftMap.find(keySymbol);

					if(foundSymbol != shiftMap.end())
					{
						convertedSymbol = foundSymbol->second;
					}
				}
			}

			text += convertedSymbol;
			SetText(text);
		}
	}
	else if(keySymbol == SDLK_BACKSPACE)
	{
		if(text.size() > 0)
		{
			text.erase(text.end() - 1);
		}

		SetText(text);
	}
	else if(keySymbol == SDLK_RETURN)
	{
		textInputHandler.RaiseEvents(text.c_str());
	}
}

InputEventHandlerCollection& SDLTextBox::GetTextInputHandler()
{
	return textInputHandler;
}

void SDLTextBox::SetText(string& newText)
{
	string croppedText = CropToMaxLength(newText.c_str(), maxLength);
	SDLText::SetText(croppedText);
}
