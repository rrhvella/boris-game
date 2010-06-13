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

#ifndef SDL_HELPER_FUNCTIONS_H
#define SDL_HELPER_FUNCTIONS_H

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

#include <Helpers/GeometricStructures2D.h>
/*
File: SDLHelperFunctions.h

Contains a collection of helper functions for SDL.
*/

namespace Helpers
{
	/*
		Function: CreateSurface

		Parameters:
			size - The size of the new surface.
			colorDepth - The surface's colour depth in bits.

		Returns:
			An empty SDL_Surface with size [size] and color depth [colorDepth].
	*/
	extern SDL_Surface* CreateSurface(const Bounds2D<int>& size, int colorDepth);
	/*
		Function: CreateSurface

		Parameters:
			size - The size of the new surface.
			colorDepth - The surface's colour depth in bits.

			red - The R value of the new surface's RGBA color.
			green - The G value of the new surface's RGBA color.
			blue - The B value of the new surface's RGBA color.
			alpha - The A value of the new surface's RGBA color.

		Returns:
			An SDL_Surface with size [size] and color depth [colorDepth], filled with color
			{red, green, blue, alpha}.
	*/
	extern SDL_Surface* CreateSurface(const Bounds2D<int>& size, int colorDepth, Uint8 red,
							Uint8 green, Uint8 blue, Uint8 alpha);
	/*
		Function: CopySurface

		Parameters:
			srcSurface - The surface you wish to copy.

		Returns:
			A copy of srcSurface.
	*/
	extern SDL_Surface* CopySurface(SDL_Surface* srcSurface);
	/*
		Function: CopySurface

		Parameters:
			srcSurface - The surface you wish to copy.
			portion - The portion you wish to copy from srcSurface.

		Returns:
			A copy of those pixels in srcSurface which fall within the area [portion].
	*/
	extern SDL_Surface* CopySurface(SDL_Surface* srcSurface, Dimensions2D<int>& portion);
	/*
		Function: CopySurface

		Parameters:
			srcSurface - The surface you wish to copy.

			red - The R value which will be alpha blended into srcSurface.
			green - The G value which will be alpha blended into srcSurface.
			blue - The B value which will be alpha blended into srcSurface.
			alpha - The A value which will be alpha blended into srcSurface.

		Returns:
			A copy of srcSurface blended with the color {red, green, blue, alpha}
	*/
	extern SDL_Surface* CreateOverlaidCopy(SDL_Surface* srcSurface, int red, int green, int blue, int alpha);
}
#endif
