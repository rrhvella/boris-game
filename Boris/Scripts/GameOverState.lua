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
require "Scripts/ScoreTableState"
TableHelper = require "Scripts/TableHelperFunctions"

local GameOverState = {}

function EnterGameOverState(currentPlayer)
	GameOverState:CreateGameOverState(currentPlayer)
end

function GameOverState:CreateGameOverState(currentPlayer)	
	collectgarbage()	
	
	self.currentPlayer = currentPlayer
	
	SDLInstance.LoadTrunk(GAME_0VER_TRUNK_NAME)
	
	SDLInstance.HaltMusic();
	SDLInstance.PerformOverlayEffect(GAME_OVERLAY_COLOR)	
	SDLInstance.PlayMusic(GLOBAL_TRUNK_NAME, GAME_OVER_BACKGROUND_MUSIC, -1, MUSIC_FADE_LENGTH)
	
	self.gameOverForm = SDLForm.New("GameOver", GAME_OVER_POSITION.x, GAME_OVER_POSITION.y, GAME_0VER_TRUNK_NAME, GAME_OVER_BACKGROUND_IMAGE_NAME)
	self.nameTextBox = SDLTextBox.New("Name Text Box", NAME_TEXT_BOX_POSITION.x, NAME_TEXT_BOX_POSITION.y, "", COMMON_FONT, 
			NAME_TEXT_BOX_MAX_LENGTH, COMMON_TEXT_COLOR)	
				
	self.nameTextBox:AddInputHandler(self, self.NameTextBox_NameInputted)
	self.nameTextBox:AddKeyDownHandler(self, self.NameTextBox_KeyDownHandler)
	
	self.gameOverForm:AddChild(self.nameTextBox)
	self.gameOverForm:SetFocus(self.nameTextBox)
	
	SDLInstance.SetFocus(self.gameOverForm)
end

function GameOverState:NameTextBox_NameInputted(input)	
	self.currentPlayer.name = input;
	local scoreTable = dofile(SCORE_FILE_NAME)
	
	table.insert(scoreTable, {name=self.currentPlayer.name, score=self.currentPlayer.currentScore})
	local numberOfElements = table.maxn(scoreTable)
	
	if numberOfElements > 10 then
		local minimumValue, minimumValueKey = TableHelper.GetExtremeInnerTableByKeyValue(scoreTable, "score", 
					TableHelper.GenericComparison, TableHelper.Direction.ASCENDING)
					
		table.remove(scoreTable, minimumValueKey)
	end
	
	TableHelper.SortInnerTablesByKeyValue(scoreTable, "score", TableHelper.GenericComparison, TableHelper.Direction.DESCENDING)
		
	scoreFile = assert(io.open(SCORE_FILE_NAME, "w"))
	scoreFile:write("local scoreTable = {\n")
	
	for position, currentScore in ipairs(scoreTable) do
	
		scoreFile:write("{name = '", currentScore.name, "', score = ", currentScore.score, "}")
		if position ~= table.maxn(scoreTable) then
			scoreFile:write(",")
		end
		
		scoreFile:write("\n")
	end
	
	scoreFile:write("}")
	scoreFile:write("return scoreTable")
	
	assert(scoreFile:close())
	
	SDLInstance.FadeOutMusic(FADE_LENGTH);
	
	SDLInstance.PerformFadeEffect(NUMBER_OF_FADE_FRAMES, FADE_LENGTH, SDLFadeDirection.SDL_FD_OUT)
	SDLInstance.UnloadTrunk(GAME_0VER_TRUNK_NAME)
	
	SDLInstance.PlayMusic(GLOBAL_TRUNK_NAME, MENU_BACKGROUND_MUSIC, -1, MUSIC_FADE_LENGTH)
	EnterScoreTableState()	
end

function GameOverState:NameTextBox_KeyDownHandler(keySym) 
	if keySym ~= SDLKeySymbols.SDLK_RETURN and keySym <= 300 then
		SDLInstance.PlaySound(GAME_0VER_TRUNK_NAME, TEXT_BOX_KEY_PRESSED)
	end
end