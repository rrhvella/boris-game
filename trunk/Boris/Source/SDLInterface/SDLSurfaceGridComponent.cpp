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

#include <SDLInterface/SDLSurfaceGridComponent.h>
#include <SDLInterface/SDLForm.h>

using namespace SDLInterfaceLibrary;

void SDLSurfaceGridComponent::Blit()
{
	previousDimensions = Dimensions2D<int>(GetLeftFromParent() - GetCurrentSurfaceGrid().GetAnchor().x,
		GetTopFromParent() - GetCurrentSurfaceGrid().GetAnchor().y, GetWidth(), GetHeight());

	previousOffsetToOrigin = Vector2D<int>(GetLeftFromOrigin() - GetCurrentSurfaceGrid().GetAnchor().x,
		GetTopFromOrigin() - GetCurrentSurfaceGrid().GetAnchor().y);

	if(clearingMethod == SDL_CM_PRECISE)
	{
		if(previouslyBlittedSurfaceGrid != NULL)
		{
			delete previouslyBlittedSurfaceGrid;
		}

		previouslyBlittedSurfaceGrid = new SDLSurfaceGrid(GetCurrentSurfaceGrid());
	}


	GetCurrentSurfaceGrid().Blit(Vector2D<int>(GetLeftFromOrigin(), GetTopFromOrigin()), GetScreen());
}

void SDLSurfaceGridComponent::Clear()
{
	if(clearingMethod == SDL_CM_BOUNDING_BOX)
	{
		SDLComponent::Clear();
	}
	else
	{
		for(int x = 0; x < previouslyBlittedSurfaceGrid->GetWidth(); x++)
		{
			for(int y = 0; y < previouslyBlittedSurfaceGrid->GetHeight(); y++)
			{
				SDL_Surface* surfaceToClear = previouslyBlittedSurfaceGrid->GetSurface(Vector2D<int>(x, y));

				if(surfaceToClear != NULL)
				{
					Vector2D<int> surfaceDrawingOrigin(x * previouslyBlittedSurfaceGrid->GetSurfaceSize().width,
						y * previouslyBlittedSurfaceGrid->GetSurfaceSize().height);

					Vector2D<int> surfacePositionOnParent = surfaceDrawingOrigin;

					surfaceDrawingOrigin += previousOffsetToOrigin;
					surfacePositionOnParent += previousDimensions.position;

					SDL_Rect offset;

					offset.x = surfaceDrawingOrigin.x;
					offset.y = surfaceDrawingOrigin.y;

					SDL_Rect segmentRect;

					segmentRect.x = surfacePositionOnParent.x;
					segmentRect.y = surfacePositionOnParent.y;

					segmentRect.w = previouslyBlittedSurfaceGrid->GetSurfaceSize().width;
					segmentRect.h =  previouslyBlittedSurfaceGrid->GetSurfaceSize().height;

					if(SDL_BlitSurface(GetParent()->GetImage(), &segmentRect, GetScreen(), &offset) == -1)
					{
						throw SDLException();
					}
				}
			}
		}
	}
}


SDLSurfaceGridComponent::SDLSurfaceGridComponent(const string& name, const Vector2D<int>& position,
	const vector<SDLSurfaceGrid>& surfaceGrids, SDLClearingMethod clearingMethod):
	SDLComponent(name, position, NULL)
{
	if(surfaceGrids.size() == 0)
	{
		string error = "Error when initializing " + name + ": Attempted to pass empty surface grid collection.";
	}

	this->clearingMethod = clearingMethod;

	this->surfaceGrids = surfaceGrids;
	currentSurfaceGridIndex = 0;

	previouslyBlittedSurfaceGrid = NULL;
}

void SDLSurfaceGridComponent::First()
{
	currentSurfaceGridIndex = 0;
	Update();
}

void SDLSurfaceGridComponent::Next()
{
	if(currentSurfaceGridIndex < surfaceGrids.size() - 1)
	{
		currentSurfaceGridIndex++;
	}
	else
	{
		currentSurfaceGridIndex = 0;
	}

	Update();
}

void SDLSurfaceGridComponent::Previous()
{
	if(currentSurfaceGridIndex > 0)
	{
		currentSurfaceGridIndex--;
	}
	else
	{
		currentSurfaceGridIndex = surfaceGrids.size() - 1;
	}

	Update();
}


