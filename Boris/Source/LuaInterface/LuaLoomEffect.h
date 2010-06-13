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

#ifndef LUA_LOOM_EFFECT_H
#define LUA_LOOM_EFFECT_H

#include <Lua/lua.hpp>

#include <SDLInterface/SDLEffects.h>

using namespace SDLInterfaceLibrary;
using namespace Helpers;

int LoomEffect_New(lua_State* luaVM)
{
	double rateOfIncrease = luaL_checknumber(luaVM, 1);
	double maxSize = luaL_checknumber(luaVM, 2);

	void* loomEffectInstance = CreateLuaInstanceBasedOnClass<LoomEffect>(luaVM);

	new(loomEffectInstance) LoomEffect(rateOfIncrease, maxSize);

	return 1;
}

int LoomEffect_DestroyInstance(lua_State* luaVM)
{
	LoomEffect* loomEffectInstance = RetrieveCPPObject<LoomEffect>(luaVM, 1);
	loomEffectInstance->~LoomEffect();

	return 0;
}

struct luaL_Reg LoomEffectMetaTable [] =
{
	{"New", LoomEffect_New},
	{NULL, NULL}
};


struct luaL_Reg LoomEffectInstance [] =
{
	{"__gc", LoomEffect_DestroyInstance},
	{NULL, NULL}
};



void RegisterLoomEffectLibrary(lua_State* luaVM)
{
	CreateMetaTableBasedOnClass<LoomEffect>(luaVM, LoomEffectInstance);
    luaL_openlib(luaVM, "LoomEffect", LoomEffectMetaTable, 0);
}


#endif
