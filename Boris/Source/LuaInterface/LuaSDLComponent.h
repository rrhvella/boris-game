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

#ifndef LUA_SDL_COMPONENT_H
#define LUA_SDL_COMPONENT_H

#include <iostream>

#include <Lua/lua.hpp>

#include <ResourcePipelineSingleton.h>
#include <SDLInterface/SDLInstance.h>
#include <Helpers/LuaHelperFunctions.h>

using namespace std;
using namespace SDLInterfaceLibrary;
using namespace Helpers;

int SDLComponent_New(lua_State* luaVM)
{
	string componentName = luaL_checkstring(luaVM, 1);

	Vector2D<int> position(luaL_checkint(luaVM, 2), luaL_checkint(luaVM, 3));

	string trunkName = luaL_checkstring(luaVM, 4);
	string imageName = luaL_checkstring(luaVM, 5);

	void* componentInstance = CreateLuaInstanceBasedOnClass<SDLComponent>(luaVM);

	new(componentInstance) SDLComponent(componentName, position, 
								ResourcePipelineSingleton::GetInstance()
									.GetImage(trunkName, imageName));

	return 1;
}

int SDLComponent_DestroyInstance(lua_State* luaVM)
{
	SDLComponent* sdlComponentInstance = RetrieveCPPObject<SDLComponent>(luaVM, 1);
	sdlComponentInstance->~SDLComponent();

	return 0;
}


struct luaL_Reg SDLComponentMetaTable [] =
{
	{"New", SDLComponent_New},
	{NULL, NULL}
};


struct luaL_Reg SDLComponentInstance [] =
{
	{"__gc", SDLComponent_DestroyInstance},
	{NULL, NULL}
};


void RegisterSDLComponentLibrary(lua_State* luaVM)
{
	CreateMetaTableBasedOnClass<SDLComponent>(luaVM, SDLComponentInstance);
    luaL_openlib(luaVM, "SDLComponent", SDLComponentMetaTable, 0);
}
#endif
