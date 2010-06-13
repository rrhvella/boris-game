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

require "Scripts/Enums"
require "Scripts/Constants"
require "Scripts/Player"
require "Scripts/GameOverState"

local StringHelper = require "Scripts/StringHelperFunctions"
local TableHelper = require "Scripts/TableHelperFunctions"

local GameState = {}

function EnterGameState()
	GameState:CreateGameState()
end

function GameState:CreateGameState()	
	collectgarbage()	
	
	SDLInstance.LoadTrunk(GAME_TRUNK_NAME)
	
	self.mainForm = SDLForm.New("Main", 0, 0, GAME_TRUNK_NAME, GAME_BACKGROUND_IMAGE_NAME)
	
	self.currentInterval = START_TIME
	
	self.moveTetraminoTimer = SDLTimer.New(self.currentInterval)
	self.moveTetraminoTimer:AddCycleCompleteHandler(self, self.MoveTetraminoTimer_CycleComplete)
	
	self.tetrominoes = dofile("Scripts/Tetrominoes.lua")
		
	self:CreateLabels()
	self:CreatePanels()
	
	self.quickTime = false
	
	self.currentPlayer = Player:New()
	
	self.mainForm:AddTimer(self.moveTetraminoTimer)
	self.mainForm:AddKeyDownHandler(GameState, self.MainForm_KeyDown)
	self.mainForm:AddKeyUpHandler(GameState, self.MainForm_KeyUp)
	
	self.mainForm:AddChild(self.levelLabel)	
	self.mainForm:AddChild(self.scoreLabel)
	self.mainForm:AddChild(self.mainPanel)
	self.mainForm:AddChild(self.previewPanel)
	
	SDLInstance.SetFocus(self.mainForm)
	
	math.randomseed(os.time())
	
	self.currentTetromino = self.tetrominoes[math.random(NUMBER_OF_TETROMINOES)]
	self.nextTetromino = self.tetrominoes[math.random(NUMBER_OF_TETROMINOES)]
	
	self:ShowPreviewAndCurrentTetromino()
	
	self.paused = false;
	
	SDLInstance.PlayMusic(GLOBAL_TRUNK_NAME, MAIN_GAME_BACKGROUND_MUSIC, -1, MUSIC_FADE_LENGTH)
	SDLInstance.PerformFadeEffect(NUMBER_OF_FADE_FRAMES, FADE_LENGTH, SDLFadeDirection.SDL_FD_IN)
end

--Content Creation

function GameState:CreateLabels()
	self.scoreLabel = SDLText.New("Score Label", SCORE_LABEL_POSITION.x, SCORE_LABEL_POSITION.y, 0, COMMON_FONT, COMMON_TEXT_COLOR)
	self.levelLabel = SDLText.New("Level Label", LEVEL_LABEL_POSITION.x, LEVEL_LABEL_POSITION.y, 1, COMMON_FONT, COMMON_TEXT_COLOR)	
end

function GameState:CreatePanels()
	self.pauseScreen = SDLComponent.New("Pause", PAUSE_PANEL_POSITION.x, PAUSE_PANEL_POSITION.y, 
									GAME_TRUNK_NAME, PAUSE_BACKGROUND_IMAGE_NAME)
	
	local mainPanelGrid = SDLSurfaceGrid.New("Main Panel Surface Grid", MAIN_PANEL_SIZE.width, MAIN_PANEL_SIZE.height, BLOCK_SIZE.width, 
							BLOCK_SIZE.height, 0, 0, AnchorPointEnum.SDL_SSP_UPPER_LEFT_CORNER)
						
	self.mainPanel = SDLSurfaceGridComponent.New("Main Panel", MAIN_PANEL_POSITION.x, MAIN_PANEL_POSITION.y, mainPanelGrid, 
							SDLClearingMethod.SDL_CM_BOUNDING_BOX)

	local previewPanelGrid = SDLSurfaceGrid.New("Preview Panel Surface Grid", PREVIEW_PANEL_SIZE.width, PREVIEW_PANEL_SIZE.height, BLOCK_SIZE.width, 
							BLOCK_SIZE.height, 0, 0, AnchorPointEnum.SDL_SSP_UPPER_LEFT_CORNER)
		
	self.previewPanel = SDLSurfaceGridComponent.New("Preview Panel", PREVIEW_PANEL_POSITION.x, PREVIEW_PANEL_POSITION.y, 
							previewPanelGrid, SDLClearingMethod.SDL_CM_BOUNDING_BOX)
