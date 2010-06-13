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

require "Scripts/Object"
require "Scripts/Constants"

Player = Object:New
{
	name = "",
	currentScore = 0,
	currentLevel = 1,
	numberOfCombos = 0
}

function Player:UpdateScore(numberOfLinesCleared)
	if numberOfLinesCleared > 0 then
		self.currentScore = self.currentScore + (((2 ^ numberOfLinesCleared) * 100) * self.currentLevel * self.numberOfCombos)
	
	
		if self.currentLevel < MAX_NUMBER_OF_LEVELS then
			local nextLevelRequirement = (self.currentLevel ^ 2) * 1000

			if self.currentScore >= nextLevelRequirement then
				self.currentLevel = self.currentLevel + 1
				
				return true --Player has gained a level
			end
		end
	end

	return false --Player has not gained a level
end