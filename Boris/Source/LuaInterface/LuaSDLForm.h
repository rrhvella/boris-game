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

#ifndef LUA_SDL_FORM_H
#define LUA_SDL_FORM_H

#include <iostream>

#include <Lua/lua.hpp>

#include <SDLInterface/SDLForm.h>
#include <SDLInterface/SDLText.h>
#include <SDLInterface/SDLTextBox.h>
#include <SDLInterface/SDLTimer.h>
#include <SDLInterface/SDLSurfaceGridComponent.h>
#include <EventHandling/KeyEventComponent.h>
#include <LuaInterface/LuaSDLInstance.h>
#include <Helpers/LuaHelperFunctions.h>

using namespace std;
using namespace SDLInterfaceLibrary;
using namespace Helpers;

int SDLForm_New(lua_State* luaVM)
{
	string formName = luaL_checkstring(luaVM, 1);

	Vector2D<int> position(luaL_checkint(luaVM, 2), luaL_checkint(luaVM, 3));

	string trunkName = luaL_checkstring(luaVM, 4);
	string imageName = luaL_checkstring(luaVM, 5);

	void* formInstance = CreateLuaInstanceBasedOnClass<SDLForm>(luaVM);

	new(formInstance) SDLForm(formName, position, ResourcePipelineSingleton::GetInstance().GetImage(trunkName, imageName));

	return 1;
}

int SDLForm_AddChild(lua_State* luaVM)
{
	vector<string> correctClasses;

	correctClasses.push_back(string(typeid(SDLText).name()));
	correctClasses.push_back(string(typeid(SDLTextBox).name()));
	correctClasses.push_back(string(typeid(SDLSurfaceGridComponent).name()));
	correctClasses.push_back(string(typeid(SDLComponent).name()));

	SDLForm* formInstance = RetrieveCPPObject<SDLForm>(luaVM, 1);
	SDLComponent* componentInstance = (SDLComponent*)LuaMultipleClassUDataCheck(luaVM, 2, correctClasses);

	formInstance->AddChild(componentInstance);

	return 0;
}

int SDLForm_Draw(lua_State* luaVM)
{
	SDLForm* formInstance = RetrieveCPPObject<SDLForm>(luaVM, 1);
	formInstance->Draw();

	return 0;
}


int SDLForm_SetFocus(lua_State* luaVM)
{
	SDLForm* formInstance = RetrieveCPPObject<SDLForm>(luaVM, 1);
	SDLTextBox* sdlTextBoxInstance = (SDLTextBox*)luaL_checkudata(luaVM, 2, typeid(SDLTextBox).name());

	formInstance->SetFocus(sdlTextBoxInstance);

	return 0;
}


int SDLForm_RemoveChild(lua_State* luaVM)
{
	vector<string> correctClasses;

	correctClasses.push_back(string(typeid(SDLText).name()));
	correctClasses.push_back(string(typeid(SDLTextBox).name()));
	correctClasses.push_back(string(typeid(SDLSurfaceGridComponent).name()));
	correctClasses.push_back(string(typeid(SDLComponent).name()));

	SDLForm* formInstance = RetrieveCPPObject<SDLForm>(luaVM, 1);
	SDLComponent* componentInstance = (SDLComponent*)LuaMultipleClassUDataCheck(luaVM, 2, correctClasses);

	formInstance->RemoveChild(componentInstance);

	return 0;
}

int SDLForm_AddTimer(lua_State* luaVM)
{
	SDLForm* formInstance = RetrieveCPPObject<SDLForm>(luaVM, 1);
	SDLTimer* sdlTimerInstance = (SDLTimer*)luaL_checkudata(luaVM, 2, typeid(SDLTimer).name());

	formInstance->AddTimer(*sdlTimerInstance);

	return 0;
}

int SDLForm_AddKeyDownHandler(lua_State* luaVM)
{
	SDLForm* formInstance = RetrieveCPPObject<SDLForm>(luaVM, 1);

	lua_remove(luaVM, 1);
	formInstance->GetKeyDownHandlers().AddLuaEventHandler(luaVM);

	return 0;
}

int SDLForm_AddKeyUpHandler(lua_State* luaVM)
{
	SDLForm* formInstance = RetrieveCPPObject<SDLForm>(luaVM, 1);

	lua_remove(luaVM, 1);
	formInstance->GetKeyUpHandlers().AddLuaEventHandler(luaVM);

	return 0;
}

int SDLForm_DestroyInstance(lua_State* luaVM)
{
	SDLForm* sdlFormInstance = RetrieveCPPObject<SDLForm>(luaVM, 1);
	sdlFormInstance->~SDLForm();

	return 0;
}


struct luaL_Reg SDLFormMetaTable [] =
{
	{"New", SDLForm_New},
	{NULL, NULL}
};


struct luaL_Reg SDLFormInstance [] =
{
	{"AddChild", SDLForm_AddChild},
	{"RemoveChild", SDLForm_RemoveChild},
	{"AddTimer", SDLForm_AddTimer},
	{"SetFocus", SDLForm_SetFocus},
	{"Draw", SDLForm_Draw},
	{"AddKeyDownHandler", SDLForm_AddKeyDownHandler},
	{"AddKeyUpHandler", SDLForm_AddKeyUpHandler},
	{"__gc", SDLForm_DestroyInstance},
	{NULL, NULL}
};


void RegisterSDLFormLibrary(lua_State* luaVM)
{
	CreateMetaTableBasedOnClass<SDLForm>(luaVM, SDLFormInstance);
    luaL_openlib(luaVM, "SDLForm", SDLFormMetaTable, 0);
}
#endif
