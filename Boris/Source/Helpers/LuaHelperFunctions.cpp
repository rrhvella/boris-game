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

#include <Helpers/LuaHelperFunctions.h>

void Helpers::DumpStackToConsole(lua_State* luaVM)
{
	int objectsOnStack = lua_gettop(luaVM);

	for(int i = -1; i >= -objectsOnStack; i--)
	{
		cout << luaL_typename(luaVM, i) << endl;
	}
}

void Helpers::CreateMetaTable(lua_State* luaVM, const char* metaTableName, luaL_Reg* metatableFunctions)
{
	 luaL_newmetatable(luaVM, metaTableName);

	 /*The following code block performs the following operation in lua:
		[metatable].__index = [metatable]. Where [metatable] is the metatable
		created above.
	 */
	 lua_pushstring(luaVM, "__index");
	 lua_pushvalue(luaVM, -2);
	 lua_settable(luaVM, -3);

	 luaL_openlib(luaVM, NULL, metatableFunctions, 0);
}

void* Helpers::CreateLuaInstance(lua_State* luaVM, unsigned int size, const char* metaTableName)
{
	void* instance = lua_newuserdata(luaVM, size);

	luaL_getmetatable(luaVM, metaTableName);
	lua_setmetatable(luaVM, -2);

	return instance;
}

void* Helpers::LuaMultipleClassUDataCheck(lua_State* luaVM, int position, const vector<string>& correctMetaTables)
{
	void* instance = lua_touserdata(luaVM, position);

	if(instance == NULL)
	{
		luaL_typerror(luaVM, position, ToStringList(correctMetaTables).c_str());
	}

	//Push the userdatum's metatable on the stack (It will now be at position -1)
	if(!lua_getmetatable(luaVM, position))
	{
		luaL_typerror(luaVM, position, ToStringList(correctMetaTables).c_str());
	}

	bool correctClassFound = false;

	for(vector<string>::const_iterator currentString = correctMetaTables.begin();
		currentString != correctMetaTables.end();
		currentString++)
	{
		//Get the reference of the current metatable.
		lua_getfield(luaVM, LUA_REGISTRYINDEX, currentString->c_str());

		//If the userdatum's metatable (position -2) matches the current metatable (-1)
		if(lua_rawequal(luaVM, -1, -2))
		{
			correctClassFound = true;

			//Remove the current metatable and exit the loop.
			lua_pop(luaVM, 1);
			break;
		}
		//Remove the current metatable
		lua_pop(luaVM, 1);
	}


	if(!correctClassFound)
	{
		luaL_typerror(luaVM, position, ToStringList(correctMetaTables).c_str());
	}

	//Remove the userdatum's metatable.
	lua_pop(luaVM, 1);

	return instance;
}

bool Helpers::LuaCheckBoolean(lua_State* luaVM, int position)
{
	 if(!lua_isboolean(luaVM, position))
	 {
		 luaL_argerror(luaVM, position, "expected boolean");
	 }

	 return lua_toboolean(luaVM, position) != 0;
}

int Helpers::GetIntField(lua_State* luaVM, const char* key)
{
	  int result;
	  lua_pushstring(luaVM, key);
	  lua_gettable(luaVM, -2);

	  if(lua_isnil(luaVM, -1))
	  {
		   string error = "Attempted to pass table without setting attribute ";
		   error += key;

		   luaL_error(luaVM, error.c_str());
	  }

	  if(!lua_isnumber(luaVM, -1))
	  {
		   string error = "Attempted to pass table without setting attribute ";
		   error += key;

		   luaL_error(luaVM, error.c_str());
	  }

	  result = (int)lua_tonumber(luaVM, -1);
	  lua_pop(luaVM, 1);

	  return result;
}

