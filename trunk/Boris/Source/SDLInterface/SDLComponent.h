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

#ifndef SDL_COMPONENT_H
#define SDL_COMPONENT_H

#include <sstream>
#include <exception>

#include <SDL/SDL.h>

#include <SDLInterface/SDLEffects.h>
#include <EventHandling/GenericEventHandler.h>
#include <Helpers/GeometricStructures2D.h>
#include <SDLInterface/SDLException.h>
#include <SDLInterface/SDLInterfaceLibraryException.h>

using std::stringstream;

using namespace EventHandling;

namespace SDLInterfaceLibrary
{
	class SDLForm;

	/*
		Class: SDLComponent

		This is the base for all classes which define an SDLComponent, that is a control
		which interacts with the SDLInstance, and with SDL at a lower level. At their
		lowest possible definition, these classes define a surface, a screen, their
		position on the screen, as well as a parent, and their position on this parent.

		See Also:
			<SDLInstance>

	*/
	class SDLComponent
	{
		protected:
			//The following variables specify where the component was last drawn.
			//They are used in the Clear function to remove the component from that position
			//before Blitting it to the new one.

			Dimensions2D<int> previousDimensions;
			Vector2D<int> previousOffsetToOrigin;

			string name;

			Vector2D<int> position;

			SDL_Surface* screen;
			SDLForm* parent;

			//The current effect which is drawn in the Blit function, as well as the info it
			//is associating with this component.
			const ISDLEffect* currentEffect;
			ISDLEffectInfo* currentEffectInfo;

			//Specifies wheater or not a Draw cycle should occur.
			bool updated;

			//Specifies wheater or not a Clear should occur during the draw cycle.
			bool requiresClear;

			//Event handlers which are called before the draw cycle begins.
			GenericEventHandlerCollection drawBeginHandlers;
			//Event handlers which are called after the draw cycle finishes.
			GenericEventHandlerCollection drawFinishHandlers;

			SDL_Surface* image;
			/*
				Function: Blit

				Blits the image on the screen according to the component's position.
			*/
			virtual void Blit();
			/*
				Function: Clear

				Blits the image of the parent on the screen according to the component's previous position
				and size.
			*/
			virtual void Clear();
			/*
				Function: ConfirmPositionInParent

				Asserts that this component is still within the bounds of it's parent, and throws
				an error if it isn't.
			*/
			void ConfirmPositionInParent();

		public:

			/*
				Constructor: SDLComponent

				Parameters:
					name - The name of the SDLComponent--This is used to identify which
							component has failed if an exception occurs.

					position - The position of the Component on the parent, not the screen.

					image - A pointer to the image which the component will display.

			*/
			SDLComponent(const string& name, const Vector2D<int>& position, SDL_Surface* image);
			/*
				Function: GetWidth

				Returns:
					The width of the current image.
			*/
			virtual int GetWidth() const;
			/*
				Function: GetHeight

				Returns:
					The height of the current image.
			*/
			virtual int GetHeight() const;
			/*
				Function: GetLeftFromOrigin

				Returns:
					The x offset from the origin of the screen.
			*/
			int GetLeftFromOrigin() const;
			/*
				Function: GetTopFromOrigin

				Returns:
					The y offset from the origin of the screen.
			*/
			int GetTopFromOrigin() const;
			/*
				Function: GetLeftFromParent

				Returns:
					The x offset from the origin of the parent.
			*/
			int GetLeftFromParent() const;
			/*
				Function: GetTopFromParent

				Returns:
					The y offset from the origin of the parent.
			*/
			int GetTopFromParent() const;
			/*
				Function: GetName

				Returns:
					The name of the component.
			*/
			const string& GetName() const;
			/*
				Function: GetImage

				Returns:
					The current image.
			*/
			virtual SDL_Surface* const GetImage() const;
			/*
				Function: GetParent

				Returns:
					The SDLForm which this component is linked to.

				See Also:
					<SDLForm>
			*/
			virtual SDLForm* GetParent() const;
			/*
				Function: GetScreen

				Returns:
					The screen on which this component will be drawn.
			*/
			virtual SDL_Surface* GetScreen() const;
			/*
				Function: GetDrawBeginHandlers

				Returns:
					A read/write collection of generic event handlers, which
					would then be called prior to the draw cycle.
			*/
			GenericEventHandlerCollection& GetDrawBeginHandlers();
			/*
				Function: GetDrawFinishHandlers

				Returns:
					A read/write collection of generic event handlers, which
					would then be called after a draw cycle finishes.
			*/
			GenericEventHandlerCollection& GetDrawFinishHandlers();
			/*
				Function: GetEffectInfo

				Returns:
					A pointer to the info currently being stored by the
					effect active on this component.

				See Also:
					<ISDLEffectInfo>
					<ISDLEffect>
			*/
			ISDLEffectInfo* GetEffectInfo() const;
			/*
				Function: SetParent

				Parameters:
					parent - The new form which this component will be tied to.

				Note:
					Ideally, this method should not be called manually, as it
					is called automatically when this component is added to a
					form.

				See Also:
					<SDLForm>
			*/
			virtual void SetParent(SDLForm* parent);
			/*
				Function: SetEffect

				Parameters:
					effect - The new effect which will be applied to this component.

				Note:

					Ideally, this method wouldn't be called manually by the user, as it
					is called automatically by the instance this component is added to.


				See Also:
					<ISDLEffect>
			*/
			void SetEffect(const ISDLEffect* effect) ;
			/*
				Function: AddToLeft

				Parameters:
					increment - The increment which will be applied to the x
								offset of this component.
			*/
			virtual void AddToLeft(int increment);
			/*
				Function: AddToTop

				Parameters:
					increment - The increment which will be applied to the y
								offset of this component.
			*/
			virtual void AddToTop(int increment);
			/*
				Function: SetLeft

				Parameters:
					newLeft - The new x offset of this component from it's parent's origin.

				See Also:
					<SDLComponent::GetLeftFromParent>
			*/
			virtual void SetLeft(int newLeft);
			/*
				Function: newTop

				Parameters:
					newLeft - The new y offset of this component from it's parent's origin.

				See Also:
					<SDLComponent::GetTopFromParent>
			*/
			virtual void SetTop(int newTop);
			/*
				Function: Draw

				Initiates a draw cycle on this component.

				Note:

					Ideally, this method wouldn't be called manually by the user, as it
					is called automatically by the instance this component is added to.

				Also Note:

					A draw cycle will not actually occur, unless this component is
					notified that it needs to do one, using the Update method.

				See Also:

					<SDLComponent::Update>
					<SDLForm>
					<SDLInstance>
			*/
			virtual void Draw();
			/*
				Function: Update

				Informs this component that it needs to redraw itself next
				draw cycle.

				Note:

					In general, you don't need to call this, as all the functions
					which cause a component to either move or in any other way change shape,
					already inform it that it must redraw itself.

					However if you create a child of this class, or any of it's children,
					and you create a method which changes the position or shape of the image,
					then you must call this method.
			*/
			virtual void Update();
			/*
				Destructor: SDLComponent

				Removes any effect information associated with this control from memory,
				and informs the parent form -if there is one- to remove this component
				from the child list.

				See Also:
					<ISDLEffectInfo>
					<ISDLEffect>
					<SDLForm>
			*/
			virtual ~SDLComponent();

	};
}

#endif
