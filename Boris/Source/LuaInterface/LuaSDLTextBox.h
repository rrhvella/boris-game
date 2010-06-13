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

#ifndef LUA_SDL_TEXT_BOX_H
#define LUA_SDL_TEXT_BOX_H

#include <string>
#include <vector>

#include <Lua/lua.hpp>

#include <Helpers/LuaHelperFunctions.h>
#include <SDLInterface/SDLTextBox.h>
#include <EventHandling/InputEventHandler.h>

using namespace std;
using namespace SDLInterfaceLibrary;
using namespace Helpers;

int SDLTextBox_New(lua_State* luaVM)
{

	string name = luaL_checkstring(luaVM, 1);
	Vector2D<int> position = Vector2D<int>(luaL_checkint(luaVM, 2), luaL_checkint(luaVM, 3));
	const char* text = luaL_checkstring(luaVM, 4);
	SDLFontFile* fontFile = (SDLFontFile*)luaL_checkudata(luaVM, 5, typeid(SDLFontFile).name());

	int maxLength = luaL_checkint(luaVM, 6);

	if(!lua_istable(luaVM, 7))
	{
		luaL_argerror(luaVM, 7, "Expected table");
	}

	lua_pushvalue(luaVM, 7);

	SDL_Color textColour = {GetIntField(luaVM, "r"), GetIntField(luaVM, "g"), GetIntField(luaVM, "b")};

	void* sdlTextBoxInstance = CreateLuaInstanceBasedOnClass<SDLTextBox>(luaVM);
	new(sdlTextBoxInstance) SDLTextBox(name, position, text, fontFile, maxLength, textColour);

	return 1;
}

int SDLTextBox_GetText(lua_State* luaVM)
{
	SDLTextBox* sdlTextBoxInstance = RetrieveCPPObject<SDLTextBox>(luaVM, 1);

	const string& text = sdlTextBoxInstance->GetText();
	lua_pushstring(luaVM, text.c_str());

	return 1;
}

int SDLTextBox_SetText(lua_State* luaVM)
{
	SDLTextBox* sdlTextBoxInstance = RetrieveCPPObject<SDLTextBox>(luaVM, 1);
	string text = luaL_checkstring(luaVM, 2);

	sdlTextBoxInstance->SetText(text);

	return 0;
}

int SDLTextBox_AddInputHandler(lua_State* luaVM)
{
	SDLTextBox* sdlTextBoxInstance = RetrieveCPPObject<SDLTextBox>(luaVM, 1);

	lua_remove(luaVM, 1);
	sdlTextBoxInstance->GetTextInputHandler().AddLuaEventHandler(luaVM);

	return 0;
}

int SDLTextBox_AddKeyDownHandler(lua_State* luaVM)
{
	SDLTextBox* sdlTextBoxInstance = RetrieveCPPObject<SDLTextBox>(luaVM, 1);

	lua_remove(luaVM, 1);
	sdlTextBoxInstance->GetKeyDownHandlers().AddLuaEventHandler(luaVM);

	return 0;
}

int SDLTextBox_DestroyInstance(lua_State* luaVM)
{
	SDLTextBox* sdlTextBoxInstance = RetrieveCPPObject<SDLTextBox>(luaVM, 1);
	sdlTextBoxInstance->~SDLTextBox();

	return 0;
}


struct luaL_Reg SDLTextBoxMetaTable [] =
{
	{"New", SDLTextBox_New},
	{NULL, NULL}
};


struct luaL_Reg SDLTextBoxInstance [] =
{
	{"GetText", SDLTextBox_GetText},
	{"SetText", SDLTextBox_SetText},
	{"AddInputHandler", SDLTextBox_AddInputHandler},
	{"AddKeyDownHandler", SDLTextBox_AddKeyDownHandler},
	{"__gc", SDLTextBox_DestroyInstance},
	{NULL, NULL}
};

void RegisterSDLTextBoxLibrary(lua_State* luaVM)
{
	CreateMetaTableBasedOnClass<SDLTextBox>(luaVM, SDLTextBoxInstance);
    luaL_openlib(luaVM, "SDLTextBox", SDLTextBoxMetaTable, 0);
}


#endif
