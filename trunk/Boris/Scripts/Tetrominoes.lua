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

local tetraLSurfaceGrids =
{	
	SDLSurfaceGrid.New(
		"LTetra",
		{
			{1, 1, 1},
			{1, 0, 0}
		},
		GAME_TRUNK_NAME,
		L_TETRA_BLOCK_IMAGE,
		1, 0,
		AnchorPointEnum.SDL_SSP_UPPER_LEFT_CORNER
	),
	SDLSurfaceGrid.New(
		"LTetra1",
		{
			{1, 1},
			{0, 1},
			{0, 1}
		},
		GAME_TRUNK_NAME,
		L_TETRA_BLOCK_IMAGE,
		1, 1,
		AnchorPointEnum.SDL_SSP_UPPER_LEFT_CORNER
	),
	SDLSurfaceGrid.New(
		"LTetra2",
		{
			{0, 0, 1},
			{1, 1, 1}
		},
		GAME_TRUNK_NAME,
		L_TETRA_BLOCK_IMAGE,
		1, 1,
		AnchorPointEnum.SDL_SSP_UPPER_LEFT_CORNER
	),
	SDLSurfaceGrid.New(
		"LTetra3",
		{
			{1, 0},
			{1, 0},
			{1, 1}
		},
		GAME_TRUNK_NAME,
		L_TETRA_BLOCK_IMAGE,
		0, 1,
		AnchorPointEnum.SDL_SSP_UPPER_LEFT_CORNER
	)
}

local tetraL = SDLSurfaceGridComponent.New("L", 0, 0, tetraLSurfaceGrids, SDLClearingMethod.SDL_CM_PRECISE);

local tetraTSurfaceGrids =
{	
	SDLSurfaceGrid.New(
		"TTetra",
		{
			{1, 1, 1},
			{0, 1, 0}
		},
		GAME_TRUNK_NAME,
		T_TETRA_BLOCK_IMAGE,
		1, 0,
		AnchorPointEnum.SDL_SSP_UPPER_LEFT_CORNER
	),
	SDLSurfaceGrid.New(
		"TTetra1",
		{
			{0, 1},
			{1, 1},
			{0, 1}
		},
		GAME_TRUNK_NAME,
		T_TETRA_BLOCK_IMAGE,
		1, 1,
		AnchorPointEnum.SDL_SSP_UPPER_LEFT_CORNER
	),
	SDLSurfaceGrid.New(
		"TTetra2",
		{
			{0, 1, 0},
			{1, 1, 1}
		},
		GAME_TRUNK_NAME,
		T_TETRA_BLOCK_IMAGE,
		1, 1,
		AnchorPointEnum.SDL_SSP_UPPER_LEFT_CORNER
	),
	SDLSurfaceGrid.New(
		"TTetra3",
		{
			{1, 0},
			{1, 1},
			{1, 0}
		},
		GAME_TRUNK_NAME,
		T_TETRA_BLOCK_IMAGE,
		0, 1,
		AnchorPointEnum.SDL_SSP_UPPER_LEFT_CORNER
	)
}

local tetraT = SDLSurfaceGridComponent.New("T", 0, 0, tetraTSurfaceGrids, SDLClearingMethod.SDL_CM_PRECISE);

local tetraJSurfaceGrids =
{	
	SDLSurfaceGrid.New(
		"JTetra",
		{
			{1, 1, 1},
			{0, 0, 1}
		},
		GAME_TRUNK_NAME,
		J_TETRA_BLOCK_IMAGE,
		1, 0,
		AnchorPointEnum.SDL_SSP_UPPER_LEFT_CORNER
	),
	SDLSurfaceGrid.New(
		"JTetra1",
		{
			{0, 1},
			{0, 1},
			{1, 1}
		},
		GAME_TRUNK_NAME,
		J_TETRA_BLOCK_IMAGE,
		1, 1,
		AnchorPointEnum.SDL_SSP_UPPER_LEFT_CORNER
	),
	SDLSurfaceGrid.New(
		"JTetra2",
		{
			{1, 0, 0},
			{1, 1, 1}
		},
		GAME_TRUNK_NAME,
		J_TETRA_BLOCK_IMAGE,
		1, 1,
		AnchorPointEnum.SDL_SSP_UPPER_LEFT_CORNER
	),
	SDLSurfaceGrid.New(
		"JTetra3",
		{
			{1, 1},
			{1, 0},
			{1, 0}
		},
		GAME_TRUNK_NAME,
		J_TETRA_BLOCK_IMAGE,
		0, 1,
		AnchorPointEnum.SDL_SSP_UPPER_LEFT_CORNER
	)
}

