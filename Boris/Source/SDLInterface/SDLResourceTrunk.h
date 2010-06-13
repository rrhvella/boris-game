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

#ifndef SDL_RESOURCE_TRUNK_H
#define SDL_RESOURCE_TRUNK_H

#include <map>
#include <sstream>
#include <fstream>

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include <SDLInterface/ResourceException.h>
#include <SDLInterface/SDLException.h>
#include <SDLInterface/MixException.h>
#include <Helpers/DirectoryTraverser.h>
#include <Helpers/IUncopyable.h>

using namespace std;
using namespace SDLInterfaceLibrary;
				   
/*
	File: SDLResourceTrunk.h
	Contains the declarations of ISDLResourceTrunk, and it's children.
*/
namespace SDLInterfaceLibrary
{	/*
		Enum: ResourceType

		Used to identify the type of a resource.

		RESOURCE_TYPE_IMAGE - Image resource
		RESOURCE_TYPE_MUSIC - Music resource
		RESOURCE_TYPE_SOUND - Sound resource
	*/
	enum ResourceType
	{
		RESOURCE_TYPE_IMAGE,
		RESOURCE_TYPE_MUSIC,
		RESOURCE_TYPE_SOUND
	};


	/*
		Class: ISDLResourceTrunk

		A resource trunk is any collection of content. This can be anything from
		a folder to a particular resource file. ISDLResourceTrunk specifies the interace
		for ResourceTrunk classes, which must implement the pure virtual function LoadResources.

		See Also:
			<ISDLResourceTrunk::LoadResources>
	*/
	class ISDLResourceTrunk: public IUncopyable
	{
		protected:
			string name;
			//The images this trunk contains.
			map<string, SDL_Surface*> images;

			//The music this trunk contains.
			map<string, Mix_Music*> music;
			map<string, Mix_Chunk*> sound;
		public:
			/*
				Constructor: ISDLResourceTrunk

				Parameters:
					name - The name of the trunk.

				Note:
					In general, the name should be used to load the trunk into memory.
					For example if the trunk is a folder, then the name should be the location
					of the folder relative to the exe.
			*/
			ISDLResourceTrunk(const string& name);
			/*
				Function: LoadResources

				Loads all the resources associated with this trunk into memory.
			*/
			virtual void LoadResources() = 0;
			/*
				Function: GetImage

				Returns:
					The image [imageName].

				Note:
					The name of the image should define it's location in the trunk.
					In the case of a folder, this would be the filename without the type.

					Example:

					"myImage" in "myImage.bmp".
			*/
			SDL_Surface* GetImage(string& imageName) const;
			/*
				Function: GetMusic

				Returns:
					The musical piece [musicName].

				Note:
					The name of the musical piece should define it's location in the trunk.
					In the case of a folder, this would be the filename without the type.

					Example:

					"myMp3File" in "myMp3File.mp3".
			*/
			Mix_Music* GetMusic(string& musicName) const;
			/*
				Function: GetSound

				Returns:
					The sound chunk [soundName].

				Note:
					The name of the sound chunk should define it's location in the trunk.
					In the case of a folder, this would be the filename without the type.

					Example:

					"myWAVFile" in "myWAVFile.wav".
			*/
			Mix_Chunk* GetSound(string& soundName) const;
			virtual ~ISDLResourceTrunk();
	};
	/*
		Class: FolderResourceTrunk

		Defines a ResourceTrunk which loads it's resources from a folder in the hard disk.

		See Also:
			<ISDLResourceTrunk>
	*/
	class FolderResourceTrunk: public ISDLResourceTrunk
	{
		private:
			SDL_Surface* LoadImage(const FileInfo* file);
			Mix_Music* LoadMusic(const FileInfo* file);
			Mix_Chunk* LoadSound(const FileInfo* file);

		public:
			/*
				Constructor: FolderResourceTrunk

				Parameters:
					name - The location of the folder from which the resources will be loaded. This
						   can be a static location, or one relative to the exe.
			*/
			FolderResourceTrunk(const string& name);
			/*
				Function: LoadResources

				Loads the content, from the folder specified in the constructor, into memory.
			*/
			void LoadResources();
	};
	/*
		Class: FileResourceTrunk

		Defines a ResourceTrunk which loads it's resources from a binary file. 

		The file should be in the following format:

			byte 0: Number of files contained within this package.

			For each file:

			byte 0: Filetype. 0 == Bitmap, 1 == Music File, 2 == Sound file 
			byte 1: Number of characters in file name. :- nameLength
			byte 2 - nameLength: Filename.
			byte nameLength + 2 - nameLength + 5: Number of bytes in file. :- fileLength
			byte nameLength + 6 - nameLength + 5 + fileLength. :- file.

		It is recommended that you use the resource packer utility to pack folders.

		See Also:
			<ISDLResourceTrunk>
	*/
	class FileResourceTrunk: public ISDLResourceTrunk
	{
		private:
			SDL_Surface* LoadImage(SDL_RWops* resourceMemory);
			Mix_Music* LoadMusic(SDL_RWops* resourceMemory);
			Mix_Chunk* LoadSound(SDL_RWops* resourceMemory);

		public:
			/*
				Constructor: FileResourceTrunk

				Parameters:
					name - The location of the file from which the resources will be loaded. This
						   can be a static location, or one relative to the exe.
			*/
			FileResourceTrunk(const string& name);
			/*
				Function: LoadResources

				Loads the content, from the file specified in the constructor, into memory.
			*/
			void LoadResources();
	};
}

#endif
