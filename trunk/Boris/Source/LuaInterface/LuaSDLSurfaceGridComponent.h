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

#ifndef LUA_SDL_SURFACE_GRID_COMPONENT_H
#define LUA_SDL_SURFACE_GRID_COMPONENT_H

#include <Lua/lua.hpp>

#include <LuaInterface/LuaSDLInstance.h>
#include <Helpers/LuaHelperFunctions.h>
#include <SDLInterface/SDLSurfaceGrid.h>
#include <SDLInterface/SDLSurfaceGridComponent.h>

using namespace SDLInterfaceLibrary;
using namespace Helpers;

int SDLSurfaceGridComponent_New(lua_State* luaVM)
{
	string name = luaL_checkstring(luaVM, 1);
	Vector2D<int> position = Vector2D<int>(luaL_checkint(luaVM, 2), luaL_checkint(luaVM, 3));

	vector<SDLSurfaceGrid> surfaceGrids;

	if(lua_istable(luaVM, 4))
	{
		lua_pushvalue(luaVM, 4);

		for(lua_pushnil(luaVM); lua_next(luaVM, -2); lua_pop(luaVM, 1))
		{
			SDLSurfaceGrid* sdlSurfaceGridInstance = (SDLSurfaceGrid*)luaL_checkudata(luaVM, -1, typeid(SDLSurfaceGrid).name());
			surfaceGrids.push_back(*sdlSurfaceGridInstance);
		}

		lua_pop(luaVM, 1);
	}
	else
	{
		SDLSurfaceGrid* surfaceGridInstance = (SDLSurfaceGrid*)luaL_checkudata(luaVM, 4, typeid(SDLSurfaceGrid).name());
		surfaceGrids.push_back(*surfaceGridInstance);
	}


	int clearingMethod = luaL_checkint(luaVM, 5);

	if(clearingMethod > 1 || clearingMethod < 0)
	{
		luaL_argerror(luaVM, 5, "Invalid clearing method value");
	}

	void* sdlSurfaceGridComponentInstance = CreateLuaInstanceBasedOnClass<SDLSurfaceGridComponent>(luaVM);

	new(sdlSurfaceGridComponentInstance) SDLSurfaceGridComponent(name, position, surfaceGrids, (SDLClearingMethod)clearingMethod);
	return 1;
}

int SDLSurfaceGridComponent_First(lua_State* luaVM)
{
	SDLSurfaceGridComponent* sdlSurfaceGridComponentInstance = RetrieveCPPObject<SDLSurfaceGridComponent>(luaVM, 1);
	sdlSurfaceGridComponentInstance->First();

	return 0;
}


int SDLSurfaceGridComponent_Next(lua_State* luaVM)
{
	SDLSurfaceGridComponent* sdlSurfaceGridComponentInstance = RetrieveCPPObject<SDLSurfaceGridComponent>(luaVM, 1);
	sdlSurfaceGridComponentInstance->Next();

	return 0;
}


int SDLSurfaceGridComponent_Previous(lua_State* luaVM)
{
	SDLSurfaceGridComponent* sdlSurfaceGridComponentInstance = RetrieveCPPObject<SDLSurfaceGridComponent>(luaVM, 1);
	sdlSurfaceGridComponentInstance->Previous();

	return 0;
}

int SDLSurfaceGridComponent_SurfacesCollide(lua_State* luaVM)
{
	SDLSurfaceGridComponent* sdlSurfaceGridComponentInstance = RetrieveCPPObject<SDLSurfaceGridComponent>(luaVM, 1);
	SDLSurfaceGridComponent* target = (SDLSurfaceGridComponent*)luaL_checkudata(luaVM, 2, typeid(SDLSurfaceGridComponent).name());

	bool treatOutsideBoundsAsCollision = LuaCheckBoolean(luaVM, 3);

	bool result = sdlSurfaceGridComponentInstance->SurfacesCollide(*target, treatOutsideBoundsAsCollision);
	lua_pushboolean(luaVM, result);

	return 1;
}

int SDLSurfaceGridComponent_ClearCurrentSurfaces(lua_State* luaVM)
{
	SDLSurfaceGridComponent* sdlSurfaceGridComponentInstance = RetrieveCPPObject<SDLSurfaceGridComponent>(luaVM, 1);

	if(lua_isnil(luaVM, 2))
	{
		sdlSurfaceGridComponentInstance->ClearCurrentSurfaces(NULL);
	}
	else if(lua_istable(luaVM, 2))
	{

		vector<int> rectFields;
		lua_pushvalue(luaVM, 2);

		Dimensions2D<int> portionToClear(GetIntField(luaVM, "x"), GetIntField(luaVM, "y"), GetIntField(luaVM, "width"), GetIntField(luaVM, "height"));

		lua_pop(luaVM, 1);

		sdlSurfaceGridComponentInstance->ClearCurrentSurfaces(&portionToClear);
	}
	else
	{
		luaL_argerror(luaVM, 2, "Expected table or nil");
	}


	return 0;
}

