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

#ifndef LUA_EVENT_HANDLER_H
#define LUA_EVENT_HANDLER_H

#include <Helpers/IUncopyable.h>
#include <Lua/lua.hpp>

using namespace Helpers;

namespace EventHandling
{
	/*
		Class: LuaEventHandler

		This is the base class of all lua event handlers, irregardless of the kind
		of events they actually handle. It makes sure that both the lua function, which
		will handle the event, and it's container, are safely captured, stored, and destroyed.
	*/

	class LuaEventHandler: public IUncopyable
	{
		protected:
			lua_State* luaVM;
			int functionContainer;
			int function;

		public:
			/*
				Constructor: LuaEventHandler

				The LuaEventHandler virtual constructor accepts two overrides from lua.
				The first parameter can either be a global function, or an instance
				of a table which contains a function. If a table is passed, then the second
				parameter should be a function -within that table-. The event handler
				is not responsible for making sure that the function passed belongs to the
				table, as it has no way of doing so.

				If a mistake is made, then the lua-developer will be informed at virtual machine level,
				when the event is raised. The same will happen if a function without the proper signature
				is registered.
			*/
			LuaEventHandler(lua_State* luaVM);
			/*
				Destructor: LuaEventHandler

				Once the event handler is destroyed, it no longer needs to keep
				a reference to the lua function, and the function container (if it exists).
				Therefore they are both discarded, and lua is informed that it can safely
				destroy them.
			*/
			~LuaEventHandler();
	};
}
#endif

