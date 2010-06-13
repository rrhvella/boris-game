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

#ifndef SDL_SURFACE_GRID_H
#define SDL_SURFACE_GRID_H

#include <sstream>
#include <string>
#include <vector>

#include <SDL/SDL.h>

#include <SDLInterface/SDLException.h>
#include <SDLInterface/SDLInterfaceLibraryException.h>
#include <Helpers/GeometricStructures2D.h>
#include <Helpers/Grid.h>

using namespace std;
using namespace Helpers;

namespace SDLInterfaceLibrary
{
	/*
		Enum: SDLSurfacePoint

		Defines a point on an SDL_Surface.

		SDL_SSP_UPPER_LEFT_CORNER - Upper left corner of the surface.
		SDL_SSP_LOWER_LEFT_CORNER - Lower left corner of the surface.
		SDL_SSP_UPPER_RIGHT_CORNER - Upper right corner of the surface.
		SDL_SSP_LOWER_RIGHT_CORNER - Lower left corner of the surface.
		SDL_SSP_CENTER - The midpoint of the surface.


	*/
	enum SDLSurfacePoint
	{
		SDL_SSP_UPPER_LEFT_CORNER,
		SDL_SSP_LOWER_LEFT_CORNER,
		SDL_SSP_UPPER_RIGHT_CORNER,
		SDL_SSP_LOWER_RIGHT_CORNER,
		SDL_SSP_CENTER
	};
	/*
		Class: SDLSurfaceGrid

		A surface grid is a rectangular array of SDL_Surface pointers. This class
		specifies a series of methods to assert, draw, and manage this array.
	*/
	class SDLSurfaceGrid
	{
		private:
			string name;

			//The size of the surfaces.
			Bounds2D<int> surfaceSize;

			Grid<SDL_Surface*>* surfaceGrid;

			//The position (In pixels) from where the surfaces are drawn.
			Vector2D<int> anchor;
			//The position in the array surfaceGrid from where the surfaces are drawn.
			Vector2D<int> anchorBlockPosition;
			//As the anchorBlockPosition Vector points to a cell in the array, it is
			//neccesary to specify which corner of that cell determines the anchor in
			//pixels.
			SDLSurfacePoint anchorPoint;

