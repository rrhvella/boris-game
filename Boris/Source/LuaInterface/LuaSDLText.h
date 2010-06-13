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

#ifndef LUA_SDL_TEXT_H
#define LUA_SDL_TEXT_H

#include <string>
#include <vector>

#include <Lua/lua.hpp>

#include <Helpers/LuaHelperFunctions.h>
#include <SDLInterface/SDLText.h>
#include <SDLInterface/SDLEffects.h>
#include <SDLInterface/SDLFontFile.h>

using namespace std;
using namespace SDLInterfaceLibrary;
using namespace Helpers;

int SDLText_New(lua_State* luaVM)
{
	string name = luaL_checkstring(luaVM, 1);
	Vector2D<int> position = Vector2D<int>(luaL_checkint(luaVM, 2), luaL_checkint(luaVM, 3));
	const char* text = luaL_checkstring(luaVM, 4);
	SDLFontFile* fontFile = (SDLFontFile*)luaL_checkudata(luaVM, 5, typeid(SDLFontFile).name());

	if(!lua_istable(luaVM, 6))
	{
		luaL_argerror(luaVM, 6, "Expected table");
	}

	lua_pushvalue(luaVM, 6);

	SDL_Color textColour = {GetIntField(luaVM, "r"), GetIntField(luaVM, "g"), GetIntField(luaVM, "b")};

	void* sdlTextInstance = CreateLuaInstanceBasedOnClass<SDLText>(luaVM);

	new(sdlTextInstance) SDLText(name, position, text, fontFile, textColour);

	return 1;
}

int SDLText_GetText(lua_State* luaVM)
{
	SDLText* sdlTextInstance = RetrieveCPPObject<SDLText>(luaVM, 1);

	const string& text = sdlTextInstance->GetText();
	lua_pushstring(luaVM, text.c_str());

	return 1;
}

int SDLText_SetText(lua_State* luaVM)
{
	SDLText* sdlTextInstance = RetrieveCPPObject<SDLText>(luaVM, 1);
	string text = luaL_checkstring(luaVM, 2);

	sdlTextInstance->SetText(text);

	return 0;
}

int SDLText_SetEffect(lua_State* luaVM)
{
	SDLText* sdlTextInstance = RetrieveCPPObject<SDLText>(luaVM, 1);
	LoomEffect* loomEffectInstance = NULL;

	if(!lua_isnil(luaVM, 2))
	{
		loomEffectInstance = (LoomEffect*)luaL_checkudata(luaVM, 2, typeid(LoomEffect).name());
	}

	sdlTextInstance->SetEffect(loomEffectInstance);

	return 0;
}

int SDLText_DestroyInstance(lua_State* luaVM)
{
	SDLText* sdlTextInstance = RetrieveCPPObject<SDLText>(luaVM, 1);
	sdlTextInstance->~SDLText();

	return 0;
}


struct luaL_Reg SDLTextMetaTable [] =
{
	{"New", SDLText_New},
	{NULL, NULL}
};


struct luaL_Reg SDLTextInstance [] =
{
	{"GetText", SDLText_GetText},
	{"SetText", SDLText_SetText},
	{"SetEffect", SDLText_SetEffect},
	{"__gc", SDLText_DestroyInstance},
	{NULL, NULL}
};

void RegisterSDLTextLibrary(lua_State* luaVM)
{
	CreateMetaTableBasedOnClass<SDLText>(luaVM, SDLTextInstance);
    luaL_openlib(luaVM, "SDLText", SDLTextMetaTable, 0);
}


#endif
