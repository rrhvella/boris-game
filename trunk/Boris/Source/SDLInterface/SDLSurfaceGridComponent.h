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

#ifndef SDL_SURFACE_GRID_COMPONENT_H
#define SDL_SURFACE_GRID_COMPONENT_H

#include <vector>
#include <sstream>

#include <SDLInterface/SDLSurfaceGrid.h>
#include <SDLInterface/SDLComponent.h>
#include <Helpers/GeometricStructures2D.h>

using namespace std;

namespace SDLInterfaceLibrary
{
	/*
		Enum: SDLClearingMethod

		Defines the method which will be used to clear a surface grid component from the screen.

		SDL_CM_PRECISE - The component will be cleared precisely, so that any only filled surfaces
						 will be cleared. This is useful for complex shapes.

		SDL_CM_BOUNDING_BOX - The component will be cleared using an area which covers all it's
							  rows and columns. This method is a lot more efficient, but will include
							  undesirable effects for complex shapes.

	*/
	enum SDLClearingMethod
	{
		SDL_CM_PRECISE,
		SDL_CM_BOUNDING_BOX
	};
	/*
		Class: SDLSurfaceGridComponent

		A surface grid component, unlike a normal component, uses an sdl surface grid instead of a
		single surface. That said, it is treated as any other component by this library, with the
		exception that it has more high-level commands for dealing with certain situations.

		This is a product of this library being created for a tetris game, rather than anything else.
		Also, please note that this class can have contain more than one surface grid, which allows it
		to be used to animate surface grids.

		See Also:
			<SDLComponent>
			<SDLSurfaceGrid>
	*/
	class SDLSurfaceGridComponent: public SDLComponent
	{
		private:
			//The surface grid which was blitted on the screen in the last drawing cycle.
			SDLSurfaceGrid* previouslyBlittedSurfaceGrid;

			vector<SDLSurfaceGrid> surfaceGrids;
			//The index of the surface grid currently displayed by this component.
			unsigned int currentSurfaceGridIndex;

			SDLClearingMethod clearingMethod;

		protected:
			/*
				Function: Blit

				Blits the current surface grid on the screen according to the component's position.
			*/
			void Blit();
			/*
				Function: Clear

				Clears the previous surface grid from the screen according to the component's
				previous position.
			*/
			void Clear();

		public:
			/*
				Constructor: SDLSurfaceGridComponent

				Parameters:
					name - The name of the component--This is used to identify which
							component has failed if an exception occurs.

					position - The position of the component on the parent, not the screen.

					surfaceGrids - A collection of surface grids which will be contained in this grid.

					clearingMethod - The method which will be used to clear this component from the screen.

				See Also:
					<SDLClearingMethod>
			*/
			SDLSurfaceGridComponent(const string& name, const Vector2D<int>& position,
					const vector<SDLSurfaceGrid>& surfaceGrids, SDLClearingMethod clearingMethod);

			/*
				Function: First

				Moves the current surface grid index to the first surface grid in the list.
			*/
			void First();
			/*
				Function: Next

				Moves the current surface grid index to the next surface grid in the list.
				If this function is at the end of the list, it will be move to the first
				surface grid.
			*/
			void Next();
			/*
				Function: Previous

				Moves the current surface grid index to the previous surface grid in the list.
				If this function is at the beginning of the list, it will be move to the last
				surface grid.
			*/
			void Previous();
			/*
				Function: GetCurrentSurfaceGrid

				Returns:
					A read/write reference to the surface grid this component is currently pointing to.
			*/
			SDLSurfaceGrid& GetCurrentSurfaceGrid();
			/*
				Function: GetCurrentSurfaceGrid

				Returns:
					A read reference to the surface grid this component is currently pointing to.
			*/
			const SDLSurfaceGrid& GetConstCurrentSurfaceGrid() const;
			/*
				Function: GetWidth

				Returns:
					The number of columns in the current surface grid.
			*/
			int	GetWidth() const;
			/*
				Function: GetHeight

				Returns:
					The number of rows in the current surface grid.
			*/
			int GetHeight() const;
			/*
				Function: ClearCurrentSurfaces

				Calls the method "ClearSurfaces" for the current surface grid. This method
				also informs the component that it must draw itself in the next cycle.

				See Also:
					<SDLSurfaceGrid::ClearSurfaces>
			*/
			void ClearCurrentSurfaces(Dimensions2D<int>* portionToClear);
			/*
				Function: ReplaceCurrentSurfaces

				Calls the method "Replace" for the current surface grid. This method
				also informs the component that it must draw itself in the next cycle.

				See Also:
					<SDLSurfaceGrid::Replace>
			*/
			void ReplaceCurrentSurfaces(const SDLSurfaceGrid& surfaceGrid, const Vector2D<int>& positionToCopyTo,
				bool nullOverwrite = false, Dimensions2D<int>* portionToCopy = NULL);
			/*
				Function: CopyCurrentSurfaces

				Calls the method "Copy" for the current surface grid. This method
				also informs the component that it must draw itself in the next cycle.

				See Also:
					<SDLSurfaceGrid::Copy>
			*/
			SDLSurfaceGrid CopyCurrentSurfaces(Dimensions2D<int>* portionToCopy = NULL);
			/*
				Function: SurfacesCollide

				Parameters:
					target - The surface grid component who's surfaces and position will be compared
							 to this surface grid component.

					treatOutsideBoundsAsCollision - If true and the target surface has a surface
													outside of the bounds of this surface, then
													this method returns true even if the surfaces
													don't actually collide.

													This is useful if you wish to keep a surface
													grid component within the bounds of this
													component without having the surfaces between
													them actually collide.

				Returns:
					True if the current surfaces in this component and those in [target] collide,
					it will also return true if one of the surfaces in target are outside the bounds
					of this component, and the flag treatOutsideBoundsAsCollision is set to true.

				Please Note:
					This collission detection method is not precise. I.E. It will not detect a collision
					unless the surfaces in this component and the target share the same exact size, and
					are perfectly alligned. This makes this method useful in certain instances such
					as checking wheater or not a tetromino in a tetris game has collided with the blocks
					in a tetris panel. This method is also extremely resource friendly for such a situation.
					Otherwhise it is not ideal.

				Also Note:
					The anchor for the current surfaces of both the target, and this component, influence
					the result of this method.

				See Also:
					<SDLSurfaceGrid::SetAnchor>


			*/
			bool SurfacesCollide(const SDLSurfaceGridComponent& target, bool treatOutsideBoundsAsCollision = false);
			/*
				Destructor: SDLSurfaceGridComponent

				This class keeps a copy of the previously drawn surface grid in memory (So that
				it can later be cleared), therefore it needs to be removed from memory when this
				component is destroyed.
			*/
			~SDLSurfaceGridComponent();
	};
}

#endif
