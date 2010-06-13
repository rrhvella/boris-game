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

#ifndef LUA_SDL_TIMER_H
#define LUA_SDL_TIMER_H

#include <SDLInterface/SDLTimer.h>
#include <Helpers/LuaHelperFunctions.h>
#include <EventHandling/GenericEventHandler.h>

using namespace SDLInterfaceLibrary;
using namespace Helpers;

int SDLTimer_New(lua_State* luaVM)
{
	int interval = luaL_checkint(luaVM, 1);
	void* timer = CreateLuaInstanceBasedOnClass<SDLTimer>(luaVM);

	new(timer) SDLTimer(interval);

	return 1;
}

int SDLTimer_AddCycleEventHandler(lua_State* luaVM)
{
	SDLTimer* sdlTimerInstance = RetrieveCPPObject<SDLTimer>(luaVM, 1);

	lua_remove(luaVM, 1);
	sdlTimerInstance->GetCycleCompleteEventHandlers().AddLuaEventHandler(luaVM);

	return 0;
}

int SDLTimer_SetInterval(lua_State* luaVM)
{
	SDLTimer* sdlTimerInstance = RetrieveCPPObject<SDLTimer>(luaVM, 1);
	int interval = luaL_checkint(luaVM, 2);

	sdlTimerInstance->SetInterval(interval);

	return 0;
}

int SDLTimer_Pause(lua_State* luaVM)
{
	SDLTimer* sdlTimerInstance = RetrieveCPPObject<SDLTimer>(luaVM, 1);
	sdlTimerInstance->Pause();

	return 0;
}

int SDLTimer_Continue(lua_State* luaVM)
{
	SDLTimer* sdlTimerInstance = RetrieveCPPObject<SDLTimer>(luaVM, 1);
	sdlTimerInstance->Continue();

	return 0;
}

struct luaL_Reg SDLTimerMetaTable [] =
{
	{"New", SDLTimer_New},
	{NULL, NULL}
};

struct luaL_Reg SDLTimerInstance [] =
{
	{"AddCycleCompleteHandler", SDLTimer_AddCycleEventHandler},
	{"SetInterval", SDLTimer_SetInterval},
	{"Pause", SDLTimer_Pause},
	{"Continue", SDLTimer_Continue},
	{NULL, NULL}
};

void RegisterSDLTimerLibrary(lua_State* luaVM)
{
	CreateMetaTableBasedOnClass<SDLTimer>(luaVM, SDLTimerInstance);
    luaL_openlib(luaVM, "SDLTimer", SDLTimerMetaTable, 0);
}


#endif
