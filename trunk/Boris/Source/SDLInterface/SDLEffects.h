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

#ifndef SDL_EFFECTS_H
#define SDL_EFFECTS_H

#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>

#include <Helpers/GeometricStructures2D.h>

using namespace Helpers;

/*
File: SDLEffects.h

Contains the declarations of ISDLEffectInfo, ISDLEffect, and their children.
*/

namespace SDLInterfaceLibrary
{
	class SDLComponent;

	/*
		Class: ISDLEffectInfo

		This is an empty interface which is inherited by all effect info classes. These
		classes contain information relevent to the effect and the current component the
		info is stored on. This allows the effect to be seperate from the component, while
		also allowing it to reference information particular to a component.

		Example:
			- StarEffect is an effect which animates a star orbitting around a component.

            - StarEffect is initialized with the star's color.

			- When applied to a component, StarEffect stores the current position of the
			  star in the component's currentEffectInfo field.

		    - Every time it's called by the component, StarEffect will animate it's star
			  image using the previous information stored, as well as the image of the
			  component calling it.

			- After drawing the current frame, StarEffect will store the position of
			  the star in the component, so that it can be used next time it is called.

	    That way the same StarEffect object can be used on multiple components.

		See Also:
			<ISDLEffect>
			<SDLComponent::GetEffectInfo()>

	*/
	struct ISDLEffectInfo
	{
	};

	/*
		Class: ISDLEffect

		This is an interface for all effects which can be applied to a component. All
		classes which inherit this interface must implement the pure functions GetEffectInfo
		and GetEffectImage. Effects are seperate from the component on which they are implemented.
		And differ from screen effects, in that screen effects take a snapshot of the current screen
		and use it to create and display an animation there and then. On the other hand, component
		effects draw themselves on the screen whenever they are called by a component, and use
		the class ISDLEffectInfo to store information pertinent to that component.

		See Also:
			<ISDLEffectInfo>

	*/
	class ISDLEffect
	{
		public:
			/*
				Function: GetEffectInfo

				Returns:
					The initial effectInfo instance which the component stores.

				See Also:
					<ISDLEffectInfo>

			*/
			virtual ISDLEffectInfo* GetEffectInfo() const = 0;
			/*
				Function: GetEffectImage

				Parameters:
					target - The component on which this effect will be performed.
					effectOffsetX - This is a referential pass, on which the x offset caused
									by the effect is stored.

					effectOffsetY - This is a referential pass, on which the y offset caused
									by the effect is stored.

					Example:

						If an effect increases the size of the target's image, then
						it will have effectively moved the position where the target
						should be blitted. Ergo, the blit function must be informed
						of this offset.


				See Also:

					<SDLComponent:Blit>

			*/
			virtual SDL_Surface* GetEffectImage(SDLComponent* target, int& effectOffsetX, int& effectOffsetY) const = 0;

			virtual ~ISDLEffect()
			{
			}
	};


	/*
		Class: LoomEffectInfo

		Effect info for the loom component effect.

		See Also:
			<ISDLEffectInfo>

	*/
	struct LoomEffectInfo: public ISDLEffectInfo
	{
		/*
			Properties:

			increasing - True if the image is increasing.
			currentSize - The current scale multiplier.
		*/
		bool increasing;
		double currentSize;

		/*
			Constructor: LoomEffectInfo
		*/
		LoomEffectInfo();
	};

	/*
		Class: LoomEffect

		This effect causes the component on which it is applied to scale in size
		at a given rate, until it reaches a given maximum scale.

		See Also:
			<ISDLEffect>

	*/
	class LoomEffect: public ISDLEffect
	{
		private:
			double rateOfIncrease;
			double maxSize;

		public:
			/*
				Constructor: LoomEffect

				Parameters:
					rateOfIncrease - The rate at which the component's scale increases/decreases.

					maxSize - The maximum scale which the component can reach before it begins lowering
							  it's scale back down to normal.
			*/
			LoomEffect(double rateOfIncrease, double maxSize);

			ISDLEffectInfo* GetEffectInfo() const;
			SDL_Surface* GetEffectImage(SDLComponent* target, int& effectOffsetX, int& effectOffsetY) const;
	};
}

#endif