local tetraJ = SDLSurfaceGridComponent.New("J", 0, 0, tetraJSurfaceGrids, SDLClearingMethod.SDL_CM_PRECISE);

local tetraSSurfaceGrids =
{	
	SDLSurfaceGrid.New(
		"STetra",
		{
			{0, 1, 1},
			{1, 1, 0}
		},
		GAME_TRUNK_NAME,
		S_TETRA_BLOCK_IMAGE,
		1, 0,
		AnchorPointEnum.SDL_SSP_UPPER_LEFT_CORNER
	),
	SDLSurfaceGrid.New(
		"STetra1",
		{
			{1, 0},
			{1, 1},
			{0, 1}
		},
		GAME_TRUNK_NAME,
		S_TETRA_BLOCK_IMAGE,
		0, 1,
		AnchorPointEnum.SDL_SSP_UPPER_LEFT_CORNER
	)
}

local tetraS = SDLSurfaceGridComponent.New("S", 0, 0, tetraSSurfaceGrids, SDLClearingMethod.SDL_CM_PRECISE);

local tetraZSurfaceGrids =
{	
	SDLSurfaceGrid.New(
		"ZTetra",
		{
			{1, 1, 0},
			{0, 1, 1}
		},
		GAME_TRUNK_NAME,
		Z_TETRA_BLOCK_IMAGE,
		1, 0,
		AnchorPointEnum.SDL_SSP_UPPER_LEFT_CORNER
	),
	SDLSurfaceGrid.New(
		"ZTetra1",
		{
			{0, 1},
			{1, 1},
			{1, 0}
		},
		GAME_TRUNK_NAME,
		Z_TETRA_BLOCK_IMAGE,
		1, 1,
		AnchorPointEnum.SDL_SSP_UPPER_LEFT_CORNER
	)
}

local tetraZ = SDLSurfaceGridComponent.New("Z", 0, 0, tetraZSurfaceGrids, SDLClearingMethod.SDL_CM_PRECISE);	

local tetraOSurfaceGrids =
{	
	SDLSurfaceGrid.New(
		"OTetra",
		{
			{1, 1},
			{1, 1}
		},
		GAME_TRUNK_NAME,
		O_TETRA_BLOCK_IMAGE,
		1, 0,
		AnchorPointEnum.SDL_SSP_UPPER_LEFT_CORNER
	)
}

local tetraO = SDLSurfaceGridComponent.New("O", 0, 0, tetraOSurfaceGrids, SDLClearingMethod.SDL_CM_BOUNDING_BOX);	

local tetraISurfaceGrids =
{	
	SDLSurfaceGrid.New(
		"ITetra",
		{
			{1, 1, 1, 1}
		},
		GAME_TRUNK_NAME,
		I_TETRA_BLOCK_IMAGE,
		2, 0,
		AnchorPointEnum.SDL_SSP_UPPER_LEFT_CORNER
	),
	SDLSurfaceGrid.New(
		"ITetra1",
		{	
			{1},
			{1},
			{1},
			{1}
		},
		GAME_TRUNK_NAME,
		I_TETRA_BLOCK_IMAGE,
		0, 2,
		AnchorPointEnum.SDL_SSP_UPPER_LEFT_CORNER
	)
}

local tetraI = SDLSurfaceGridComponent.New("I", 0, 0, tetraISurfaceGrids, SDLClearingMethod.SDL_CM_BOUNDING_BOX);

local tetrominoes = 
{
	tetraL,
	tetraT,
	tetraJ,
	tetraS,
	tetraZ,
	tetraO,
	tetraI

}

return tetrominoes	