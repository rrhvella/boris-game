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

#ifndef LUA_SDL_INSTANCE_H
#define LUA_SDL_INSTANCE_H

#include <Lua/lua.hpp>

#include <ResourcePipelineSingleton.h>
#include <Helpers/LuaHelperFunctions.h>
#include <SDLInterface/SDLResourceTrunk.h>
#include <SDLInterface/SDLForm.h>
#include <SDLInterface/SDLInstance.h>
#include <SDLInterface/SDLScreenEffects.h>


using namespace SDLInterfaceLibrary;
using namespace Helpers;

int SDLInstance_InitializeVideo(lua_State* luaVM)
{
	bool fullScreen = LuaCheckBoolean(luaVM, 1);
	Bounds2D<int> windowSize(luaL_checkint(luaVM, 2), luaL_checkint(luaVM, 3));
	int colorDepth = luaL_checkint(luaVM, 4);
	int frameRate = luaL_checkint(luaVM, 5);

	SDLInstance& sdlInstance = SDLInstance::GetInstance();
	sdlInstance.InitializeVideo(fullScreen, windowSize, colorDepth, frameRate);

	return 0;
}

int SDLInstance_InitializeAudio(lua_State* luaVM)
{

	int sampleRate = luaL_checkint(luaVM, 1);
	int bufferSize = luaL_checkint(luaVM, 2);

	SDLInstance& sdlInstance = SDLInstance::GetInstance();
	sdlInstance.InitializeAudio(sampleRate, bufferSize);

	return 0;
}

int SDLInstance_AddRunStartHandler(lua_State* luaVM)
{
	SDLInstance& sdlInstance = SDLInstance::GetInstance();
	sdlInstance.GetRunStartHandlers().AddLuaEventHandler(luaVM);

	return 0;
}

int SDLInstance_AddMusicEndHandler(lua_State* luaVM)
{
	SDLInstance& sdlInstance = SDLInstance::GetInstance();
	sdlInstance.GetMusicEndHandlers().AddLuaEventHandler(luaVM);

	return 0;
}

int SDLInstance_SetCursorEnabled(lua_State* luaVM)
{
	bool enabled = LuaCheckBoolean(luaVM, 1);

	SDLInstance& sdlInstance = SDLInstance::GetInstance();
	sdlInstance.SetCursorEnabled(enabled);

	return 0;
}

int SDLInstance_SetCaption(lua_State* luaVM)
{
	const char* caption = luaL_checkstring(luaVM, 1);

	SDLInstance& sdlInstance = SDLInstance::GetInstance();
	sdlInstance.SetCaption(caption);

	return 0;
}

int SDLInstance_SetFocus(lua_State* luaVM)
{
	SDLForm* formInstance = (SDLForm*)luaL_checkudata(luaVM, 1, typeid(SDLForm).name());

	SDLInstance& sdlInstance = SDLInstance::GetInstance();
	sdlInstance.SetFocus(formInstance);

	return 0;
}


int SDLInstance_PerformFlashEffect(lua_State* luaVM)
{
	vector< Dimensions2D<int> > portions;

	lua_pushvalue(luaVM, 1);

	for(lua_pushnil(luaVM); lua_next(luaVM, -2); lua_pop(luaVM, 1))
	{
		portions.push_back(Dimensions2D<int>(GetIntField(luaVM, "x"), GetIntField(luaVM, "y"),
								GetIntField(luaVM, "width"), GetIntField(luaVM, "height")));
	}

	lua_pop(luaVM, 1);

	int numberOfFrames = luaL_checkint(luaVM, 2);
	int numberOfIterations = luaL_checkint(luaVM, 3);

	FlashScreenEffect effect(portions, numberOfFrames, numberOfIterations);

	SDLInstance& sdlInstance = SDLInstance::GetInstance();
	sdlInstance.PerformScreenEffect(effect);

	return 0;
}

int SDLInstance_PerformOverlayEffect(lua_State* luaVM)
{
	SDLInstance& sdlInstance = SDLInstance::GetInstance();

	if(!lua_istable(luaVM, 1))
	{
		luaL_argerror(luaVM, 1, "Expected table");
	}

	lua_pushvalue(luaVM, 1);

	vector< Dimensions2D<int> > entireWindowPortion(1, Dimensions2D<int>(0, 0, sdlInstance.GetWindowWidth(), sdlInstance.GetWindowHeight()));

	OverlayScreenEffect effect(entireWindowPortion, GetIntField(luaVM, "r"), GetIntField(luaVM, "g"),
						GetIntField(luaVM, "b"), GetIntField(luaVM, "a"));

	sdlInstance.PerformScreenEffect(effect);

	return 0;
}

