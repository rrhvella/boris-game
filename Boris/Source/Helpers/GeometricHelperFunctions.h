/*
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
*/

#ifndef GEOMETRIC_HELPER_FUNCTIONS_H
#define GEOMETRIC_HELPER_FUNCTIONS_H

#include <vector>
#include <iterator>

#include <Helpers/GeometricStructures2D.h>

using namespace std;

namespace Helpers
{
	/*
		Function: GetBoundingRectangle

		Template:
			NumericType - The number type for the size and position values.

			Example:
				int, float, long, double etc..

		Parameters:
			rectanglesToBound - The list of dimensions which will be bound.

		Returns:
			A bounding rectangle for the dimensions in rectanglesToBound. When
			drawn on a surface with those dimensions, the bounding rectangle
			perfectly contains them.

		Example:
			If the following rectangles are passed (Format {x, y, width, height}):

			{1, 0, 2, 2},
			{2, -2, 4, 3}

			Then their bounding rectangle (where x2 and y2 are x + width, and y + height respectively
			for each dimension) is

			{
				1 - Minimum x1,
				-2 - Minimum y1,
				5 - Maximum x2 - Minimum x1,
				4 - Maximum y2 - Minimum y1
			}

	*/
	template<class NumericType>
	static Dimensions2D<NumericType> GetBoundingRectangle(const vector< Dimensions2D<NumericType> >& rectanglesToBound)
	{
		NumericType minX1 = rectanglesToBound[0].position.x;
		NumericType minY1 = rectanglesToBound[0].position.y;

		NumericType maxX2 = rectanglesToBound[0].position.x;
		NumericType maxY2 = rectanglesToBound[0].position.y;

		for(typename vector< Dimensions2D<NumericType> >::const_iterator currentRectangle = rectanglesToBound.begin();
			currentRectangle != rectanglesToBound.end(); currentRectangle++)
		{
			NumericType currentPortionX2 = currentRectangle->position.x + currentRectangle->size.width;
			NumericType currentPortionY2 = currentRectangle->position.y + currentRectangle->size.height;

			if(currentPortionX2 > maxX2)
			{
				maxX2 = currentPortionX2;
			}

			if(currentPortionY2 > maxY2)
			{
				maxY2 = currentPortionY2;
			}

			if(currentRectangle->position.x < minX1)
			{
				minX1 = currentRectangle->position.x;
			}

			if(currentRectangle->position.y < minY1)
			{
				minY1 = currentRectangle->position.y;
			}

		}

		return  Dimensions2D<NumericType>(minX1, minY1, maxX2 - minX1, maxY2 - minY1);
	}
}
#endif
