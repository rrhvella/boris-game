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

#ifndef SDL_FORM_H
#define SDL_FORM_H

#include <algorithm>

#include <SDL/SDL.h>

#include <Helpers/GeometricStructures2D.h>
#include <SDLInterface/SDLComponent.h>
#include <EventHandling/KeyEventComponent.h>
#include <SDLInterface/SDLTextBox.h>
#include <SDLInterface/SDLTimer.h>

using namespace EventHandling;

namespace SDLInterfaceLibrary
{
	class SDLInstance;

	/*
		Class: SDLForm

		Object derived from SDLForm are the primary containers of any other SDLComponent objects.
		They define the neccesary functions to interect with their children as a whole. They also
		provide a layer to interact with the SDLInstance, and the primary drawing screen, as only
		objects of type SDLForm can be focused on by the instance.

		SDLForm objects are also the only containers for SDLTimers, meaning that once the main SDLInstance
		removes it's focus from an SDLForm, that form's timers will automatically pause themselves.

		See Also:
			<SDLInstance>
			<SDLInstance::SetFocus>
	*/
	class SDLForm: public SDLComponent, public KeyEventComponent
	{
		private:
			//The children currently contained within this form.
			vector<SDLComponent*> children;
			//The timers currently active on this form.
			vector<SDLTimer*> timers;

			//The keyevent component to which key events will be delegated to.
			KeyEventComponent* componentWithFocus;

		public:
			/*
				Constructor: SDLForm

				Parameters:
					name - The name of the SDLForm--This is used to identify which
							form has failed if an exception occurs.

					position - The position of the form on the screen.

					image - A pointer to the image which will be used for the form's background.
			*/
			SDLForm(const string& name, const Vector2D<int>& position, SDL_Surface* image);
			/*
				Function: AddChild

				Adds an SDLComponent to the Child list.

				Parameters:
					child - The child to add.
			*/
			void AddChild(SDLComponent* child);
			/*

				Function: RemoveChild

				Removes an SDLComponent from the Child list.

				Parameters:
					child - The child to remove.

			*/
			void RemoveChild(SDLComponent* child);
			/*
				Function: AddTimer

				Adds an SDLTimer to the timer list.

				Parameters:
					timer - The timer to add.

			*/
			void AddTimer(SDLTimer& timer);
			/*
				Function: KeyUp

				Fires a key up event, and delegates it to another key event component if one is
				specified.

				Parameters:
					keySymbol - The key which has been unpressed.

				See Also:
					<SDLForm::SetFocus>
			*/
			void KeyUp(int keySymbol);
			/*
				Function: KeyDown

				Fires a key down event, and delegates it to another key event component if one is
				specified.

				Parameters:
					keySymbol - The key which has been pressed.

				See Also:
					<SDLForm::SetFocus>
			*/
			void KeyDown(int keySymbol);
			/*
				Function: Draw

				Initiates the Draw cycle for this form and it's children.

				Note:
					This method does not need to be called, as it is automatically
					called by the SDLInstance when neccessary.

				See Also:
					<SDLInstance>
					<SDLComponent::Draw>

			*/
			void Draw();
			/*
				Function: UpdateTimers

				Updates the timers attached to this form.

				Parameters:
					frameRate - The application's current frame rate.

			*/
			void UpdateTimers(int frameRate);
			/*
				Function: SetFocus

				Parameters:
					keyComponent - The key event component to which key events fired on this form
								   will be delegated.

			*/
			void SetFocus(KeyEventComponent* keyComponent);
			SDL_Surface* GetScreen() const;
			/*
				Function: Update

				Informs this component, and all it's children, that it needs to redraw itself next
				draw cycle.

				See Also:
					<SDLComponent:Update>

			*/
			void Update();
			/*
				Destructor: SDLForm

				Destroys the SDLForm's instance and detaches all it's children from it.
			*/
			~SDLForm();

			//Required for SDLInstance to set a form's screen in SDLInstance::SetFocus.
			friend class SDLInstance;
	};
}

#endif