			//Asserts that the surfaces in the surfaceList all have a uniform size, and that none
			//of them are null.
			void ConfirmSurfaceList(const vector<SDL_Surface*>& surfaceList);
			//Asserts that the given position actually exists in the vector surfaceGrids.
			void ConfirmBlockPosition(const Vector2D<int>& blockPosition) const;
			//Asserts that a given surface has a size that conforms to the one specified in surfaceSize
			void ConfirmSurfaceSize(SDL_Surface* surface);
		public:
			/*
				Constructor: SDLSurfaceGrid

				Parameters:
					name - The name of the surface grid.

					surfaceGridSize - The size of the surface grid.

					surfaceSize - The size of the surfaces within the surface grid.

					anchorBlockPosition - The 2D position of the cell which will determine the anchor.

					anchorPoint - The point of the internal surfaces which detemine the anchor in pixels.

				See Also:
					<SDLSurfaceGrid::SetAnchor>
			*/
			SDLSurfaceGrid(const string& name, const Bounds2D<int>& surfaceGridSize, const Bounds2D<int>& surfaceSize,
				const Vector2D<int>& anchorBlockPosition, SDLSurfacePoint anchorPoint);
			/*
				Constructor: SDLSurfaceGrid

				Parameters:
					name - The name of the surface grid.

					surfacePlacementMap - A 2D array of unsigned integers which specifies the way the surfaces in
										  this grid will be arranged. Each cell should contain either a 0
										  (Which means "No surface here") or the index of the image in
										  the array surfaceList, +1. That is, if a cell contains the number
										  "1", then the surface at index 0 is retrieved from the array surfaceList,
										  and placed at that position inside the grid.

					surfaceList - A list of surfaces which will be used to fill the surface grid. See above.

					anchorBlockPosition - The 2D position of the cell which will determine the anchor.

					anchorPoint - The point on the cell which detemines the anchor in pixels.

				See Also:
					<SDLSurfaceGrid::SetAnchor>
			*/
			SDLSurfaceGrid(const string& name, const Grid<unsigned int>& surfacePlacementMap,
				const vector<SDL_Surface*>& surfaceList, const Vector2D<int>& anchorBlockPosition,
				SDLSurfacePoint anchorPoint);
			/*
				Function: Blit

				Blits all the surfaces to the screen, at the given position.

				Parameters:

					position - The position from where all the surfaces will be blitted.
							   Please note that this will be adjusted by the anchor.

					screen - The screen where the surfaces will be blitted.
			*/
			void Blit(const Vector2D<int>& position, SDL_Surface* screen);
			/*
				Function: Replace

				Replaces the surface in the cell at position [blockPosition] with [surface].

				Parameters:

					blockPosition - The position of the cell where [surface] will be placed.

					surface - The surface which will be placed in the cell at position [blockPosition]
			*/
			void Replace(SDL_Surface* surface, const Vector2D<int>& blockPosition);
			/*
				Function: Replace

				Replaces the surfaces in the surface grid from position [positionToCopyTo] with the
				surfaces inside [surfaceGrid], or a portion of it [portionToCopy] if one is specified.
				Also, if nullOverwrite is true, then any NULL surfaces in [surfaceGrid] will replace
				any filled surfaces in this surface grid.

				Parameters:

					surfaceGrid - The surface grid which will be used to replace the surfaces in this
								  grid.

					positionToCopyTo - The position where [surfaceGrid] will be copied. Please note that this
									   will be adjusted by the anchor.

					nullOverwrite - True if NULL surfaces should overwrite valid surfaces.
									Default: false.

					portionToCopy - If not NULL, the portion of [surfaceGrid] which should be copied.
									Default: NULL.

				See Also:
					<SDLSurfaceGrid::SetAnchor>
			*/
			void Replace(const SDLSurfaceGrid& surfaceGrid, Vector2D<int> positionToCopyTo,
				bool nullOverwrite = false, Dimensions2D<int>* portionToCopy = NULL);
			/*
				Function: Copy

				Parameters:
					portionToCopy - If not NULL, the portion which will be copied from this surface grid.
									Default Value: NULL.
				Returns:
					A copy of this surface grid.
			*/
			SDLSurfaceGrid Copy(Dimensions2D<int>* portionToCopy = NULL) const;
			/*
				Function: ClearSurfaceGrid

				Sets all the surfaces in this grid to NULL.

				Parameters:
					portionToClear - If not NULL, the portion which will be copied from this surface grid.
									 Default Value: NULL.
			*/
			void ClearSurfaceGrid(Dimensions2D<int>* portionToClear = NULL);
			/*
				Function: GetSurface

				Parameters:
					position - The position of the surface you wish to retrieve.

				Returns:
					The surface at the given position [position]
			*/
			SDL_Surface* GetSurface(const Vector2D<int>& position) const;
			/*
				Function: GetWidth

				Returns: The number of columns in the surface grid.
			*/
			int GetWidth() const;
			/*
				Function: GetHeight

				Returns: The number of rows in the surface grid.
			*/
			int GetHeight() const;
			/*
				Function: GetAnchor

				Returns: The pixel anchor position.

				See Also:
					<SDLSurfaceGrid::SetAnchor>
			*/
			const Vector2D<int>& GetAnchor() const;
			/*
				Function: GetAnchorBlockPosition

				Returns: The position of the cell where the anchor is located.

				See Also:
					<SDLSurfaceGrid::SetAnchor>
			*/
			const Vector2D<int>& GetAnchorBlockPosition() const;
			/*
				Function: GetSurfaceSize

				Returns: The size of the surfaces inside the grid.
			*/
			const Bounds2D<int>& GetSurfaceSize() const;
			/*
				Function: SetAnchor

				The anchor is the central position from which the surfaces inside the grid
				are drawn. In the case of images this is usually the origin of the image
				(i.e Coordinate (0, 0) on the image), however when dealing with interactive
				objects it is usually neccesary to specify another point in order to tie
				animations.

				For example take the following two surface grids (Where 1 is a surface, and 0 is NULL):

				10  111
				11  010
				10

				In order to tie these two surface grids as two frames of the same animation, it is neccesary
				to specify a common point which they share. If we keep coordinate (0,0) as the anchor, the
				animation would look unnatural. So we specify the anchor like so (Where x is the cell
				which contains the anchor):


				10  1x1
				x1  010
				10

				In order to convert this anchor cell into a point on the grid in pixels (For blitting purposes).
				We also need to specify the corner of the surface which will be anchored. This is helpful when
				the actual anchor is not the upper left corner of the surface, but another position.

				This function sets the anchor for this grid, using the location of the anchor cell [anchorBlockPosition]
				and the point on that cell which will be used to generate the anchor in pixels [anchorPoint].

				Parameters:
					anchorBlockPosition - The 2D position of the cell which will determine the anchor.

					anchorPoint - The point on the cell which detemines the anchor in pixels.


			*/
			void SetAnchor(const Vector2D<int>& anchorBlockPosition, SDLSurfacePoint anchorPoint);

	};
}

#endif
