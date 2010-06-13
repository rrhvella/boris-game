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
require "Scripts/ScoreTableState"
require "Scripts/GameState"

local MainMenuState = {}

function EnterMainMenuState()
	MainMenuState:CreateMainMenuState()
end

function MainMenuState:CreateMainMenuState()		
	SDLInstance.LoadTrunk(MENU_TRUNK_NAME)
	
	self.mainMenuForm = SDLForm.New("Main Menu", 0, 0, MENU_TRUNK_NAME, MENU_BACKGROUND_IMAGE_NAME)
	self.mainMenuForm:AddKeyDownHandler(self, self.MainMenuForm_KeyDown)
	
	self.optionsTable = {CreateMenuItem("New Game", self.NewGame), CreateMenuItem("High Scores", self.HighScores), 
		CreateMenuItem("Exit Game", self.Exit)}
		
	self.currentOption = 1
	
	self:GenerateMenuList()
	
	self.textEffect = LoomEffect.New(LOOM_EFFECT_RATE_OF_INCREASE, LOOM_EFFECT_MAX_SIZE)
	self:SetCurrentOptionEffect(self.textEffect)
	
	SDLInstance.SetFocus(self.mainMenuForm)	
	SDLInstance.PerformFadeEffect(NUMBER_OF_FADE_FRAMES, FADE_LENGTH, SDLFadeDirection.SDL_FD_IN)
end

function CreateMenuItem(label, functionToCall)
	local menuItem = {}
	
	menuItem.label = label
	menuItem.functionToCall = functionToCall
	
	return menuItem
end

function MainMenuState:GenerateMenuList()
	for key, value in ipairs(self.optionsTable) do
		local newTextBox = SDLText.New("MenuLabel" .. key, MENU_ITEMS_START_POSITION.x, MENU_ITEMS_START_POSITION.y + 
			((key - 1) * MENU_ITEMS_Y_PADDING), value.label, COMMON_FONT, COMMON_TEXT_COLOR)
		
		value.textBox = newTextBox
		
		self.mainMenuForm:AddChild(value.textBox)
	end
end

function MainMenuState:MainMenuForm_KeyDown(keySymbol)
	if keySymbol == SDLKeySymbols.SDLK_DOWN then
		SDLInstance.PlaySound(MENU_TRUNK_NAME, MENU_OPTION_CHANGE_SOUND)
		self:SetCurrentOptionEffect(nil)
		self:MoveOptionCursor(1)
		self:SetCurrentOptionEffect(self.textEffect)
	elseif keySymbol == SDLKeySymbols.SDLK_UP then  
		SDLInstance.PlaySound(MENU_TRUNK_NAME, MENU_OPTION_CHANGE_SOUND)
		self:SetCurrentOptionEffect(nil)
		self:MoveOptionCursor(-1)
		self:SetCurrentOptionEffect(self.textEffect)
	elseif keySymbol == SDLKeySymbols.SDLK_RETURN then  
		SDLInstance.PlaySound(GLOBAL_TRUNK_NAME, MENU_OPTION_SELECT_SOUND)
		self.optionsTable[self.currentOption].functionToCall(self)
	end
end

function MainMenuState:NewGame()
	SDLInstance.FadeOutMusic(FADE_LENGTH);
		
	self:ExitState()
	EnterGameState()
end

function MainMenuState:HighScores()
	self:ExitState()
	EnterScoreTableState()
end

function MainMenuState:Exit()
	SDLInstance.FadeOutMusic(FADE_LENGTH);
	
	self:ExitState()
	SDLInstance.Quit()
end


function MainMenuState:ExitState()
	SDLInstance.UnloadTrunk(MENU_TRUNK_NAME)	
	SDLInstance.PerformFadeEffect(NUMBER_OF_FADE_FRAMES, FADE_LENGTH, SDLFadeDirection.SDL_FD_OUT)
end

function MainMenuState:SetCurrentOptionEffect(effect)
	self.optionsTable[self.currentOption].textBox:SetEffect(effect)
end

function MainMenuState:MoveOptionCursor(offset)
	if self.currentOption + offset > table.maxn(self.optionsTable) or self.currentOption + offset < 1 then
		self.currentOption = offset < 1 and table.maxn(self.optionsTable) or 1
	else
		self.currentOption = self.currentOption + offset
	end	
end


