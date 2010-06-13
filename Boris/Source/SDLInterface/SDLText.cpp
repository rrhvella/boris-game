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

#include <SDLInterface/SDLText.h>

using namespace SDLInterfaceLibrary;

SDL_Surface* SDLText::GetTextImage() const
{
	//TTF_Font can't process an empty string, ergo NULL must be returned so as to not cause an exception.
	if(text.size() == 0 || fontFile == NULL)
	{
		return NULL;
	}

	SDL_Surface* textImage = TTF_RenderText_Solid(fontFile->GetFont(), text.c_str(), textColour);

	if(textImage == NULL)
	{
		throw TTFException();
	}

	return textImage;
}

SDLText::SDLText(const string& name, const Vector2D<int>& position, string text, SDLFontFile* fontFile,
			SDL_Color textColour):SDLComponent(name, position, NULL)
{
	this->text = text;
	this->textColour = textColour;

	this->fontFile = fontFile;

	this->image = GetTextImage();
}

const string& SDLText::GetText() const
{
	return text;
}

void SDLText::SetText(const string& newText)
{
	this->text = newText;

	if(image != NULL)
	{
		SDL_FreeSurface(image);
	}

	image = GetTextImage();

	Update();
}

SDLText::~SDLText()
{
	if(image != NULL)
	{
		SDL_FreeSurface(image);
	}
}