end
	
--Events

function GameState:MoveTetraminoTimer_CycleComplete()
	self.currentTetromino:AddToTop(BLOCK_SIZE.height)
		
	if self.mainPanel:SurfacesCollide(self.currentTetromino, true) then
		self.currentTetromino:AddToTop(-BLOCK_SIZE.height)
		
		if self.currentTetromino:GetLeft() ~= STARTING_POSITION.x or self.currentTetromino:GetTop() ~= STARTING_POSITION.y then
			self:PlaceCurrentTetrominoInMainPanel()
			
			local completeLineIndexes = self.mainPanel:GetCompleteLineIndexes()
			self:RemoveCompletedLinesAndUpdateScore(completeLineIndexes)
			
			self:NextTetromino()
		else
			SDLInstance.UnloadTrunk(GAME_TRUNK_NAME)
			EnterGameOverState(self.currentPlayer)
		end	
	end
end

function GameState:MainForm_KeyDown(keySymbol)
	if keySymbol == SDLKeySymbols.SDLK_p then
		self:TogglePause()
	end

	if keySymbol == SDLKeySymbols.SDLK_DOWN then
		if self.quickTime == false then
			self.moveTetraminoTimer:SetInterval(QUICK_TIME)
			self.quickTime = true
		end
	end

	if keySymbol == SDLKeySymbols.SDLK_UP then
		self.currentTetromino:Next()
		
		if self.mainPanel:SurfacesCollide(self.currentTetromino, true) then
			self.currentTetromino:Previous()	
		else
			SDLInstance.PlaySound(GAME_TRUNK_NAME, TURN_TETROMINO_SOUND)
		end	
	end		
		
	if keySymbol == SDLKeySymbols.SDLK_RIGHT then
		self.currentTetromino:AddToLeft(BLOCK_SIZE.width)
		
		if self.mainPanel:SurfacesCollide(self.currentTetromino, true) then
			self.currentTetromino:AddToLeft(-BLOCK_SIZE.width)	
		end	
	end
	
	if keySymbol == SDLKeySymbols.SDLK_LEFT then
		self.currentTetromino:AddToLeft(-BLOCK_SIZE.width)
		
		if self.mainPanel:SurfacesCollide(self.currentTetromino, true) then
			self.currentTetromino:AddToLeft(BLOCK_SIZE.width)	
		end	
	end	
end

function GameState:MainForm_KeyUp(keySymbol)	
	if keySymbol == SDLKeySymbols.SDLK_DOWN then
		self.moveTetraminoTimer:SetInterval(self.currentInterval)
		self.quickTime =  false
	end	
end

--Logical Functions
	
function GameState:ShowPreviewAndCurrentTetromino()
	self.currentTetromino:First()
	self.nextTetromino:First()
	
	self.mainForm:AddChild(self.currentTetromino)
	
	self.previewPanel:ClearCurrentSurfaces(nil)	
	self.previewPanel:ReplaceCurrentSurfaces(self.nextTetromino, nil, PREVIEW_PANEL_PLACEMENT_POSITION.x, 
				PREVIEW_PANEL_PLACEMENT_POSITION.y, false) 
		
	self.currentTetromino:SetLeft(STARTING_POSITION.x)
	self.currentTetromino:SetTop(STARTING_POSITION.y)
end

function GameState:NextTetromino()
	self.mainForm:RemoveChild(self.currentTetromino)
	
	self.currentTetromino = self.nextTetromino
	self.nextTetromino = self.tetrominoes[math.random(NUMBER_OF_TETROMINOES)]
	
	self:ShowPreviewAndCurrentTetromino()
