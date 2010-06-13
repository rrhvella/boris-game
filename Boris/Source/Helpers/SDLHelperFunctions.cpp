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

#include <Helpers/SDLHelperFunctions.h>

SDL_Surface* Helpers::CreateSurface(const Bounds2D<int>& size, int colorDepth)
{
	Uint32 rmask, gmask, bmask, amask;

	//Big endian/little endian check for different systems.
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xFF000000;
		gmask = 0x00FF0000;
		bmask = 0x0000FF00;
		amask = 0x000000FF;
	#else
		rmask = 0x000000FF;
		gmask = 0x0000FF00;
		bmask = 0x00FF0000;
		amask = 0xFF000000;
	#endif

	return SDL_CreateRGBSurface(SDL_SWSURFACE, size.width, size.height, colorDepth, rmask, gmask, bmask, amask);
}


SDL_Surface* Helpers::CreateSurface(const Bounds2D<int>& size, int colorDepth, Uint8 red,
						Uint8 green, Uint8 blue, Uint8 alpha)
{
	SDL_Surface* newSurface = CreateSurface(size, colorDepth);

	if(newSurface == NULL)
	{
		return NULL;
	}

	SDL_FillRect(newSurface, NULL, SDL_MapRGBA(newSurface->format, red, green, blue, alpha));
	return newSurface;
}

SDL_Surface* Helpers::CopySurface(SDL_Surface* srcSurface)
{
	SDL_Surface* newSurface = SDL_ConvertSurface(srcSurface, srcSurface->format, srcSurface->flags);
	return newSurface;
}

SDL_Surface* Helpers::CopySurface(SDL_Surface* srcSurface, Dimensions2D<int>& portion)
{
	SDL_Surface* newSurface = CreateSurface(portion.size, srcSurface->format->BitsPerPixel);

	if(newSurface == NULL)
	{
		return NULL;
	}

	SDL_Rect portionToCopy;

	portionToCopy.x = portion.position.x;
	portionToCopy.y = portion.position.y;

	portionToCopy.w = portion.size.width;
	portionToCopy.h = portion.size.height;

	if(SDL_BlitSurface(srcSurface, &portionToCopy, newSurface, &portionToCopy) != 0)
	{
		return NULL;
	}

	return newSurface;
}

SDL_Surface* Helpers::CreateOverlaidCopy(SDL_Surface* srcSurface, int red, int green, int blue, int alpha)
{
	SDL_Surface* newSurface = CopySurface(srcSurface);

	if(newSurface == NULL)
	{
		return NULL;
	}

	SDL_Surface* overlay = CreateSurface(Bounds2D<int>(srcSurface->w, srcSurface->h),
			srcSurface->format->BitsPerPixel, red, green, blue, alpha);

	if(SDL_BlitSurface(overlay, NULL, newSurface, NULL) == -1)
	{
		return NULL;
	}

	SDL_FreeSurface(overlay);

	return newSurface;
}
