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

#include <SDLInterface/SDLSurfaceGrid.h>

using namespace std;
using namespace SDLInterfaceLibrary;
using namespace Helpers;

void SDLSurfaceGrid::ConfirmSurfaceList(const vector<SDL_Surface*>& surfaceList)
{
	for(unsigned int surfaceIndex = 0; surfaceIndex < surfaceList.size(); surfaceIndex++)
	{
		if(surfaceList[surfaceIndex] == NULL)
		{
			stringstream exceptionString;

			exceptionString << "Error in surface grid " << name << ": ";
			exceptionString << "surface, in surfacelist, at position " << surfaceIndex << "is null";

			throw SDLInterfaceLibraryException(exceptionString.str().c_str());
		}

		ConfirmSurfaceSize(surfaceList[surfaceIndex]);
	}
}

void SDLSurfaceGrid::ConfirmSurfaceSize(SDL_Surface* surface)
{
	if(surface->w != surfaceSize.width ||
			surface->h != surfaceSize.height)
	{
		stringstream exceptionString;

		exceptionString << "Error in surface grid " << name << ": ";
		exceptionString << "surface Width*Height (";
		exceptionString << surface->clip_rect.w << "*" << surface->clip_rect.h << ")";
		exceptionString << "does not conform to specification ";
		exceptionString << surfaceSize.ToString();

		throw SDLInterfaceLibraryException(exceptionString.str().c_str());
	}
}

SDLSurfaceGrid::SDLSurfaceGrid(const string& name, const Bounds2D<int>& surfaceGridSize,
		const Bounds2D<int>& surfaceSize, const Vector2D<int>& anchorBlockPosition,
		SDLSurfacePoint anchorPoint)
{
	this->name = name;

	this->surfaceSize = surfaceSize;
	this->anchorPoint = anchorPoint;

	surfaceGrid = new Grid<SDL_Surface*>(surfaceGridSize.width,
									surfaceGridSize.height);

	for(int x = 0; x < surfaceGrid->GetWidth(); x++)
	{
		for(int y = 0; y < surfaceGrid->GetHeight(); y++)
		{
			surfaceGrid->Set(x, y, NULL);
		}
	}

	SetAnchor(anchorBlockPosition, anchorPoint);
}

SDLSurfaceGrid::SDLSurfaceGrid(const string& name, const Grid<unsigned int>& surfacePlacementMap,
	const vector<SDL_Surface*>& surfaceList, const Vector2D<int>& anchorBlockPosition,
	SDLSurfacePoint anchorPoint)
{
	this->name = name;
	this->surfaceSize = Bounds2D<int>(surfaceList[0]->clip_rect.w,
					surfaceList[0]->clip_rect.h);


	ConfirmSurfaceList(surfaceList);

	surfaceGrid = new Grid<SDL_Surface*>(surfacePlacementMap.GetWidth(),
										surfacePlacementMap.GetHeight());

	for(int columnPosition = 0; columnPosition < surfaceGrid->GetWidth(); columnPosition++)
	{
		for(int rowPosition = 0; rowPosition < surfaceGrid->GetHeight(); rowPosition++)
		{
			unsigned int surfaceIndex = surfacePlacementMap.Get(columnPosition, rowPosition);

			if(surfaceIndex == 0)
			{
				surfaceGrid->Set(columnPosition, rowPosition, NULL);
			}
			else
			{
				Replace(surfaceList[surfaceIndex - 1], Vector2D<int>(columnPosition, rowPosition));
			}

		}
	}

	SetAnchor(anchorBlockPosition, anchorPoint);
}

void SDLSurfaceGrid::Blit(const Vector2D<int>& position, SDL_Surface* screen)
{
	if(screen != NULL)
	{
		for(int x = 0; x < surfaceGrid->GetWidth(); x++)
		{
			for(int y = 0; y < surfaceGrid->GetHeight(); y++)
			{
				SDL_Surface* surfaceToDraw = GetSurface(Vector2D<int>(x, y));

				if(surfaceToDraw != NULL)
				{
					Vector2D<int> surfaceDrawingOrigin(x * surfaceSize.width,
						y * surfaceSize.height);

					surfaceDrawingOrigin += position - anchor;

					SDL_Rect offset;

					offset.x = surfaceDrawingOrigin.x;
					offset.y = surfaceDrawingOrigin.y;

					if(SDL_BlitSurface(surfaceToDraw, NULL, screen, &offset) == -1)
					{
						throw SDLException();
					}
				}
			}
		}
	}
}


void SDLSurfaceGrid::Replace(SDL_Surface* surface,
							const Vector2D<int>& blockPosition)
{

	if(surface != NULL)
	{
		ConfirmSurfaceSize(surface);
	}

	surfaceGrid->Set(blockPosition.x, blockPosition.y, surface);
}

