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

#include <EventHandling/LuaEventHandler.h>

using namespace EventHandling;

LuaEventHandler::LuaEventHandler(lua_State* luaVM)
{
	this->luaVM = luaVM;
	function = 0;
	functionContainer = 0;

	bool firstParameterIsContainer = false;

	if(!lua_istable(luaVM, 1) && !lua_isfunction(luaVM, 1))
	{
		luaL_error(luaVM, "Illegal registration, the first parameter should either be a lua table or a lua function");
	}

	firstParameterIsContainer = lua_istable(luaVM, 1);

	if(firstParameterIsContainer && !lua_isfunction(luaVM, 2))
	{
		luaL_error(luaVM, "Illegal registration, the second parameter should be a lua function");
	}

	//Collect the handler function reference from Lua.
	function = luaL_ref(luaVM, LUA_REGISTRYINDEX);

	if(firstParameterIsContainer)
	{
		//If the first parameter is a container. Collect it's reference from Lua.
		functionContainer = luaL_ref(luaVM, LUA_REGISTRYINDEX);
	}
}

LuaEventHandler::~LuaEventHandler()
{
	if(functionContainer)
	{
		luaL_unref(luaVM, LUA_REGISTRYINDEX, functionContainer);
	}

	luaL_unref(luaVM, LUA_REGISTRYINDEX, function);
}
