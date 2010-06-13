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

#include <Helpers/DirectoryTraverser.h>

string Helpers::DIRECTORY_TYPE = "Directory";
string Helpers::NO_FILE_TYPE = "Not Available";

using namespace Helpers;

FileSystemException::FileSystemException(const char* errorMessage):	ApplicationException(errorMessage)
{
}

FileInfo::FileInfo(const string& name, const string& type, const string& location)
{
	this->name = name;
	this->type = type;
	this->location = location;

	//If this file is a directory, or it has no type, then it's path
	//is simply it's name appended to the location, otherwhise the type must also be included.
	if(name.compare(NO_FILE_TYPE) == 0 || IsDirectory())
	{
		path = location + "/" + name;
	}
	else
	{
		path = location + "/" + name + "." + type;
	}

}

const string& FileInfo::GetName() const
{
	return name;
}

const string& FileInfo::GetType() const
{
	return type;
}

const string& FileInfo::GetLocation() const
{
	return location;
}

const string& FileInfo::GetPath() const
{
	return path;
}

bool FileInfo::IsDirectory()
{
	return type.compare(DIRECTORY_TYPE) == 0;
}

FileInfo::~FileInfo()
{
}

string RemoveLastSlash(const string& directoryPath)
{
	if(directoryPath.find_last_of('/') == directoryPath.length())
	{
		return directoryPath.substr(0, directoryPath.length() -1);
	}
	else
	{
		return directoryPath;
	}
}

string PathToDirectoryName(const string& directoryPath)
{
	string modifiedDirectoryPath = RemoveLastSlash(directoryPath);
	size_t lastSlashPosition = modifiedDirectoryPath.find_last_of('/');

	if(lastSlashPosition != string::npos)
	{
		return modifiedDirectoryPath.substr(lastSlashPosition + 1);
	}
	else
	{
		return modifiedDirectoryPath;
	}
}

string PathToDirectoryLocation(const string& directoryPath)
{
	string modifiedDirectoryPath = RemoveLastSlash(directoryPath);
	size_t lastSlashPosition = modifiedDirectoryPath.find_last_of('/');

	if(lastSlashPosition != string::npos)
	{
		return modifiedDirectoryPath.substr(0, lastSlashPosition);
	}
	else
	{
		return ".";
	}
}

#ifdef _WIN32

Directory::Directory(const string& directoryPath):
	FileInfo(PathToDirectoryName(directoryPath),
			 DIRECTORY_TYPE,
			 PathToDirectoryLocation(directoryPath)
			)

{
	string convertedString = path;
	convertedString.append("/*");

	directoryTraverser = FindFirstFile(convertedString.c_str(), &fileData);

	if(directoryTraverser == INVALID_HANDLE_VALUE)
	{
		string error = "Directory " + name + " does not exist";
		throw FileSystemException(error.c_str());
	}

	//Skip the string '..', which is at the beginning of every directory listing in windows.
	FindNextFile(directoryTraverser, &fileData);

}

FileInfo* Directory::GetNextFile()
{
	//If there is a next file
	if(FindNextFile(directoryTraverser, &fileData) != 0)
	{
		string fullFileName = "";
		fullFileName = fileData.cFileName;

		//The char* returnedh by WIN32_FILE_DATA is the full name of the file, meaning name + '.' + type.
		unsigned int lastDotPosition = fullFileName.find_last_of('.');

		if(lastDotPosition != string::npos)
		{
			string newFileName = fullFileName.substr(0, lastDotPosition);
			string newFileType = ToLower(fullFileName.substr(lastDotPosition + 1));

			return new FileInfo(newFileName, newFileType, path);
		}
		else
		{
			//If the next file is a directory.
			if(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				return new Directory(path + "/" + fullFileName);
			}
			else
			{
				//No file types have no type, and therefore one can simply pass the full
				//name of the file.
				return new FileInfo(fullFileName, NO_FILE_TYPE, path);
			}
		}
	}
	else
	{
		DWORD error = GetLastError();

		if(error == ERROR_NO_MORE_FILES)
		{
			//If there are no more files, return NULL pointer.
			return NULL;
		}
		else
		{
			stringstream fileError;

			fileError << "General WINAPI file error: ";
			fileError << error;

			throw FileSystemException(fileError.str().c_str());
		}
	}
}

Directory::~Directory()
{
	FindClose(directoryTraverser);
}

#else

Directory::Directory(const string& directoryPath):
	FileInfo(PathToDirectoryName(directoryPath),
			 DIRECTORY_TYPE,
			 PathToDirectoryLocation(directoryPath)
			 )

{
	directoryTraverser = opendir(path.c_str());

	if(directoryTraverser == NULL)
	{
	    stringstream error;
	    error << "File System Error: " << strerror(errno);
		throw FileSystemException(error.str().c_str());
	}
}

FileInfo* Directory::GetNextFile()
{
    errno = 0;

	fileData = readdir(directoryTraverser);

	if(fileData != NULL)
	{
		string fullFileName = fileData->d_name;

		unsigned int lastDotPosition = fullFileName.find_last_of('.');

		if(lastDotPosition != string::npos)
		{
		    string newFileName = fullFileName.substr(0, lastDotPosition);
			string newFileType = ToLower(fullFileName.substr(lastDotPosition + 1));

			return new FileInfo(newFileName, newFileType, path);
		}
		else
		{
		    //Check if file is a directory.
		    struct stat fileStatistics;

		    stat(fullFileName.c_str(), &fileStatistics);

			if(fileStatistics.st_mode & S_IFDIR)
			{
				return new Directory(path + "/" + fullFileName);
			}
			else
			{
				//No file types have no type, and therefore one can simply pass the full
				//name of the file.
				return new FileInfo(fullFileName, NO_FILE_TYPE, path);
			}
		}
	}
	else
	{
		if(errno)
		{
            stringstream error;
            error << "File System Error: " << strerror(errno);
            throw FileSystemException(error.str().c_str());
		}
		else
		{
			//If there are no more files, return NULL pointer.
			return NULL;
		}
	}
}

Directory::~Directory()
{

	closedir(directoryTraverser);
}
#endif