int SDLSurfaceGridComponent_ReplaceCurrentSurfaces(lua_State* luaVM)
{
	SDLSurfaceGridComponent* sdlSurfaceGridComponentInstance = RetrieveCPPObject<SDLSurfaceGridComponent>(luaVM, 1);
	SDLSurfaceGridComponent* target = (SDLSurfaceGridComponent*)luaL_checkudata(luaVM, 2, typeid(SDLSurfaceGridComponent).name());

	Dimensions2D<int>* pPortionToCopy;

	if(lua_isnil(luaVM, 3))
	{
		pPortionToCopy = NULL;
	}
	else if(lua_istable(luaVM, 3))
	{
		lua_pushvalue(luaVM, 3);

		Dimensions2D<int> portionToCopy(GetIntField(luaVM, "x"), GetIntField(luaVM, "y"), GetIntField(luaVM, "width"), GetIntField(luaVM, "height"));
		pPortionToCopy = &portionToCopy;

		lua_pop(luaVM, 1);
	}
	else
	{
		luaL_argerror(luaVM, 3, "Expected table or nil");
	}

	Vector2D<int> positionToCopyTo(luaL_checkint(luaVM, 4), luaL_checkint(luaVM, 5));
	bool nullOverwrite = LuaCheckBoolean(luaVM, 6);

	sdlSurfaceGridComponentInstance->ReplaceCurrentSurfaces(target->GetCurrentSurfaceGrid(), positionToCopyTo, nullOverwrite, pPortionToCopy);
	return 0;
}

int SDLSurfaceGridComponent_MoveCurrentSurfaces(lua_State* luaVM)
{
	SDLSurfaceGridComponent* sdlSurfaceGridComponentInstance = RetrieveCPPObject<SDLSurfaceGridComponent>(luaVM, 1);

	Dimensions2D<int>* pPortionToCopy;

	if(lua_isnil(luaVM, 2))
	{
		pPortionToCopy = NULL;
	}
	else if(lua_istable(luaVM, 2))
	{
		lua_pushvalue(luaVM, 2);

		Dimensions2D<int> portionToCopy(GetIntField(luaVM, "x"), GetIntField(luaVM, "y"), GetIntField(luaVM, "width"), GetIntField(luaVM, "height"));
		pPortionToCopy = &portionToCopy;

		lua_pop(luaVM, 1);
	}
	else
	{
		luaL_argerror(luaVM, 2, "Expected table or nil");
	}

	SDLSurfaceGrid surfaceGridCopy = sdlSurfaceGridComponentInstance->CopyCurrentSurfaces(pPortionToCopy);

	Vector2D<int> positionToCopyTo(luaL_checkint(luaVM, 3), luaL_checkint(luaVM, 4));

	sdlSurfaceGridComponentInstance->ReplaceCurrentSurfaces(surfaceGridCopy, positionToCopyTo, true, pPortionToCopy);
	return 0;
}


int SDLSurfaceGridComponent_GetWidth(lua_State* luaVM)
{
	SDLSurfaceGridComponent* sdlSurfaceGridComponentInstance = RetrieveCPPObject<SDLSurfaceGridComponent>(luaVM, 1);
	int result = sdlSurfaceGridComponentInstance->GetWidth();

	lua_pushinteger(luaVM, result);

	return 1;
}

int SDLSurfaceGridComponent_GetHeight(lua_State* luaVM)
{
	SDLSurfaceGridComponent* sdlSurfaceGridComponentInstance = RetrieveCPPObject<SDLSurfaceGridComponent>(luaVM, 1);
	int result = sdlSurfaceGridComponentInstance->GetHeight();

	lua_pushinteger(luaVM, result);

	return 1;
}

int SDLSurfaceGridComponent_GetLeft(lua_State* luaVM)
{
	SDLSurfaceGridComponent* sdlSurfaceGridComponentInstance = RetrieveCPPObject<SDLSurfaceGridComponent>(luaVM, 1);
	int result = sdlSurfaceGridComponentInstance->GetLeftFromOrigin();

	lua_pushinteger(luaVM, result);

	return 1;
}

int SDLSurfaceGridComponent_GetTop(lua_State* luaVM)
{
	SDLSurfaceGridComponent* sdlSurfaceGridComponentInstance = RetrieveCPPObject<SDLSurfaceGridComponent>(luaVM, 1);
	int result = sdlSurfaceGridComponentInstance->GetTopFromOrigin();

	lua_pushinteger(luaVM, result);

	return 1;
}

int SDLSurfaceGridComponent_SetLeft(lua_State* luaVM)
{
	SDLSurfaceGridComponent* sdlSurfaceGridComponentInstance = RetrieveCPPObject<SDLSurfaceGridComponent>(luaVM, 1);
	int newLeft = luaL_checkint(luaVM, 2);

	sdlSurfaceGridComponentInstance->SetLeft(newLeft);

	return 0;
}

