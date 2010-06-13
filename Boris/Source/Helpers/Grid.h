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

#ifndef GRID_H
#define GRID_H

#include <sstream>

#include <Helpers/ApplicationException.h>

using namespace std;

namespace Helpers
{
	class GridException: public ApplicationException
	{
		public:
			GridException(const char* errorMessage):
				ApplicationException(errorMessage)
				{
				}
	};

	template <class CellClass>
	class Grid
	{
		private:
			CellClass** grid;
			unsigned int width;
			unsigned int height;

			unsigned int totalSize;

		public:
			Grid(unsigned int width, unsigned int height)
			{
				this->width = width;
				this->height = height;

				this->totalSize = width * height;

				this->grid = new CellClass*[totalSize];

				for(unsigned int i = 0; i < totalSize; i++)
				{
					this->grid[i] = NULL;
				}
			}

			Grid(const Grid<CellClass>& source)
			{
				this->width = source.GetWidth();
				this->height = source.GetHeight();

				this->totalSize = this->width * this->height;

				for(int i = 0; i < totalSize; i++)
				{
					if(source.grid[i] == NULL)
					{
						this->grid[i] = source.grid[i];
					}
					else
					{
						*(this->grid[i]) = *source.grid[i];
					}
				}
			}

			CellClass Get(unsigned int x, unsigned int y) const
			{
				if(x >= width || y >= height)
				{
					stringstream error;

					error << "Attempted to read from out-of-bounds location : ";
					error  << "(" << x << ", " << y << "). When legal bounds within ";
					error  << "(" << width << ", " << height << ").";

					throw GridException(error.str().c_str());
				}

				CellClass* result = grid[y*width + x];

				if(result == NULL)
				{
					stringstream error;

					error << "Attempted from null location.";

					throw GridException(error.str().c_str());
				}

				return *grid[y*width + x];
			}

			void Set(unsigned int x, unsigned int y, CellClass newObject)
			{
				if(x >= width || y >= height)
				{
					stringstream error;

					error << "Attempted to write to out-of-bounds location : ";
					error  << "(" << x << ", " << y << "). When legal bounds within ";
					error  << "(" << width << ", " << height << ").";

					throw GridException(error.str().c_str());
				}

				CellClass* currentValue = grid[y*width + x];

				if(currentValue == NULL)
				{
					grid[y*width + x] = new CellClass(newObject);
				}
				else
				{
					*grid[y*width + x] = newObject;
				}
			}

			int GetWidth() const
			{
				return width;
			}

			int GetHeight() const
			{
				return height;
			}

			~Grid()
			{
				for(unsigned int i = 0; i < totalSize; i++)
				{
					if(grid[i] != NULL)
					{
						delete grid[i];
					}
				}

				delete [] grid;
			}
	};
}

#endif