void SDLSurfaceGrid::Replace(const SDLSurfaceGrid& surfaceGrid, Vector2D<int> positionToCopyTo,
				bool nullOverwrite, Dimensions2D<int>* portionToCopy)
{
	int startingX = 0;
	int startingY = 0;

	int widthToTraverse = surfaceGrid.GetWidth();
	int heightToTraverse = surfaceGrid.GetHeight();

	if(portionToCopy != NULL)
	{
		startingX = portionToCopy->position.x;
		startingY = portionToCopy->position.y;

		widthToTraverse = portionToCopy->size.width;
		heightToTraverse = portionToCopy->size.height;
	}

	positionToCopyTo -= surfaceGrid.GetAnchorBlockPosition();

	for(int x = 0; x < widthToTraverse; x++)
	{
		for(int y = 0; y < heightToTraverse; y++)
		{
			Vector2D<int> blockPosition(startingX + x, startingY + y);
			SDL_Surface* currentSurface = surfaceGrid.GetSurface(blockPosition);

			if(currentSurface != NULL || nullOverwrite)
			{
				Replace(currentSurface, positionToCopyTo + blockPosition);
			}
		}
	}
}

SDLSurfaceGrid SDLSurfaceGrid::Copy(Dimensions2D<int>* portionToCopy) const
{
	int startingX = 0;
	int startingY = 0;

	int widthToTraverse = GetWidth();
	int heightToTraverse = GetHeight();

	if(portionToCopy != NULL)
	{
		startingX = portionToCopy->position.x;
		startingY = portionToCopy->position.y;

		widthToTraverse = portionToCopy->size.width;
		heightToTraverse = portionToCopy->size.height;
	}

	string newName = name + "Copy";
	SDLSurfaceGrid surfaceGridCopy(newName, Bounds2D<int>(widthToTraverse, heightToTraverse),
					surfaceSize, anchorBlockPosition, anchorPoint);

	for(int x = 0; x < widthToTraverse; x++)
	{
		for(int y = 0; y < heightToTraverse; y++)
		{
			Vector2D<int> blockPosition(startingX + x, startingY + y);
			SDL_Surface* currentSurface = GetSurface(blockPosition);

			if(currentSurface != NULL)
			{
				surfaceGridCopy.Replace(currentSurface, Vector2D<int>(x, y));
			}
		}
	}

	return surfaceGridCopy;
}

void SDLSurfaceGrid::ClearSurfaceGrid(Dimensions2D<int>* portionToClear)
{
	int startingX = 0;
	int startingY = 0;

	int widthToTraverse = this->GetWidth();
	int heightToTraverse = this->GetHeight();

	if(portionToClear != NULL)
	{
		startingX = portionToClear->position.x;
		startingY = portionToClear->position.y;

		widthToTraverse = portionToClear->size.width;
		heightToTraverse = portionToClear->size.height;
	}

	for(int x = 0; x < widthToTraverse; x++)
	{
		for(int y = 0; y < heightToTraverse; y++)
		{
			surfaceGrid->Set(startingX + x, startingY + y, NULL);
		}
	}
}

SDL_Surface* SDLSurfaceGrid::GetSurface(const Vector2D<int>& position) const
{
	return surfaceGrid->Get(position.x, position.y);
}

int SDLSurfaceGrid::GetWidth() const
{
	return surfaceGrid->GetWidth();
}

int SDLSurfaceGrid::GetHeight() const
{
	return surfaceGrid->GetHeight();
}

const Vector2D<int>& SDLSurfaceGrid::GetAnchor() const
{
	return anchor;
}

const Vector2D<int>& SDLSurfaceGrid::GetAnchorBlockPosition() const
{
	return anchorBlockPosition;
}

const Bounds2D<int>& SDLSurfaceGrid::GetSurfaceSize() const
{
	return surfaceSize;
}

void SDLSurfaceGrid::SetAnchor(const Vector2D<int>& anchorBlockPosition,
							SDLSurfacePoint anchorPoint)
{

	this->anchorBlockPosition = anchorBlockPosition;
	this->anchor = anchorBlockPosition;

	anchor.x *= surfaceSize.width;
	anchor.y *= surfaceSize.height;

	switch(anchorPoint)
	{
		case SDL_SSP_UPPER_LEFT_CORNER:
		{
			break;
		}
		case SDL_SSP_UPPER_RIGHT_CORNER:
		{
			anchor.x += surfaceSize.width;
			break;
		}
		case SDL_SSP_LOWER_LEFT_CORNER:
		{
			anchor.y += surfaceSize.height;
			break;
		}
		case SDL_SSP_LOWER_RIGHT_CORNER:
		{
			anchor.x += surfaceSize.width;
			anchor.y += surfaceSize.height;
			break;
		}
		case SDL_SSP_CENTER:
		{
			anchor.x += surfaceSize.width/2;
			anchor.y += surfaceSize.height/2;
			break;
		}
	}
}
