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
local LogicHelper = require "Scripts/LogicHelperFunctions"

local Module = {}

--Comparison Types

Module.Direction = 
{
	ASCENDING = false,
	DESCENDING = true
}

Module.BaseComparer = Object:New{
	direction = Module.Direction.ASCENDING,
	CompareFunction = nil
}

function Module.BaseComparer:Compare(value1, value2) 
	return LogicHelper.XNOR(self.CompareFunction(value1, value2),  self.direction)
end

Module.ListComparer = Module.BaseComparer:New{
	keyToCompare = nil
}

function Module.ListComparer:Compare(table1, table2)		
	return Module.BaseComparer.Compare(self, table1[self.keyToCompare], table2[self.keyToCompare])
end


--Comparison Functions

function Module.GenericComparison(value1, value2)
	return value1 > value2
end

--Extreme Value Functions (Minimum/Maximum)

function Module.GetExtremeInnerTableByKeyValue(list, keyToCompare, compareFunction, direction)
	local tableComparer = Module.ListComparer:New()
	
	tableComparer.direction = direction
	tableComparer.CompareFunction = compareFunction
	tableComparer.keyToCompare = keyToCompare
	
	return Module.GetExtremeValue(list, tableComparer)	
end

function Module.GetExtremeValue(list, comparer)
	local extremeValue = list[1]
	local extremeValueKey = 1
	
	for key, value in ipairs(list) do
		if comparer:Compare(value, extremeValue) then
			extremeValue = value
			extremeValueKey = key
		end
	end
	
	return extremeValue, extremeValueKey
end

--Sorting functions

function Module.SortInnerTablesByKeyValue(list, keyToCompare, compareFunction, direction)
	local tableComparer = Module.ListComparer:New()
	
	tableComparer.direction = direction
	tableComparer.CompareFunction = compareFunction
	tableComparer.keyToCompare = keyToCompare
	
	Module.SortValues(list, tableComparer)
end

function Module.SortValues(list, comparer)
	table.sort(list, function(value1, value2) return comparer:Compare(value1, value2) end)
end

return Module