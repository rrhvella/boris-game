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

#ifndef SDL_RESOURCE_PIPELINE_H
#define SDL_RESOURCE_PIPELINE_H

#include <map>
#include <string>
#include <sstream>
#include <iterator>

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>


#include <SDLInterface/ResourceException.h>
#include <SDLInterface/SDLResourceTrunk.h>
#include <Helpers/IUncopyable.h>

using namespace std;

namespace SDLInterfaceLibrary
{
	/*
		Class: SDLResourcePipeline

		The resource pipeline is a template class which manages the layer between the application, and
		content stored on the hard disk (Such as images and wave files). It does so by providing functions
		which can be used to load a collection of these files when they are needed, and to unload them
		when they aren't.

		Templates:
			ResourceTrunkType - The trunk type which will be used to load, access, and destroy the content.

		See Also:
			<ISDLResourceTrunk>
	*/
	template <class ResourceTrunkType>
	class SDLResourcePipeline: public IUncopyable
	{

		private:
			//The trunks currently loaded into memory.
			map<string, ResourceTrunkType*> trunks;

			//Checks if a trunk has been loaded into memory.
			void checkTrunk(string& trunkName)
			{
				if(trunks[trunkName] == NULL)
				{
					string error = "Error in Resource Pipeline: Trunk ";
					error += trunkName + " " + "does not exist.";

					throw ResourceException(error.c_str());
				}

			}

		public:
			/*
				Constructor: SDLResourcePipeline

				Checks if all the proper SDL flags have been intialized, and
				throws an error if they haven't.
			*/
			SDLResourcePipeline()
			{
				if(!SDL_WasInit(SDL_INIT_VIDEO))
				{
					stringstream error;

					error << "Error in Resource Pipeline: ";
					error << "SDL Video has not been initialized";

					string errorstring = error.str();

					throw ResourceException(errorstring.c_str());
				}


				if(!SDL_WasInit(SDL_INIT_AUDIO))
				{
					stringstream error;

					error << "Error in Resource Pipeline: ";
					error << "SDL Audio has not been initialized";
					string errorstring = error.str();

					throw ResourceException(errorstring.c_str());
				}

				int initializedFlags = Mix_Init(MIX_INIT_OGG);

				if(!(initializedFlags&MIX_INIT_OGG))
				{
					stringstream error;

					error << "Error in Resource Pipeline: ";
					error << "Ogg dynamic library could not be linked." << endl;
					error << "Inner Error: " << Mix_GetError();

					throw ResourceException(error.str().c_str());
				}
			}
			/*
				Function: LoadResourceTrunk

				Loads trunk [trunkName] into memory.

				Parameters:
					trunkName - The name of the trunk which will be loaded into memory.

			*/
			void LoadResourceTrunk(const string& trunkName)
			{
				trunks[trunkName] = new ResourceTrunkType(trunkName);
				trunks[trunkName]->LoadResources();
			}
			/*
				Function: UnloadResourceTrunk

				Unloads trunk [trunkName] from memory.

				Parameters:
					trunkName - The name of the trunk which will be unloaded from memory.

			*/
			void UnloadResourceTrunk(string& trunkName)
			{
				checkTrunk(trunkName);
				delete trunks[trunkName];

				trunks.erase(trunkName);
			}
			/*
				Function: UnloadAllTrunks

				Unloads all trunks from memory.
			*/
			void UnloadAllTrunks()
			{
				for(typename map<string, ResourceTrunkType*>::iterator currentTrunk = trunks.begin();
					currentTrunk != trunks.end();
					currentTrunk++)
				{
					delete currentTrunk->second;
				}
				trunks.clear();
			}
			/*
				Function: GetImage

				Parameters:
					trunkName - The name of the trunk from which the image will be retrieved.
					imageName - The name of the image which will be retrieved.

				Returns:
					Image, [imageName], from trunk, [trunkName].
			*/
			SDL_Surface* GetImage(string& trunkName, string& imageName)
			{
				checkTrunk(trunkName);
				return trunks[trunkName]->GetImage(imageName);
			}
			/*
				Function: GetImage

				Parameters:
					trunkName - The name of the trunk from which the musical piece will be retrieved.
					imageName - The name of the musical piece which will be retrieved.

				Returns:
					Musical piece, [musicName], from trunk, [trunkName].
			*/
			Mix_Music* GetMusic(string& trunkName, string& musicName)
			{
				checkTrunk(trunkName);
				return trunks[trunkName]->GetMusic(musicName);
			}
			/*
				Function: GetSound

				Parameters:
					trunkName - The name of the trunk from which the musical piece will be retrieved.
					soundName - The name of the sound chunk which will be retrieved.

				Returns:
					Sound chunk, [soundName], from trunk, [trunkName].
			*/
			Mix_Chunk* GetSound(string& trunkName, string& soundName)
			{
				checkTrunk(trunkName);
				return trunks[trunkName]->GetSound(soundName);
			}
	};
}

#endif