int SDLSurfaceGridComponent_SetTop(lua_State* luaVM)
{
	SDLSurfaceGridComponent* sdlSurfaceGridComponentInstance = RetrieveCPPObject<SDLSurfaceGridComponent>(luaVM, 1);
	int newTop = luaL_checkint(luaVM, 2);

	sdlSurfaceGridComponentInstance->SetTop(newTop);

	return 0;
}

int SDLSurfaceGridComponent_AddToLeft(lua_State* luaVM)
{
	SDLSurfaceGridComponent* sdlSurfaceGridComponentInstance = RetrieveCPPObject<SDLSurfaceGridComponent>(luaVM, 1);
	int increment = luaL_checkint(luaVM, 2);

	sdlSurfaceGridComponentInstance->AddToLeft(increment);

	return 0;
}

int SDLSurfaceGridComponent_AddToTop(lua_State* luaVM)
{
	SDLSurfaceGridComponent* sdlSurfaceGridComponentInstance = RetrieveCPPObject<SDLSurfaceGridComponent>(luaVM, 1);
	int increment = luaL_checkint(luaVM, 2);

	sdlSurfaceGridComponentInstance->AddToTop(increment);

	return 0;
}

int SDLSurfaceGridComponent_DestroyInstance(lua_State* luaVM)
{
	SDLSurfaceGridComponent* sdlSurfaceGridComponentInstance = RetrieveCPPObject<SDLSurfaceGridComponent>(luaVM, 1);
	sdlSurfaceGridComponentInstance->~SDLSurfaceGridComponent();

	return 0;
}

int SDLSurfaceGridComponent_GetCompleteLineIndexes(lua_State* luaVM)
{
	SDLSurfaceGridComponent* sdlSurfaceGridComponentInstance = RetrieveCPPObject<SDLSurfaceGridComponent>(luaVM, 1);
	SDLSurfaceGrid& surfaceGridToCheck  = sdlSurfaceGridComponentInstance->GetCurrentSurfaceGrid();

	//Find completed lines indexes
	vector<int> completeLineIndexes;

	for(int y = 0; y < surfaceGridToCheck.GetHeight(); y++)
	{
		bool lineComplete = true;

		for(int x = 0; x < surfaceGridToCheck.GetWidth(); x++)
		{
			if(surfaceGridToCheck.GetSurface(Vector2D<int>(x, y)) == NULL)
			{
				lineComplete = false;
				break;
			}
		}

		if(lineComplete)
		{
			completeLineIndexes.push_back(y);
		}
	}

	//Push complete line indexes to lua

	//Create a new table, and push it to lua
	lua_newtable(luaVM);

	for(unsigned int currentIndex = 0; currentIndex < completeLineIndexes.size(); currentIndex++)
	{
		//Push the value we need to set
		lua_pushnumber(luaVM, completeLineIndexes[currentIndex]);
		//Set the value (The previously created table is at index -2, with the value we pushed before right above it)
		lua_rawseti(luaVM, -2, currentIndex + 1);
	}

	//Return the table with the indexes of the completed lines
	return 1;
}


struct luaL_Reg SDLSurfaceGridComponentMetaTable [] =
{
	{"New", SDLSurfaceGridComponent_New},
	{NULL, NULL}
};


struct luaL_Reg SDLSurfaceGridComponentInstance [] =
{
	{"First", SDLSurfaceGridComponent_First},
	{"Next", SDLSurfaceGridComponent_Next},
	{"Previous", SDLSurfaceGridComponent_Previous},
	{"SurfacesCollide", SDLSurfaceGridComponent_SurfacesCollide},
	{"ClearCurrentSurfaces", SDLSurfaceGridComponent_ClearCurrentSurfaces},
	{"ReplaceCurrentSurfaces", SDLSurfaceGridComponent_ReplaceCurrentSurfaces},
	{"MoveCurrentSurfaces", SDLSurfaceGridComponent_MoveCurrentSurfaces},
	{"GetLeft", SDLSurfaceGridComponent_GetLeft},
	{"GetTop", SDLSurfaceGridComponent_GetTop},
	{"GetWidth", SDLSurfaceGridComponent_GetWidth},
	{"GetHeight", SDLSurfaceGridComponent_GetHeight},
	{"AddToLeft", SDLSurfaceGridComponent_AddToLeft},
	{"AddToTop", SDLSurfaceGridComponent_AddToTop},
	{"SetLeft", SDLSurfaceGridComponent_SetLeft},
	{"SetTop", SDLSurfaceGridComponent_SetTop},
	{"GetCompleteLineIndexes", SDLSurfaceGridComponent_GetCompleteLineIndexes},
	{"__gc", SDLSurfaceGridComponent_DestroyInstance},
	{NULL, NULL}
};


void RegisterSDLSurfaceGridComponentLibrary(lua_State* luaVM)
{
	CreateMetaTableBasedOnClass<SDLSurfaceGridComponent>(luaVM, SDLSurfaceGridComponentInstance);
    luaL_openlib(luaVM, "SDLSurfaceGridComponent", SDLSurfaceGridComponentMetaTable, 0);
}

#endif
