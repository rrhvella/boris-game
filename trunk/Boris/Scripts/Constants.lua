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


--General

BLOCK_SIZE = 
{
	width = 22,
	height = 22
}


MAX_NUMBER_OF_LEVELS = 10

QUICK_TIME = 50
START_TIME = QUICK_TIME * (MAX_NUMBER_OF_LEVELS + 1)

MAIN_FPS = 60

--Effects

NUMBER_OF_FADE_FRAMES = 20
FADE_LENGTH = 500

LOOM_EFFECT_RATE_OF_INCREASE = 0.05
LOOM_EFFECT_MAX_SIZE = 2.0

--Forms

GAME_OVERLAY_COLOR = 
{
	r = 137,
	g = 137,
	b = 137,
	a = 225
}

GAME_OVER_POSITION = 
{
	x = 200,
	y = 150
}

SCORE_TABLE_LIST_NAME_POSITION = 
{
	x = 285,
	y = 118
}


SCORE_TABLE_LIST_SCORE_POSITION = 
{
	x = SCORE_TABLE_LIST_NAME_POSITION.x + 100,
	y = SCORE_TABLE_LIST_NAME_POSITION.y
}

SCORE_TABLE_LIST_Y_PADDING = 20

MENU_ITEMS_START_POSITION = 
{
	x = 350,
	y = 214
}

MENU_ITEMS_Y_PADDING = 30

--Images

require "Scripts/Locations"

MENU_BACKGROUND_IMAGE_NAME = "MainMenuScreen"
GAME_BACKGROUND_IMAGE_NAME = "Background"
GAME_OVER_BACKGROUND_IMAGE_NAME = "GameOverScreen"
PAUSE_BACKGROUND_IMAGE_NAME = "PauseScreen"
SCORE_TABLE_BACKGROUND_IMAGE_NAME = "ScoreTableScreen"

L_TETRA_BLOCK_IMAGE = "BlueBlock"
T_TETRA_BLOCK_IMAGE = "YellowBlock"
J_TETRA_BLOCK_IMAGE = "VioletBlock"
S_TETRA_BLOCK_IMAGE = "LightBlueBlock"
Z_TETRA_BLOCK_IMAGE = "RedBlock"
O_TETRA_BLOCK_IMAGE = "BrownBlock"
I_TETRA_BLOCK_IMAGE = "GreenBlock"

--Labels and Text Boxes

SCORE_LABEL_POSITION =
{
	x = 417,
	y = 177
}

LEVEL_LABEL_POSITION =
{
	x = 417,
	y = 210
}


NAME_TEXT_BOX_POSITION =
{
	x = 200,
	y = 204
}

NAME_TEXT_BOX_MAX_LENGTH = 10
NAME_TEXT_BOX_CURSOR_IMAGE = "Cursor"

COMMON_TEXT_COLOR = 
{
	r = 255,
	g = 255,
	b = 255,
}
--Panels

MAIN_PANEL_POSITION = 
{
	x = 35,
	y = 28
}

MAIN_PANEL_SIZE = 
{
	width = 12,
	height = 24
}

PREVIEW_PANEL_POSITION = 
{
	x = 349,
	y = 29
}

PREVIEW_PANEL_SIZE = 
{
	width = 4,
	height = 4
}

PAUSE_PANEL_POSITION = 
{
	x = GAME_OVER_POSITION.x,
	y = GAME_OVER_POSITION.y
}

--Tetrominoes

STARTING_POSITION = 
{
	x = MAIN_PANEL_POSITION.x + (6 * BLOCK_SIZE.width),
	y = MAIN_PANEL_POSITION.y
}

PREVIEW_PANEL_PLACEMENT_POSITION = 
{
	x = 2,
	y = 1
}

NUMBER_OF_TETROMINOES = 7

--External files

SCORE_FILE_NAME = "Data/top.dat"


--Music

MENU_BACKGROUND_MUSIC = "First Time in a Long Time"
MAIN_GAME_BACKGROUND_MUSIC = "First Time in a Long Time"
GAME_OVER_BACKGROUND_MUSIC = "First Time in a Long Time"

MUSIC_FADE_LENGTH = 1000

--Sound

MENU_OPTION_CHANGE_SOUND = "cowbell"
MENU_OPTION_SELECT_SOUND = "cowbell"

TURN_TETROMINO_SOUND = "cowbell"
LINE_REMOVED_SOUND = "cowbell" 
TETROMINO_REACHES_FLOOR_SOUND = "cowbell"

TEXT_BOX_KEY_PRESSED = "cowbell"