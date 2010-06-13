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

#ifndef GEOMETRIC_STRUCTURES_2D_H
#define GEOMETRIC_STRUCTURES_2D_H

#include <string>
#include <sstream>

using std::string;
using std::stringstream;

namespace Helpers
{
	/*
		Class: Vector2D

		Specifies an offset, or a coordinate (x, y).

		Template:
			NumericType - The number type for the x, y values

			Example:
				int, float, long, double etc..

		Properties:
			x - The x offset to the origin.
			y - The y offset to the origin.
	*/
	template<class NumericType>
	struct Vector2D
	{
		NumericType x;
		NumericType y;

		/*
			Constructor: Vector2D

			Creates an origin vector (0, 0).
		*/
		Vector2D()
		{
			this->x = 0;
			this->y = 0;
		}

		/*
			Constructor: Vector2D

			Parameters:
					x,
					y
		*/
		Vector2D(NumericType x, NumericType y)
		{
			this->x = x;
			this->y = y;
		}

		/*
			Function: ToString

			Returns the vector as a string in the following form: ([x], [y])

		*/
		string ToString() const
		{
			stringstream vectorString;

			vectorString << "(" << x << ", " << y << ")";

			return vectorString.str();
		}

		/*
			Operators:

			+= Vector2D,
			-= Vector2D,
			Vector2D + Vector2D,
			Vector2D - Vector2D

		*/
		Vector2D<NumericType>& operator+=(const Vector2D<NumericType>& v2)
		{
			this->x += v2.x;
			this->y += v2.y;

			return(*this);
		}

		Vector2D<NumericType>& operator-=(const Vector2D<NumericType>& v2)
		{
			this->x -= v2.x;
			this->y -= v2.y;

			return(*this);
		}
	};


	template<class NumericType>
	Vector2D<NumericType> operator+(const Vector2D<NumericType>& v1, const Vector2D<NumericType>& v2)
	{
		Vector2D<NumericType> result = v1;

		result += v2;

		return result;
	}


	template<class NumericType>
	Vector2D<NumericType> operator-(const Vector2D<NumericType>& v1, const Vector2D<NumericType>& v2)
	{
		Vector2D<NumericType> result = v1;

		result -= v2;

		return result;
	}


	/*
		Class: Bounds2D

		Specifies a size, or boundary (width, height)

		Template:
			NumericType - The number type for the width, height values

			Example:
				int, float, long, double etc..


		Properties:
			width - The distance along the x-axis.
			height - The distance along the y-axis.
	*/
	template<class NumericType>
	struct Bounds2D
	{
		NumericType width;
		NumericType height;

		/*
			Constructor: Bounds2D

			Creates a zero bounds object (0, 0).
		*/
		Bounds2D()
		{
			width = 0;
			height = 0;
		}


		/*
			Constructor: Bounds2D

			Parameters:
				width,
				height
		*/
		Bounds2D(NumericType width, NumericType height)
		{
			this->width = width;
			this->height = height;
		}

		/*
			Function: ToString

			Returns the bounds as a string in the following form: ([width]*[height])
		*/
		string ToString() const
		{
			stringstream boundsString;

			boundsString << "(" << width << "*" << height << ")";

			return boundsString.str();
		}

		/*
			Operators:

			+= Bounds2D,
			-= Bounds2D,
			== Bounds2D,
			!= Bounds2D,
			/= scalar - Divides both the width and height by a scalar,
			Bounds2D + Bounds2D,
			Bounds2D - Bounds2D

		*/
		Bounds2D<NumericType>& operator+=(const Bounds2D<NumericType>& b2)
		{
			this->width += b2.width;
			this->height += b2.height;

			return(*this);
		}

		Bounds2D<NumericType>& operator-=(const Bounds2D<NumericType>& b2)
		{
			this->width -= b2.width;
			this->height -= b2.height;

			return(*this);
		}


		Bounds2D<NumericType>& operator/=(NumericType scalar)
		{
			this->width /= scalar;
			this->height /= scalar;

			return(*this);
		}

	};


	template<class NumericType>
	bool operator==(const Bounds2D<NumericType>& b1, const Bounds2D<NumericType>& b2)
	{
		return b1.width == b2.width && b1.height == b2.height;
	}

	template<class NumericType>
	bool operator!=(const Bounds2D<NumericType>& b1, const Bounds2D<NumericType>& b2)
	{
		return b1.width != b2.width || b1.height != b2.height;
	}

	template<class NumericType>
	Bounds2D<NumericType> operator+(const Bounds2D<NumericType>& b1, const Bounds2D<NumericType>& b2)
	{
		Bounds2D<NumericType> result = b1;

		result += b2;

		return result;
	}


	template<class NumericType>
	Bounds2D<NumericType> operator-(const Bounds2D<NumericType>& b1, const Bounds2D<NumericType>& b2)
	{
		Bounds2D<NumericType> result = b1;

		result -= b2;

		return result;
	}

	/*
		Class: Dimensions2D

		Specifies a size, and a position (x, y, width, height)

		Template:
			NumericType - The number type for the size, and position.

		Properties:
			size: A Bounds2D object containing the width and height.
			position:  A Vector2D object containing position (x, y).
	*/
	template<class NumericType>
	struct Dimensions2D
	{
		Bounds2D<NumericType> size;
		Vector2D<NumericType> position;

		Dimensions2D()
		{
			this->position = Vector2D<NumericType>();
			this->size = Bounds2D<NumericType>();
		}

		/*
			Constructor: Dimensions2D

			Parameters:
				x,
				y,
				width,
				height
		*/
		Dimensions2D(NumericType x, NumericType y, NumericType width, NumericType height)
		{
			this->position = Vector2D<NumericType>(x, y);
			this->size = Bounds2D<NumericType>(width, height);
		}

		/*
			Constructor: Dimensions2D

			Parameters:
				position,
				size
		*/
		Dimensions2D(const Vector2D<NumericType>& position, const Vector2D<NumericType>& size)
		{
			this->position = position;
			this->size = size;
		}

	};
}
#endif
