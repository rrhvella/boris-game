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

#include <SDLInterface/SDLEffects.h>
#include <SDLInterface/SDLComponent.h>

using namespace SDLInterfaceLibrary;

LoomEffectInfo::LoomEffectInfo()
{
	this->currentSize = 1;
	this->increasing = true;
}

LoomEffect::LoomEffect(double rateOfIncrease, double maxSize)
{
	this->rateOfIncrease = rateOfIncrease;
	this->maxSize = maxSize;
}

ISDLEffectInfo* LoomEffect::GetEffectInfo() const
{
	return new LoomEffectInfo();
}

SDL_Surface* LoomEffect::GetEffectImage(SDLComponent* target, int& effectOffsetX, int& effectOffsetY) const
{
	SDL_Surface* targetImage = target->GetImage();
	LoomEffectInfo* info = (LoomEffectInfo*)target->GetEffectInfo();

	if(info->increasing)
	{
		if(maxSize - info->currentSize > rateOfIncrease)
		{
			info->currentSize += rateOfIncrease;
		}
		else
		{
			info->currentSize = maxSize;
			info->increasing = false;
		}
	}
	else
	{
		if(info->currentSize - 1 > rateOfIncrease)
		{
			info->currentSize -= rateOfIncrease;
		}
		else
		{
			info->currentSize = 1;
			info->increasing = true;
		}
	}

	SDL_Surface* newImage = zoomSurface(targetImage, info->currentSize, info->currentSize, 1);

	effectOffsetX = -(newImage->w - targetImage->w)/2;
	effectOffsetY = -(newImage->h - targetImage->h)/2;

	target->Update();

	return newImage;
}
