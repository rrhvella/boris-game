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

#include <SDLInterface/SDLResourceTrunk.h>

using namespace std;

ISDLResourceTrunk::ISDLResourceTrunk(const string& name)
{
	this->name = name;
}

SDL_Surface* ISDLResourceTrunk::GetImage(string& imageName) const
{
	if(images.find(imageName) == images.end())
	{
		stringstream error;

		error << "Error in trunk " << name << ": ";
		error << "Image '" << imageName << "' ";
		error << "does not exist";

		throw ResourceException(error.str().c_str());
	}
	else
	{
		return images.at(imageName);
	}
}

Mix_Music* ISDLResourceTrunk::GetMusic(string& musicName) const
{
	if(music.find(musicName) == music.end())
	{
		stringstream error;

		error << "Error in trunk " << name << ": ";
		error << "Music segment '" << musicName << "' ";
		error << "does not exist";

		throw ResourceException(error.str().c_str());
	}
	else
	{
		return music.at(musicName);
	}
}

Mix_Chunk* ISDLResourceTrunk::GetSound(string& soundName) const
{
	if(sound.find(soundName) == sound.end())
	{
		stringstream error;

		error << "Error in trunk " << name << ": ";
		error << "Sound chunk '" << soundName << "' ";
		error << "does not exist";

		throw ResourceException(error.str().c_str());
	}
	else
	{
		return sound.at(soundName);
	}
}

ISDLResourceTrunk::~ISDLResourceTrunk()
{
	for(map<string, SDL_Surface*>::iterator currentImage = images.begin();
		currentImage != images.end();
		currentImage++)
	{
		SDL_FreeSurface(currentImage->second);
	}


	for(map<string, Mix_Music*>::iterator currentMusic = music.begin();
		currentMusic != music.end();
		currentMusic++)
	{
		Mix_FreeMusic(currentMusic->second);
	}


	for(map<string, Mix_Chunk*>::iterator currentSound = sound.begin();
		currentSound != sound.end();
		currentSound++)
	{
		Mix_FreeChunk(currentSound->second);
	}


}

FolderResourceTrunk::FolderResourceTrunk(const string& name): ISDLResourceTrunk(name)
{
};

void FolderResourceTrunk::LoadResources()
{
	Directory trunkDirectory(name);

	FileInfo* currentFile = trunkDirectory.GetNextFile();

	while(currentFile != NULL)
	{
		if(currentFile->GetType().compare("bmp") == 0)
		{
			images[currentFile->GetName()] = LoadImage(currentFile);
		}

		if(currentFile->GetType().compare("ogg") == 0)
		{
			music[currentFile->GetName()] = LoadMusic(currentFile);
		}

		if(currentFile->GetType().compare("wav") == 0)
		{
			sound[currentFile->GetName()] = LoadSound(currentFile);
		}

		delete currentFile;
		currentFile = trunkDirectory.GetNextFile();
	}
}


SDL_Surface* FolderResourceTrunk::LoadImage(const FileInfo* file)
{
	//Load the image from the hard disk.
	SDL_Surface* loadedImage = SDL_LoadBMP(file->GetPath().c_str());
	SDL_Surface* optimizedImage = NULL;

	if(loadedImage != NULL)
	{
		//Return screen optimized version of the image.
		optimizedImage = SDL_DisplayFormat(loadedImage);
		SDL_FreeSurface(loadedImage);
	}
	else
	{
		throw SDLException();
	}

	return optimizedImage;
}

Mix_Music* FolderResourceTrunk::LoadMusic(const FileInfo* file)
{
	//Load the music from the hard disk.
	Mix_Music* loadedMusic = Mix_LoadMUS(file->GetPath().c_str());

	if(loadedMusic == NULL)
	{
		throw MixException();
	}

	return loadedMusic;
}

Mix_Chunk* FolderResourceTrunk::LoadSound(const FileInfo* file)
{
	//Load the music from the hard disk.
	Mix_Chunk* loadedSound = Mix_LoadWAV(file->GetPath().c_str());

	if(loadedSound == NULL)
	{
		throw MixException();
	}

	return loadedSound;
}



SDL_Surface* FileResourceTrunk::LoadImage(SDL_RWops* resourceMemory)
{
	SDL_Surface* loadedImage = SDL_LoadBMP_RW(resourceMemory, 0);
	SDL_Surface* optimizedImage = NULL;

	if(loadedImage != NULL)
	{
		//Return screen optimized version of the image.
		optimizedImage = SDL_DisplayFormat(loadedImage);
		SDL_FreeSurface(loadedImage);
	}
	else
	{
		throw SDLException();
	}

	return optimizedImage;
}

Mix_Music* FileResourceTrunk::LoadMusic(SDL_RWops* resourceMemory)
{
	Mix_Music* loadedMusic = Mix_LoadMUS_RW(resourceMemory);

	if(loadedMusic == NULL)
	{
		throw MixException();
	}

	return loadedMusic;
}

Mix_Chunk* FileResourceTrunk::LoadSound(SDL_RWops* resourceMemory)
{
	Mix_Chunk* loadedSound = Mix_LoadWAV_RW(resourceMemory, 0);

	if(loadedSound == NULL)
	{
		throw MixException();
	}

	return loadedSound;
}


FileResourceTrunk::FileResourceTrunk(const string& name): ISDLResourceTrunk(name)
{
};

void FileResourceTrunk::LoadResources()
{
	fstream trunkFile(name.c_str(), fstream::in | fstream::binary);

	char numberOfFiles = trunkFile.get();

	for(int i = 0; i < (int)numberOfFiles; i++)
	{
		//Get type
		char resourceType = trunkFile.get();

		//Get name length
		char nameLength = trunkFile.get();

		//Get name
		char* name = new char[nameLength];
		trunkFile.read(name, (int)nameLength);

		//Get file
		int fileLength;
		trunkFile >> fileLength;

		char* file = new char[fileLength];
		trunkFile.read(file, fileLength);

		SDL_RWops* rwopsPointer = SDL_RWFromMem((void*)file, fileLength);

		if(rwopsPointer == NULL)
		{
			throw SDLException();
		}

		switch((ResourceType)resourceType)
		{
			case RESOURCE_TYPE_IMAGE:
				images[string(name, nameLength)] = LoadImage(rwopsPointer);
				break;

			case RESOURCE_TYPE_MUSIC:
				music[string(name, nameLength)] = LoadMusic(rwopsPointer);
				break;

			case RESOURCE_TYPE_SOUND:
				sound[string(name, nameLength)] = LoadSound(rwopsPointer);
				break;
		}

		//Free buffers, music data should not be freed because mixer
		//actually accesses the same data.
		if(resourceType != RESOURCE_TYPE_MUSIC)
		{
			delete [] file;
		}

		delete [] name;
	}

	trunkFile.close();
}
