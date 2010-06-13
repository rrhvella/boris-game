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

#ifndef I_KEY_EVENT_COMPONENT_H
#define I_KEY_EVENT_COMPONENT_H

#include <EventHandling/KeyEventHandler.h>

namespace EventHandling
{
	/*
		Class: KeyEventComponent

		The KeyEventComponent is a base for all classes which can have both a Key Up, and a Key Down
		event. It implements the functions, and accessors required to interact with the internal event
		handlers.

	*/
	class KeyEventComponent
	{
		protected:
			KeyEventHandlerCollection keyUpHandlers;
			KeyEventHandlerCollection keyDownHandlers;

		public:

			/*
				Function: KeyUp

				Fires a key up event, and calls all the key up event handlers.

				Parameters:
					keySymbol - The key which has been unpressed.
			*/
			void KeyUp(int keySymbol)
			{
				keyUpHandlers.RaiseEvents(keySymbol);
			}

			/*
				Function: KeyDown

				Fires a key down event, and calls all the KeyDown event handlers.

				Parameters:
					keySymbol - The key which has been pressed.
			*/
			void KeyDown(int keySymbol)
			{
				keyDownHandlers.RaiseEvents(keySymbol);
			}
			/*
				Function: GetKeyDownHandlers

				Returns:
					A read/write reference to the key down event handler collection.

				See Also:
					<KeyEventHandlerCollection>
			*/
			KeyEventHandlerCollection& GetKeyDownHandlers()
			{
				return keyDownHandlers;
			}

			/*
				Function: GetKeyUpHandlers

				Returns:
					A read/write reference to the key up event handler collection.

				See Also:
					<KeyEventHandlerCollection>
			*/
			KeyEventHandlerCollection& GetKeyUpHandlers()
			{
				return keyUpHandlers;
			}

	};
}
#endif
