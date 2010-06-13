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

#ifndef LUA_HELPER_FUNCTIONS_H
#define LUA_HELPER_FUNCTIONS_H

#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>

#include <Lua/lua.hpp>

#include <Helpers/StringHelperFunctions.h>

using namespace std;

/*
File: LuaHelperFunctions.h

Contains a collection of functions meant to ease the process of raw lua binding.
*/

namespace Helpers
{
	/*
		Function: DumpStackToConsole

		Dumps a list of the objects currently present on the stack, from top
		to bottom, to the console.

		Parameters:
			luaVM - The pointer to the lua_State whose stack will be dumped
				    to the console.
	*/
	extern void DumpStackToConsole(lua_State* luaVM);

	/*
		Function: CreateMetaTable

		Creates a new metatable on the global registry, and assigns the functions
		in metatableFunctions to it.

		Parameters:
			luaVM - The pointer to the lua_State where you would like to
					create the metatable.

			metaTableName - The new metatable's name.

			metatableFunctions - The functions you would like to assign to the
								 new metatable.

	*/
	extern void CreateMetaTable(lua_State* luaVM, const char* metaTableName, luaL_Reg* metatableFunctions);

	/*
		Function: CreateLuaInstanceBasedOnClass

		Creates a new userdatum and pushes it unto the stack. It then uses
		the string passed in metaTableName to retrieve it's metatable from
		the global registry.


		Parameters:
			luaVM - The pointer to the lua_State where you would like to
					create the userdatum.

			size - The size (in bytes) of the new userdatum.

			metaTableName - The new userdatum's metatable.

		Returns:
			The memory location of the newly created userdatum.

	*/
	extern void* CreateLuaInstance(lua_State* luaVM, unsigned int size, const char* metaTableName);

	/*
		Function: LuaMultipleClassUDataCheck

		Performs a "safe" user data check on multiple metatables. This function is an ideal
		alternative luaL_udatacheck which immediately throws an error when the userdata it checks
		doesn't conform to the single metatable passed to it. On the other hand this function
		will only throw an error if the userdata doesn't match all the classes passed to it.

		This allows the binding developer to polymorph the userdatum to a base class, if it
		matches one of the metatables specified in "correctMetaTables".

		Parameters:
			luaVM - Pointer to the lua_State which will be checked.
			position - Position of the userdatum on the stack.
			correctMetaTables - List of metatables which will be used to validate the userdatum on
								[luaVM] at position [position]

		Returns:
			The memory location of the validated userdatum.

	*/
	extern void* LuaMultipleClassUDataCheck(lua_State* luaVM, int position, const vector<string>& correctMetaTables);

	/*
		Function: LuaCheckBoolean

		This function checks if a value at the given position on the stack is a boolean. If it isn't
		then the proper error is raised on the lua state.

		Parameters:
			luaVM - Pointer to the lua_State which will be checked.
			position - Position of the boolean on the stack.

		Returns:
			The boolean at the given position on the stick. If there isn't
			a valid boolean at that position then the proper error is raised on luaVM.

	*/
	extern bool LuaCheckBoolean(lua_State* luaVM, int position);

	/*
		Function: GetIntField

		This function allows you to retrieve a field from the table at the top
		of the stack, by using that field's key. The field however, must return
		an integer, or a value that can be converted to an integer.

		Parameters:
			luaVM - Pointer to a lua_State which has a table at the top of it's stack.
			key - The key of the field you would like to retrieve.

		Returns:
			The int in t[k], where t is the table at the top of the stack, and k is the string value
			passed in the variable "key".

	*/
	extern int GetIntField(lua_State* luaVM, const char* key);

	/*
		Function: CreateLuaInstanceBasedOnClass

		Creates a userdatum based on the size of C++ class, it also retrieves the
		metatable based on it's name.

		Templates:
			CPPClass - The class you would like to use to retrieve the metatable.

		Parameters:
			luaVM - The pointer to the lua_State where you would like to create the userdatum.

		Returns:
			The memory location of the newly created userdatum.

		See Also:
			<CreateLuaInstance>
			<CreateMetaTableBasedOnClass>
	*/
	template<class CPPClass>
	static void* CreateLuaInstanceBasedOnClass(lua_State* luaVM)
	{
		return CreateLuaInstance(luaVM, sizeof(CPPClass), typeid(CPPClass).name());
	}

	/*
		Function: CreateMetaTableBasedOnClass

		Creates a metatable based on the name of a C++ class.

		Templates:
			CPPClass - The class you would like to use to name the metatable.

		Parameters:
			luaVM - The pointer to the lua_State where you would like to create the metatable.

			metatableFunctions - The functions you would like to assign to the
								 new metatable.

		See Also:
			<CreateMetaTable>
	*/
	template<class CPPClass>
	static void CreateMetaTableBasedOnClass(lua_State* luaVM, luaL_Reg* metatableFunctions)
	{
		CreateMetaTable(luaVM, typeid(CPPClass).name(), metatableFunctions);
	}

	/*
		Function: RetrieveCPPObject

		Retrieves a userdatum and casts it to a C++ object. It also validates the userdatum by
		matching it to the metatable correspoding to the C++ class passed in the template.

		Templates:
			CPPClass - The class you would like to use to validate the userdatum's metatable,

		Parameters:
			luaVM - The pointer to the lua_State where you would like to create the metatable.

			position - The position of the userdatum on the stack.

		Returns:
			The memory location of the userdatum at the given position, cast to the template CPPClass.

		See Also:
			<CreateMetaTableBasedOnClass>
			<CreateLuaInstanceBasedOnClass>
	*/
	template<class CPPClass>
	static CPPClass* RetrieveCPPObject(lua_State* luaVM, int position)
	{
		return (CPPClass*)luaL_checkudata(luaVM, position, typeid(CPPClass).name());
	}
}


#endif
