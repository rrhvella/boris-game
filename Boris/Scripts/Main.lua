--[[
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
--]]

require "Scripts/Constants"
require "Scripts/MainMenuState"

function Main()	
	SDLInstance.InitializeVideo(false, 800, 600, 32, MAIN_FPS)
	SDLInstance.InitializeAudio(44100, 1024)
	
	COMMON_FONT = SDLFontFile.New("Data/ARIAL.TTF", 15)
	
	SDLInstance.AddRunStartHandler(GameStart)
	
	SDLInstance.Run()	
end

function GameStart()
	SDLInstance.LoadTrunk(GLOBAL_TRUNK_NAME)
	SDLInstance.PlayMusic(GLOBAL_TRUNK_NAME, MENU_BACKGROUND_MUSIC, -1, MUSIC_FADE_LENGTH)
	
	EnterMainMenuState()
end

