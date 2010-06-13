#include <SDLInterface/SDLResourcePipeline.h>
#include <SDLInterface/SDLResourceTrunk.h>

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::cout;


/*
The following structure is mainly used with the pack function 
in order to store information for resources located within a folder.

source - Th/home/robert/Documents/Code/Main/C++/Borise source file of the resource.
name - The name of the resource.
type - The type of the resource, based on the ResourceType enum.
*/

struct ResourceInfo 
{
	string source;
	string name;
	ResourceType type;
};

/*
Function: Pack

Packs all the resources in a folder into a file.

Parameters:
	sourceFolder - The source folder from which the resources will be gathered.
	destinationFileName - The file into which the resources will be packed.
*/
void Pack(const string& sourceFolder, const string& destinationFileName) 
{
	/*Pack File Format:


	byte 0: Number of files contained within this package.

	For each file:

	byte 0: Filetype. 0 == Bitmap, 1 == Music File, 2 == Sound file 
	byte 1: Number of characters in file name. :- nameLength
	byte 2 - nameLength: Filename.
	byte nameLength + 2 - nameLength + 5: Number of bytes in file. :- fileLength
	byte nameLength + 6 - nameLength + 5 + fileLength. :- file.
	*/
	Directory trunkDirectory(sourceFolder);

	FileInfo* currentFile = trunkDirectory.GetNextFile();
	vector<ResourceInfo> resources;

	while(currentFile != NULL)
	{
		ResourceInfo resource;

		resource.name = currentFile->GetName();
		resource.source = currentFile->GetPath();

		if(currentFile->GetType().compare("bmp") == 0)
		{
			resource.type = RESOURCE_TYPE_IMAGE;
		} 
		else if(currentFile->GetType().compare("ogg") == 0)
		{
			resource.type = RESOURCE_TYPE_MUSIC;
		} 
		else if(currentFile->GetType().compare("wav") == 0)
		{
			resource.type = RESOURCE_TYPE_SOUND;
		}
		else 
		{
			delete currentFile;
			currentFile = trunkDirectory.GetNextFile();

			continue;
		}

		resources.push_back(resource);

		delete currentFile;
		currentFile = trunkDirectory.GetNextFile();
	}

	fstream packedFile(destinationFileName.c_str(), fstream::out | fstream::binary);

	//Write number of files
	int numberOfFiles = resources.size();
	packedFile << (char)numberOfFiles;

	//Write music files
	for(vector<ResourceInfo>::const_iterator resourceIterator = resources.begin();
		resourceIterator != resources.end();
		resourceIterator++)
	{
		//Write type
		packedFile << (char)resourceIterator->type;

		//Write name		
		packedFile << (char)resourceIterator->name.size();
		packedFile << resourceIterator->name.c_str();

		//Get file and write file length
		int fileLength = 0;
		fstream srcFile(resourceIterator->source.c_str(), fstream::in | std::ios::binary);

		srcFile.seekg(0, ios_base::end);
		fileLength = srcFile.tellg();

		srcFile.seekg(0, ios_base::beg);

		packedFile << fileLength;		
		
		//write file
		char* buffer = new char[fileLength];
		
		srcFile.read(buffer, fileLength);
		packedFile.write(buffer, fileLength);

		delete [] buffer;
	}

	packedFile.flush();
	packedFile.close();
}

int main(int argc, char** argv)
{
	try 
	{
		if(argc < 3) 
		{
			throw ApplicationException("Please specify the source folder (first argument) and destination file (first argument)");
		}

		string sourceFolder = argv[1];
		string destinationFileName = argv[2];

		Pack(sourceFolder, destinationFileName);
	}
	catch(exception& error)
	{
		cout << error.what();
	}
	return 0;
}
