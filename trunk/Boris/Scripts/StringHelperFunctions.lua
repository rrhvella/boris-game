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

local Module = {}


function Module.InsertThousandsSeperators(stringToCopy, thousandSeperator)
	local gsubBegin = stringToCopy:len()%3
	
	if gsubBegin == 0 then
		gsubBegin = 3
	end
	
	local stringPortionToIgnore = stringToCopy:sub(1, gsubBegin)
	local stringPortionToConvert = stringToCopy:sub(gsubBegin + 1, stringToCopy:len())	

	local stringToReturn = stringPortionToIgnore .. stringPortionToConvert:gsub("(%d%d%d)", thousandSeperator .. "%1")
	
	return stringToReturn	
end

return Module