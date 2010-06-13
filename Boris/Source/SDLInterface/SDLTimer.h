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

#ifndef SDL_TIMER_H
#define SDL_TIMER_H

#include <EventHandling/GenericEventHandler.h>

using namespace EventHandling;

namespace SDLInterfaceLibrary
{
	/*
		Class: SDLTimer

		Objects derived from this class will raise a generic event when
		an amount of time has elapsed. They are tied to SDLForms which
		control their internal counting process using the application's
		capped FPS.

		See Also:
			<SDLForm>
	*/
	class SDLTimer
	{
		private:
			//The amount (in milliseconds) the current cycle has progressed.
			int currentCycleProgress;
			int interval;

			bool paused;

			GenericEventHandlerCollection cycleCompleteEventHandlers;
		public:
			/*
				Constructor: SDLTimer

				Parameters:
					interval - The interval which should pass before an event is raised.
			*/
			SDLTimer(int interval);
			/*
				Function: GetCycleCompleteEventHandlers

				Returns:
					A Read/Write collection of generic event handlers which will be called
					when an amount in milliseconds greater or equal to the interval
					has elapsed.
			*/
			GenericEventHandlerCollection& GetCycleCompleteEventHandlers();
			/*
				Function: SetInterval

				Parameters:
					interval - The interval which should pass before an event is raised.

				Note:
					The time progression is not reversed when this method is called.
					So if:
						- The previous interval was 1000.
						- The time elapsed is currently 500.
						- The new interval is 400.

					An cycle completion event is fired.
			*/
			void SetInterval(int interval);
			/*
				Function: Tick

				Progresses the timer's internal clock.

				Parameters:
					fps - The current application's frame cap.
			*/
			void Tick(int fps);
			/*
				Function: Pause

				Pauses the timer's internal clock.
			*/
			void Pause();
			/*
				Function: Continue

				Continues the timer's internal clock.
			*/
			void Continue();
	};
}

#endif
