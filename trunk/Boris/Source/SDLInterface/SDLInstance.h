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

#ifndef SDL_INSTANCE_H
#define SDL_INSTANCE_H

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_framerate.h>
#include <SDL/SDL_mixer.h>

#include <Helpers/GeometricStructures2D.h>
#include <SDLInterface/SDLException.h>
#include <SDLInterface/SDLForm.h>
#include <SDLInterface/TTFException.h>
#include <SDLInterface/SDLInterfaceLibraryException.h>
#include <SDLInterface/MixException.h>
#include <Helpers/IUncopyable.h>
#include <EventHandling/GenericEventHandler.h>


using std::string;
using std::vector;

using namespace EventHandling;

namespace SDLInterfaceLibrary
{
	class ISDLScreenEffect;

	/*
		Class: SDLInstance

		The SDLInstance takes care of initializing, and cleaning up SDL. As well as
		handling the event loop, drawing the components, and informing them of any
		events retrieved from SDL.

		See Also:
			<SDLForm>
	*/
	class SDLInstance: public IUncopyable
	{
		private:
			//The primary screen on which all surfaces are drawn.
			SDL_Surface* screen;

			//The form on which the application is focusing.
			SDLForm* childWithFocus;
			//The framerate manager for this application.
			FPSmanager* frameRateManager;

			//Is true if the main loop is active.
			bool running;

			//Collection of generic event handlers which will be called when
			//application starts.
			GenericEventHandlerCollection* runStartHandlers;
			
			//Collection of generic event handlers which will be called when
			//a music file stops playing.
			GenericEventHandlerCollection* musicEndHandlers;


			//The following private methods can be called to
			//assert that video and audio libraries have been loaded.
			void AssertVideo();
			void AssertAudio();

			//Creates the main screen, based on the size [widowsize], flags [flags], and color
			//depth [colorDepth].
			SDL_Surface* SetupSDL(Uint32 flags, const Bounds2D<int>& windowSize, int colorDepth);

			//Draws the focused form and it's children.
			void Draw();

			//Flips the main screen.
			void FlipScreen();

			//Fires all the event handlers registered with musicEndHandlers.
			static void MusicEndCallback();

			//Private initializor for this class.
			SDLInstance();

	public:

			/*
				Function: InitializeVideo

				Initializes all libraries and settings related to video output.

				Parameters:
					fullScreen - True if this application should be fullscreen, false if it
							     should be windowed.

								 Note:

								 Only fullscreen applications make use of hardware rendering
								 and double buffers.

					windowSize - The size of the window which will be created, or, in the case
								 of a fullscreen application, the resolution.

				    colorDepth - The application's bit depth.

					frameRate - The number of frames per second which this application generates.
			*/
			void InitializeVideo(bool fullScreen, const Bounds2D<int>& windowSize,
							int colorDepth, int frameRate);
			
			/*
				Function: InitializeAudio

				Initializes all libraries and settings related to audio output.
				
				Parameters:
					sampleRate - The sample rate in hz.

					bufferSize - The buffer size in bytes.

			*/
			void InitializeAudio(int sampleRate, int bufferSize);
			/*
				Function: GetScreen

				Returns:
					The primary drawing screen.
			*/
			SDL_Surface* GetScreen()  const;
			/*
				Function: GetWindowWidth

				Returns:
					The width of the primary drawing screen.
			*/
			int GetWindowWidth()  const;
			/*
				Function: GetWindowHeight

				Returns:
					The height of the primary drawing screen.
			*/
			int GetWindowHeight()  const;
			/*
				Function: GetRunStartHandlers

				Returns:
					A read/write reference to the run start event handler collection, which
					will be called when the main loop starts.
			*/
			GenericEventHandlerCollection& GetRunStartHandlers();
			/*
				Function: GetFrameRateManager

				Returns:
					A pointer to the FPSManager used to control this application's framerate.
			*/

			FPSmanager* GetFrameRateManager()  const;
			/*
				Function: GetRunStartHandlers

				Returns:
					A read/write reference to the run start event handler collection, which
					will be called when the main loop starts.
			*/
			GenericEventHandlerCollection& GetMusicEndHandlers();
			/*
				Function: SetCursorEnabled

				Parameters:
					enabled - True if the mouse cursor should be showed, false otherwhise.
			*/
			void SetCursorEnabled(bool enabled);
			/*
				Function: SetCaption

				Parameters:
					caption - The window caption for this application.
			*/
			void SetCaption(const char* caption);
			/*
				Function: SetFocus

				Parameters:
					childForm - The form on which this application will focus.

				See Also:
					<SDLForm>
			*/
			void SetFocus(SDLForm* childForm);
			/*
				Function: PerformScreenEffect

				Parameters:
					screenEffect - The screen effect which will be performed on this application.

				See Also:
					<ISDLScreenEffect>
			*/
			void PerformScreenEffect(ISDLScreenEffect& screenEffect);
			/*
				Function: PlaySound

				Plays an sdl_mixer chunk file.
				
				Parameters
					sound - The mixer chunk which you would like to play.

			*/
			void PlaySound(Mix_Chunk* sound);
			/*
				Function: PlayMusic

				Plays an sdl_mixer music file.
				
				Parameters
					music - The mixer music file which you would like to play.
					numberOfLoops - The number of loops which the file will be played.
									
								Note:
									A value of -1 plays the sample infinitely.
									A value of 0 plays the sample once.

					fadeInLength - The number of milli-seconds it takes for 
								   the music to fade in.

			*/
			void PlayMusic(Mix_Music* music, int numberOfLoops,
					int fadeInLength);
			/*
				Function: FadeOutMusic

				Fades out the music file which is currently being played.
				
				Parameters
					fadeOutLength - The number of milli-seconds it takes 
									for the music to fade out

			*/
			void FadeOutMusic(int fadeOutLength);
			/*
				Function: HaltMusic

				Stops the music file which is currently being played.

			*/
			void HaltMusic();
			/*
				Function: GetInstance

				Returns the single instance of this class.

			*/
			static SDLInstance& GetInstance();
			/*
				Function: KeyIsPressed

				Parameters:
					keySym - The symbol of the key which will be checked.

				Returns:
					True if the given key [keySym] is being pressed, and false
					otherwhise.
			*/
			bool KeyIsPressed(int keySymbol);
			/*
				Function: Run

				Initiates the main loop. Note that the application will be locked
				in this function from hereon, and you will only be able to interact
				with it through event handlers. It is highly reccomendable that you
				finish setting up the application before running this function.

			*/
			void Run();
			/*
				Function: Quit

				Halts the main loop.
			*/
			void Quit();
			/*
				Function: CleanUp

				Cleans up the event handlers and other objects unrelated to sql.

			*/
			void CleanUp();
			/*
				Function: CleanUpSDL

				Cleans up all objects related to SDL.

			*/
			void CleanUpSDL();

	};
}
#endif
