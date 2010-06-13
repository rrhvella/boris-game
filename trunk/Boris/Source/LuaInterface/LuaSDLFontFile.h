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

#ifndef LUA_SDL_FONT_FILE_H
#define LUA_SDL_FONT_FILE_H

#include <Lua/lua.hpp>

#include <SDLInterface/SDLFontFile.h>
#include <Helpers/LuaHelperFunctions.h>

using namespace SDLInterfaceLibrary;
using namespace Helpers;

int SDLFontFile_New(lua_State* luaVM)
{
	const char* fontFilePath = luaL_checkstring(luaVM, 1);
	int textSize = luaL_checkint(luaVM, 2);

	void* fontFileInstance = CreateLuaInstanceBasedOnClass<SDLFontFile>(luaVM);

	new(fontFileInstance) SDLFontFile(fontFilePath, textSize);

	return 1;
}

int SDLFontFile_DestroyInstance(lua_State* luaVM)
{
	SDLFontFile* fontFileInstance = RetrieveCPPObject<SDLFontFile>(luaVM, 1);
	fontFileInstance->~SDLFontFile();

	return 0;
}

struct luaL_Reg SDLFontFileMetaTable [] =
{
	{"New", SDLFontFile_New},
	{NULL, NULL}
};


struct luaL_Reg SDLFontFileInstance [] =
{
	{"__gc", SDLFontFile_DestroyInstance},
	{NULL, NULL}
};



void RegisterSDLFontFileLibrary(lua_State* luaVM)
{
	CreateMetaTableBasedOnClass<SDLFontFile>(luaVM, SDLFontFileInstance);
    luaL_openlib(luaVM, "SDLFontFile", SDLFontFileMetaTable, 0);
}


#endif