SDLSurfaceGrid& SDLSurfaceGridComponent::GetCurrentSurfaceGrid()
{
	return surfaceGrids[currentSurfaceGridIndex];
}

const SDLSurfaceGrid& SDLSurfaceGridComponent::GetConstCurrentSurfaceGrid() const
{
	return surfaceGrids[currentSurfaceGridIndex];
}

int	SDLSurfaceGridComponent::GetWidth() const
{
	return GetConstCurrentSurfaceGrid().GetWidth() *
		GetConstCurrentSurfaceGrid().GetSurfaceSize().width;
}

int SDLSurfaceGridComponent::GetHeight() const
{
	return GetConstCurrentSurfaceGrid().GetHeight() *
		GetConstCurrentSurfaceGrid().GetSurfaceSize().height;
}

void SDLSurfaceGridComponent::ClearCurrentSurfaces(Dimensions2D<int>* portionToClear)
{
	GetCurrentSurfaceGrid().ClearSurfaceGrid(portionToClear);
	Update();
}

void SDLSurfaceGridComponent::ReplaceCurrentSurfaces(const SDLSurfaceGrid& surfaceGrid,
	const Vector2D<int>& positionToCopyTo, bool nullOverwrite, Dimensions2D<int>* portionToCopy)
{
	GetCurrentSurfaceGrid().Replace(surfaceGrid, positionToCopyTo, nullOverwrite, portionToCopy);
	Update();
}


SDLSurfaceGrid SDLSurfaceGridComponent::CopyCurrentSurfaces(Dimensions2D<int>* portionToCopy)
{
	return GetCurrentSurfaceGrid().Copy(portionToCopy);
}


bool SDLSurfaceGridComponent::SurfacesCollide(const SDLSurfaceGridComponent& target, bool treatOutsideBoundsAsCollision)
{
	const Bounds2D<int> surfaceSize = this->GetCurrentSurfaceGrid().GetSurfaceSize();

	if(surfaceSize != target.GetConstCurrentSurfaceGrid().GetSurfaceSize())
	{
		return false;
	}

	Vector2D<int> positionOnMainSurface(GetLeftFromOrigin(), GetTopFromOrigin());
	Vector2D<int> targetPositionOnMainSurface(target.GetLeftFromOrigin(), target.GetTopFromOrigin());

	positionOnMainSurface -= GetCurrentSurfaceGrid().GetAnchor();
	targetPositionOnMainSurface -= target.GetConstCurrentSurfaceGrid().GetAnchor();


	Vector2D<int> distanceBetweenThisAndTarget = targetPositionOnMainSurface - positionOnMainSurface;

	if(distanceBetweenThisAndTarget.x % surfaceSize.width != 0 ||
		distanceBetweenThisAndTarget.y % surfaceSize.height != 0)
	{
		return false;
	}

	distanceBetweenThisAndTarget.x /= surfaceSize.width;
	distanceBetweenThisAndTarget.y /= surfaceSize.height;

	const SDLSurfaceGrid& targetSurfaceGrid = target.GetConstCurrentSurfaceGrid();

	for(int x = 0; x < targetSurfaceGrid.GetWidth(); x++)
	{
		for(int y = 0; y < targetSurfaceGrid.GetHeight(); y++)
		{
			SDL_Surface* targetSurface = targetSurfaceGrid.GetSurface(Vector2D<int>(x, y));
			SDL_Surface* mySurface = NULL;


			try
			{
				mySurface = GetCurrentSurfaceGrid().GetSurface(Vector2D<int>(x, y) +
										distanceBetweenThisAndTarget);

				if(mySurface != NULL && targetSurface != NULL)
				{
					return true;
				}

			}
			catch(GridException& outOfBoundsError)
			{
				if(treatOutsideBoundsAsCollision && targetSurface != NULL)
				{
					return true;
				}
			}
		}
	}

	return false;
}

SDLSurfaceGridComponent::~SDLSurfaceGridComponent()
{
	if(previouslyBlittedSurfaceGrid != NULL)
	{
		delete previouslyBlittedSurfaceGrid;
	}
}
