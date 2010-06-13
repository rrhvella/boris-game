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
require "Scripts/Enums"
local StringHelper = require "Scripts/StringHelperFunctions"

local ScoreTableState = {}

function EnterScoreTableState()
	ScoreTableState:CreateScoreTableState()
end

function ScoreTableState:CreateScoreTableState()		
	collectgarbage()
		
	SDLInstance.LoadTrunk(SCORE_TRUNK_NAME)
	
	self.scoreTableForm = SDLForm.New("Score Table", 0, 0, SCORE_TRUNK_NAME, SCORE_TABLE_BACKGROUND_IMAGE_NAME)
	self.scoreTableForm:AddKeyDownHandler(self, self.ScoreTableForm_KeyDown)
	
	local scoreTable = dofile(SCORE_FILE_NAME)
	self:GenerateScoreList(scoreTable);
	
	SDLInstance.SetFocus(self.scoreTableForm)	
	SDLInstance.PerformFadeEffect(NUMBER_OF_FADE_FRAMES, FADE_LENGTH, SDLFadeDirection.SDL_FD_IN)
end

function ScoreTableState:GenerateScoreList(scoreTable)
	for key, value in ipairs(scoreTable) do
		
		self.scoreTableForm:AddChild(SDLText.New("HighScoreNameLabel" .. key, SCORE_TABLE_LIST_NAME_POSITION.x, 
			SCORE_TABLE_LIST_NAME_POSITION.y + ((key - 1) * SCORE_TABLE_LIST_Y_PADDING), value.name, COMMON_FONT, COMMON_TEXT_COLOR))
		
		local scoreText = StringHelper.InsertThousandsSeperators(tostring(value.score), ',')
		
					
		self.scoreTableForm:AddChild(SDLText.New("HighScoreLabel" .. key, SCORE_TABLE_LIST_SCORE_POSITION.x, 
			SCORE_TABLE_LIST_SCORE_POSITION.y + ((key - 1) * SCORE_TABLE_LIST_Y_PADDING), scoreText, COMMON_FONT, COMMON_TEXT_COLOR))
	end
end

function ScoreTableState:ScoreTableForm_KeyDown(keySymbol)
	if keySymbol == SDLKeySymbols.SDLK_RETURN then
		SDLInstance.UnloadTrunk(SCORE_TRUNK_NAME)
		SDLInstance.PerformFadeEffect(NUMBER_OF_FADE_FRAMES, FADE_LENGTH, SDLFadeDirection.SDL_FD_OUT)
		
		EnterMainMenuState()
	end
end