int SDLInstance_PlayMusic(lua_State* luaVM)
{
	string trunkName = luaL_checkstring(luaVM, 1);
	string musicName = luaL_checkstring(luaVM, 2);

	int numberOfLoops = luaL_checkint(luaVM, 3);
	int fadeInLength = luaL_checkint(luaVM, 4);

	Mix_Music* musicFile = ResourcePipelineSingleton::GetInstance().GetMusic(trunkName, musicName);

	SDLInstance& sdlInstance = SDLInstance::GetInstance();

	sdlInstance.PlayMusic(musicFile, numberOfLoops, fadeInLength);
	return 0;
}

int SDLInstance_PlaySound(lua_State* luaVM)
{
	string trunkName = luaL_checkstring(luaVM, 1);
	string soundName = luaL_checkstring(luaVM, 2);

	Mix_Chunk* soundFile = ResourcePipelineSingleton::GetInstance().GetSound(trunkName, soundName);

	SDLInstance& sdlInstance = SDLInstance::GetInstance();

	sdlInstance.PlaySound(soundFile);
	return 0;
}


int SDLInstance_FadeOutMusic(lua_State* luaVM)
{
	int fadeOutLength = luaL_checkint(luaVM, 1);

	SDLInstance& sdlInstance = SDLInstance::GetInstance();

	sdlInstance.FadeOutMusic(fadeOutLength);
	return 0;
}

int SDLInstance_HaltMusic(lua_State* luaVM)
{
	SDLInstance& sdlInstance = SDLInstance::GetInstance();

	sdlInstance.HaltMusic();
	return 0;
}

int SDLInstance_PerformFadeEffect(lua_State* luaVM)
{
	SDLInstance& sdlInstance = SDLInstance::GetInstance();

	int numberOfFrames = luaL_checkint(luaVM, 1);
	unsigned int animationLength = luaL_checkint(luaVM, 2);

	int fadeDirection = luaL_checkint(luaVM, 3);

	if(fadeDirection > 2 || fadeDirection < 0)
	{
		luaL_argerror(luaVM, 3, "Invalid fade direction");
	}

	vector< Dimensions2D<int> > entireWindowPortion(1, Dimensions2D<int>(0, 0, sdlInstance.GetWindowWidth(), sdlInstance.GetWindowHeight()));
	FadeScreenEffect effect(entireWindowPortion, numberOfFrames, animationLength, (SDLFadeDirection)fadeDirection);

	sdlInstance.PerformScreenEffect(effect);

	return 0;
}

int SDLInstance_Run(lua_State* luaVM)
{
	SDLInstance& sdlInstance = SDLInstance::GetInstance();
	sdlInstance.Run();

	return 0;
}

int SDLInstance_Quit(lua_State* luaVM)
{
	SDLInstance& sdlInstance = SDLInstance::GetInstance();
	sdlInstance.Quit();

	return 0;
}

int SDLInstance_LoadTrunk(lua_State* luaVM)
{
	string trunkName = luaL_checkstring(luaVM, 1);

	ResourcePipelineSingleton::GetInstance().LoadResourceTrunk(trunkName);

	return 0;
}

int SDLInstance_UnloadTrunk(lua_State* luaVM)
{
	string trunkName = luaL_checkstring(luaVM, 1);

	ResourcePipelineSingleton::GetInstance().UnloadResourceTrunk(trunkName);

	return 0;
}

int SDLInstance_KeyIsPressed(lua_State* luaVM)
{
	SDLInstance& sdlInstance = SDLInstance::GetInstance();
	int keySym = luaL_checkint(luaVM, 1);

	bool result = sdlInstance.KeyIsPressed(keySym);

	lua_pushboolean(luaVM, result);

	return 1;
}

struct luaL_Reg SDLInstanceMetaTable [] =
{
	{"InitializeVideo", SDLInstance_InitializeVideo},
	{"InitializeAudio", SDLInstance_InitializeAudio},
	{"AddRunStartHandler", SDLInstance_AddRunStartHandler},
	{"AddMusicEndHandler", SDLInstance_AddMusicEndHandler},
	{"SetCursorEnabled", SDLInstance_SetCursorEnabled},
	{"SetCaption", SDLInstance_SetCaption},
	{"SetFocus", SDLInstance_SetFocus},
	{"PerformFlashEffect", SDLInstance_PerformFlashEffect},
	{"PerformOverlayEffect", SDLInstance_PerformOverlayEffect},
	{"PerformFadeEffect", SDLInstance_PerformFadeEffect},
	{"Run", SDLInstance_Run},
	{"Quit", SDLInstance_Quit},
	{"LoadTrunk", SDLInstance_LoadTrunk},
	{"UnloadTrunk", SDLInstance_UnloadTrunk},
	{"KeyIsPressed", SDLInstance_KeyIsPressed},
	{"PlayMusic", SDLInstance_PlayMusic},
	{"PlaySound", SDLInstance_PlaySound},
	{"FadeOutMusic", SDLInstance_FadeOutMusic},
	{"HaltMusic", SDLInstance_HaltMusic},
	{NULL, NULL}
};

void RegisterSDLInstanceLibrary(lua_State* luaVM)
{
	luaL_openlib(luaVM, "SDLInstance", SDLInstanceMetaTable, 0);
}


#endif