end
	
	
function GameState:PlaceCurrentTetrominoInMainPanel()
	local positionToCopyToX = (self.currentTetromino:GetLeft() - MAIN_PANEL_POSITION.x)/BLOCK_SIZE.width
	local positionToCopyToY = (self.currentTetromino:GetTop() - MAIN_PANEL_POSITION.y)/BLOCK_SIZE.height
	
	self.mainPanel:ReplaceCurrentSurfaces(self.currentTetromino, nil, positionToCopyToX, positionToCopyToY, false)	
end

function GameState:RemoveCompletedLinesAndUpdateScore(completeLineIndexes)	
	local linesCompleted = table.maxn(completeLineIndexes)
	
	if linesCompleted > 0 then
		SDLInstance.PlaySound(GAME_TRUNK_NAME, LINE_REMOVED_SOUND)
		local portionsToFlash = {}
		
		for _,currentIndex in ipairs(completeLineIndexes) do
		
			local xToFlash = MAIN_PANEL_POSITION.x
			local yToFlash = MAIN_PANEL_POSITION.y + currentIndex * BLOCK_SIZE.height
			
			local widthToFlash = MAIN_PANEL_SIZE.width * BLOCK_SIZE.width
			local heightToFlash = BLOCK_SIZE.height
			
			table.insert(portionsToFlash, {x = xToFlash, y = yToFlash, width = widthToFlash, height = heightToFlash})
						
			local xToClear = 0
			local yToClear = currentIndex
			
			local widthToClear = MAIN_PANEL_SIZE.width
			local heightToClear = 1
			
			self.mainPanel:ClearCurrentSurfaces({x = xToClear, y = yToClear, width = widthToClear, height = heightToClear})
		end
		
		for _,currentIndex in ipairs(completeLineIndexes) do
		
			local xToCopy = 0
			local yToCopy = 0
			
			local widthToCopy = MAIN_PANEL_SIZE.width
			local heightToCopy = currentIndex
			
			local xToCopyTo = 0
			local yToCopyTo = 1
			
			self.mainPanel:MoveCurrentSurfaces({x = xToCopy, y = yToCopy, width = widthToCopy, height = heightToCopy}, xToCopyTo, yToCopyTo)
		end
		
		SDLInstance.PerformFlashEffect(portionsToFlash, 4, 3);
		
		self.currentPlayer.numberOfCombos = self.currentPlayer.numberOfCombos + 1
		
		if SDLInstance.KeyIsPressed(SDLKeySymbols.SDLK_DOWN) then
			self.moveTetraminoTimer:SetInterval(QUICK_TIME)
			self.quickTime = true
		else
			self.moveTetraminoTimer:SetInterval(self.currentInterval)
			self.quickTime = false			
		end
		
		local newLevelGained = self.currentPlayer:UpdateScore(linesCompleted)
		local newScore = StringHelper.InsertThousandsSeperators(tostring(self.currentPlayer.currentScore), ',')
		
		self.scoreLabel:SetText(newScore)

		if newLevelGained then
			self.currentInterval = QUICK_TIME * (MAX_NUMBER_OF_LEVELS - (self.currentPlayer.currentLevel - 1))

			if not self.quickTime then
				self.moveTetraminoTimer:SetInterval(self.currentInterval)
			end
			
			self.levelLabel:SetText(self.currentPlayer.currentLevel)
		end
	else
		SDLInstance.PlaySound(GAME_TRUNK_NAME, TETROMINO_REACHES_FLOOR_SOUND)
		self.currentPlayer.numberOfCombos = 0
	end		
end

function GameState:TogglePause()
	if not self.paused then
		SDLInstance.PerformOverlayEffect(GAME_OVERLAY_COLOR)	
		self.mainForm:AddChild(self.pauseScreen);
	
		self.moveTetraminoTimer:Pause()
		self.paused = true
	else
		self.moveTetraminoTimer:Continue()
		self.mainForm:RemoveChild(self.pauseScreen);
		
		self.paused = false
	end
end