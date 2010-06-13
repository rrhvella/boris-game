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

#include <SDLInterface/SDLScreenEffects.h>
#include <SDLInterface/SDLInstance.h>

using std::vector;
using namespace SDLInterfaceLibrary;
using namespace Helpers;

SDL_Surface* ISDLScreenEffect::CreateScreenCopy(SDL_Surface* screen,
		const Vector2D<int>& copyPosition, const Bounds2D<int>& copyWidth)
{
	SDL_Surface* originalImage = CreateSurface(copyWidth, screen->format->BitsPerPixel);

	for(vector< Dimensions2D<int> >::iterator currentPortion = portions.begin();
		currentPortion != portions.end(); currentPortion++)
	{
		SDL_Rect copyPositionRect;

		copyPositionRect.x = currentPortion->position.x - copyPosition.x;
		copyPositionRect.y = currentPortion->position.y - copyPosition.y;


		SDL_Rect portionToCopy;

		portionToCopy.x = currentPortion->position.x;
		portionToCopy.y = currentPortion->position.y;

		portionToCopy.w = currentPortion->size.width;
		portionToCopy.h = currentPortion->size.height;


		if(SDL_BlitSurface(screen, &portionToCopy, originalImage, &copyPositionRect) != 0)
		{
			throw SDLException();
		}
	}

	return originalImage;
}


void ISDLScreenEffect::DrawFrames(SDL_Surface* screen, const Vector2D<int>& copyPosition,
		const vector<SDL_Surface*>& frames, FPSmanager* frameRateManager)
{
	SDL_EventState(SDL_KEYDOWN, SDL_IGNORE);

	for(int currentIteration = 0; currentIteration < numberOfIterations; currentIteration++)
	{
		for(vector<SDL_Surface*>::const_iterator currentFrame = frames.begin();
				currentFrame != frames.end(); currentFrame++)
		{
			if(delayBetweenFrames < 1) 
			{
				SDL_framerateDelay(frameRateManager);
			}
			else 
			{
				SDL_Delay(delayBetweenFrames);
			}

			DrawFrame(screen, copyPosition, *currentFrame);
		}
	}

	SDL_EventState(SDL_KEYDOWN, SDL_ENABLE);
}

void ISDLScreenEffect::DrawFrame(SDL_Surface* screen, const Vector2D<int>& copyPosition, SDL_Surface* frame)
{
	SDL_Rect position;

	position.x = copyPosition.x;
	position.y = copyPosition.y;

	if(SDL_BlitSurface(frame, NULL, screen, &position) == -1)
	{
		throw SDLException();
	}

	if(SDL_Flip(screen) == -1)
	{
		throw SDLException();
	}

}

ISDLScreenEffect::ISDLScreenEffect(const vector<Dimensions2D<int> >& portions, int numberOfIterations, 
								int delayBetweenFrames, bool requiresDraw)
{
	this->requiresDraw = requiresDraw;
	this->portions = portions;

	if(numberOfIterations < 1)
	{
		numberOfIterations = 1;
	}

	this->numberOfIterations = numberOfIterations;
	this->delayBetweenFrames = delayBetweenFrames;
}

bool ISDLScreenEffect::RequiresDraw()
{
	return requiresDraw;
}

void ISDLScreenEffect::Draw(SDLInstance* instance)
{
	SDL_Surface* screen = instance->GetScreen();
	FPSmanager* frameRateManager = instance->GetFrameRateManager();

	Dimensions2D<int> boundingRectangle = GetBoundingRectangle(portions);

	Vector2D<int> copyPosition = boundingRectangle.position;
	Bounds2D<int> copyWidth = boundingRectangle.size;

	SDL_Surface* originalImage = CreateScreenCopy(screen, copyPosition, copyWidth);

	vector<SDL_Surface*> frames = GenerateFrames(originalImage);

	DrawFrames(screen, copyPosition, frames, frameRateManager);

	SDL_FreeSurface(originalImage);

	for(vector<SDL_Surface*>::iterator currentFrame = frames.begin();
		currentFrame != frames.end();
		currentFrame++)
	{
		SDL_FreeSurface(*currentFrame);
	}
}

ISDLScreenEffect::~ISDLScreenEffect() {
}

FlashScreenEffect::FlashScreenEffect(const vector< Dimensions2D<int> >& portions, int numberOfFrames,
	int numberOfIterations): ISDLScreenEffect(portions, numberOfIterations, -1, false)
{
	if(numberOfFrames < 1)
	{
		numberOfFrames = 1;
	}

	this->numberOfFrames = numberOfFrames;
}

vector<SDL_Surface*> FlashScreenEffect::GenerateFrames(SDL_Surface* originalImage)
{
	vector<SDL_Surface*> frames;

	for(int i = 0; i < numberOfFrames; i++)
	{
		int alpha = 255/numberOfFrames * i;
		frames.push_back(CreateOverlaidCopy(originalImage, 255, 255, 255, alpha));
	}

	frames.push_back(CreateOverlaidCopy(originalImage, 255, 255, 255, 255));

	return frames;
}

OverlayScreenEffect::OverlayScreenEffect(const vector< Dimensions2D<int> >& portions,
				int red, int green, int blue, int alpha):
	ISDLScreenEffect(portions, 1, -1, false)
{
	this->red = red;
	this->green = green;
	this->blue = blue;
	this->alpha = alpha;
}

vector<SDL_Surface*> OverlayScreenEffect::GenerateFrames(SDL_Surface* originalImage)
{
	vector<SDL_Surface*> frames;

	frames.push_back(CreateOverlaidCopy(originalImage, red, green, blue, alpha));

	return frames;
}



FadeScreenEffect::FadeScreenEffect(const vector< Dimensions2D<int> >& portions,
		int numberOfFrames, unsigned int animationLength, SDLFadeDirection fadeDirection):
		ISDLScreenEffect(portions, 1, (int)(animationLength / (float)numberOfFrames), fadeDirection == SDL_FD_IN)
{
	if(numberOfFrames < 1)
	{
		numberOfFrames = 1;
	}

	this->numberOfFrames = numberOfFrames;
	this->fadeDirection = fadeDirection;
}

vector<SDL_Surface*> FadeScreenEffect::GenerateFrames(SDL_Surface* originalImage)
{
	vector<SDL_Surface*> frames;

	for(int i = 0; i < numberOfFrames; i++)
	{
		int alpha = 255/numberOfFrames * ((fadeDirection == SDL_FD_IN)? numberOfFrames - i: i);
		frames.push_back(CreateOverlaidCopy(originalImage, 0, 0, 0, alpha));
	}

	frames.push_back(CreateOverlaidCopy(originalImage, 0, 0, 0, (fadeDirection == SDL_FD_IN)? 0: 255));

	return frames;
}
