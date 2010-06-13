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

#ifndef FILE_SYSTEM_H
#define	FILE_SYSTEM_H

#include <string>
#include <sstream>

#include <Helpers/ApplicationException.h>
#include <Helpers/StringHelperFunctions.h>
#include <Helpers/IUncopyable.h>

//Platform Specific Include files
#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #undef UNICODE
    #include <windows.h>
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <dirent.h>
    #include <errno.h>
    #include <string.h>
    #include <stdio.h>
#endif


using namespace std;
/*
File: DirectoryTraverser.h

Contains the classes FileInfo, and Directory, which are wrappers to OS specific file access functions.
*/
namespace Helpers
{
	//External constant declarations
	extern string DIRECTORY_TYPE;
	extern string NO_FILE_TYPE;

	/*
	Class: FileSystemException

	General exception to indicate that the error occured while accessing the OS filesystem.
	*/
	class FileSystemException: public ApplicationException
	{
		public:
			/*
			Constructor: FileSystemException.

			Parameters:
				errorMessage - The errorMessage you would like to the display to the user.
			*/
			FileSystemException(const char* errorMessage);
	};

	/*
	Class: FileInfo

	This is a container class for information pertaining to a file,
	which may be either an actual file, or a directory.

	*/
	class FileInfo
	{
		protected:
			string name;
			string type;
			string location;

			string path;


		public:

			/*
			Constructor: FileSystemException.

			Parameters:
				name - The name of the file, before the last ".".

				type - The type of the file, after the last ".".
					 h  As a standard the Constant DIRECTORY_TYPE should
					   be passed if a file is a directory. And NO_FILE_TYPE
					   should be passed if it has no type.

				location - The location of the file.
			*/
			FileInfo(const string& name, const string& type, const string& location);
			/*
				Function: GetName.

				Get the name of the file, without the path, or the type.

				Example:

				"myFile" from "C:\myFile.dat"
			*/
			const string& GetName() const;
			/*
				Function: GetType.

				Get the type of the file.

				Example:

				"dat" from "C:\myFile.dat"
			*/
			const string& GetType() const;
			/*
				Function: GetLocation.

				The location of the file, without the name included.

				Example:

				"C:\" from "C:\myFile.dat"
			*/
			const string& GetLocation() const;
			/*
				Function: GetPath.

				The full name of the file including the location.

				Example:

				"C:\myFile.dat" from "C:\myFile.dat"
			*/
			const string& GetPath() const;

			/*
				Function: IsDirectory

				Returns true if this file is a directory.
			*/
			bool IsDirectory();

			virtual ~FileInfo();
	};


	/*
	Class: Directory

	This structure contains basic information pertaining to a directory, and functions
	which can be used to traverse through it.
	*/
	class Directory: public FileInfo, public IUncopyable
	{
		private:
			#ifdef WIN32
                HANDLE directoryTraverser;
                WIN32_FIND_DATA fileData;
            #else
                DIR* directoryTraverser;
                dirent* fileData;
			#endif

		public:
			/*h
				Constructor: Directory

				Loads an OS specific directory traverser into memory, and stores it for later use.

				Parameters:
					directoryPath - The location of the directory you would like to traverse, this can be
								       a static path, or one relative to the compiled exe.
			*/
			Directory(const string& directoryPath);


			/*
				Function: GetNextFile

				Returns:
					The memory location to the information for the next file in the traversor's trajectory.
					If there is no next file, a NULL pointer is returned.

					Note:
						- The file info object is created on the heap, and therefore can be safely stored for later use.
						But it must be deleted manually.

						- If the file info object returned is a directory (See FileInfo::IsDirectory) then it can be safely
						cast into a Directory object, and used to traverse that directory.

			*/
			FileInfo* GetNextFile();

			/*
				Destructor: Directory

				Release the OS specific file traverser.
			*/
			~Directory();

	};
}

#endif
