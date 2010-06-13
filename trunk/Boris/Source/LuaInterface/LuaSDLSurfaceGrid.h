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

#ifndef LUA_SURFACE_GRID_H
#define LUA_SURFACE_GRID_H

#include <string>
#include <vector>

#include <Lua/lua.hpp>

#include <Helpers/LuaHelperFunctions.h>
#include <SDLInterface/SDLEffects.h>

using namespace SDLInterfaceLibrary;
using namespace Helpers;

int SDLSurfaceGrid_New(lua_State* luaVM)
{
	string name = luaL_checkstring(luaVM, 1);

	if(!lua_istable(luaVM, 2))
	{
		Bounds2D<int> surfaceGridSize(luaL_checkint(luaVM, 2), luaL_checkint(luaVM, 3));
		Bounds2D<int> surfaceSize(luaL_checkint(luaVM, 4), luaL_checkint(luaVM, 5));

		Vector2D<int> anchorBlockPosition(luaL_checkint(luaVM, 6), luaL_checkint(luaVM, 7));
		int anchorPoint = luaL_checkint(luaVM, 8);

		if(anchorPoint > 4 || anchorPoint < 0)
		{
			luaL_argerror(luaVM, 4, "Invalid anchor point value");
		}

		void* sdlSurfaceGridInstance = CreateLuaInstanceBasedOnClass<SDLSurfaceGrid>(luaVM);
		new(sdlSurfaceGridInstance) SDLSurfaceGrid(name, surfaceGridSize, surfaceSize, anchorBlockPosition, (SDLSurfacePoint)anchorPoint);
	}
	else
	{
		lua_pushvalue(luaVM, 2);
		vector< vector<unsigned int> > placementMap;

		for(lua_pushnil(luaVM); lua_next(luaVM, -2); lua_pop(luaVM, 1))
		{
			vector<unsigned int> currentRow;

			for(lua_pushnil(luaVM); lua_next(luaVM, -2); lua_pop(luaVM, 1))
			{
				currentRow.push_back(luaL_checkint(luaVM, -1));
			}

			placementMap.push_back(currentRow);
		}

		lua_pop(luaVM, 1);

		Grid<unsigned int> placementGrid(placementMap[0].size(), placementMap.size());
		for(int x = 0; x < placementGrid.GetWidth(); x++)
		{
			for(int y = 0; y < placementGrid.GetHeight(); y++)
			{
				placementGrid.Set(x, y, placementMap[y][x]);
			}
		}

		string trunkName = luaL_checkstring(luaVM, 3);
		string imageName = luaL_checkstring(luaVM, 4);

		vector<SDL_Surface*> surfaceList;
		surfaceList.push_back(ResourcePipelineSingleton::GetInstance().GetImage(trunkName, imageName));

		Vector2D<int> anchorBlockPosition(luaL_checkint(luaVM, 5), luaL_checkint(luaVM, 6));
		int anchorPoint = luaL_checkint(luaVM, 7);

		if(anchorPoint > 4 || anchorPoint < 0)
		{
			luaL_argerror(luaVM, 7, "Invalid anchor point value");
		}

		void* sdlSurfaceGridInstance = CreateLuaInstanceBasedOnClass<SDLSurfaceGrid>(luaVM);
		new(sdlSurfaceGridInstance) SDLSurfaceGrid(name, placementGrid, surfaceList, anchorBlockPosition,
						(SDLSurfacePoint) anchorPoint);
	}

	return 1;
}

int SDLSurfaceGrid_DestroyInstance(lua_State* luaVM)
{
	SDLSurfaceGrid* sdlSurfaceGridInstance = RetrieveCPPObject<SDLSurfaceGrid>(luaVM, 1);
	sdlSurfaceGridInstance->~SDLSurfaceGrid();

	return 0;
}

struct luaL_Reg SDLSurfaceGridMetaTable [] =
{
	{"New", SDLSurfaceGrid_New},
	{NULL, NULL}
};


struct luaL_Reg SDLSurfaceGridInstance [] =
{
	{"__gc", SDLSurfaceGrid_DestroyInstance},
	{NULL, NULL}
};



void RegisterSDLSurfaceGridLibrary(lua_State* luaVM)
{
	CreateMetaTableBasedOnClass<SDLSurfaceGrid>(luaVM, SDLSurfaceGridInstance);
    luaL_openlib(luaVM, "SDLSurfaceGrid", SDLSurfaceGridMetaTable, 0);
}


#endif
