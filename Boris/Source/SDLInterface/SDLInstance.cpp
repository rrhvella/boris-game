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

#include <SDLInterface/SDLScreenEffects.h>
#include <SDLInterface/SDLInstance.h>
#include <SDLInterface/SDLForm.h>

using namespace SDLInterfaceLibrary;
using namespace EventHandling;

void SDLInstance::AssertVideo()
{
	if(screen == NULL)
	{
		stringstream error;

		error << "Error: Attempted to run an SDLInstance video function without first initializing video" << endl;
		error << "Please run SDLInstance::InitializeVideo() first" << endl;

		throw SDLInterfaceLibraryException(error.str().c_str());
	}
}

void SDLInstance::AssertAudio()
{
	if(!SDL_WasInit(SDL_INIT_AUDIO))
	{
		stringstream error;

		error << "Error: Attempted to run an SDLInstance audio function without first initializing audio" << endl;
		error << "Please run SDLInstance::InitializeVideo() first" << endl;

		throw SDLInterfaceLibraryException(error.str().c_str());
	}
}


SDL_Surface* SDLInstance::SetupSDL(Uint32 flags, const Bounds2D<int>& windowSize, int colorDepth)
{
	int error = SDL_Init(SDL_INIT_VIDEO);

	if(error)
	{
		throw SDLException();
	}

	error = TTF_Init();

	if(error)
	{
		throw TTFException();
	}

	SDL_Surface* mainSurface = SDL_SetVideoMode(windowSize.width, windowSize.height,
												colorDepth, flags);
	if(mainSurface == NULL)
	{
		throw SDLException();
	}

	return mainSurface;
}

void SDLInstance::Draw()
{
	if(childWithFocus != NULL)
	{
		childWithFocus->Draw();
	}
}


void SDLInstance::FlipScreen()
{
	if(SDL_Flip(screen) == -1)
	{
		throw SDLException();
	}
}

void SDLInstance::MusicEndCallback()
{
	SDLInstance::GetInstance().GetMusicEndHandlers().RaiseEvents();
}

SDLInstance::SDLInstance()
{
	runStartHandlers = new GenericEventHandlerCollection();
	musicEndHandlers = new GenericEventHandlerCollection();

	screen = NULL;
}

void SDLInstance::InitializeVideo(bool fullScreen, const Bounds2D<int>& windowSize,
							int colorDepth, int frameRate)
{
	if(fullScreen)
	{
		screen = SetupSDL(SDL_FULLSCREEN | SDL_HWSURFACE | SDL_DOUBLEBUF, windowSize, colorDepth);
	}
	else
	{
		screen = SetupSDL(SDL_SWSURFACE, windowSize, colorDepth);
	}

	frameRateManager = new FPSmanager();
	SDL_initFramerate(frameRateManager);

	SDL_setFramerate(frameRateManager, frameRate);

	childWithFocus = NULL;
	running = false;

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	Mix_HookMusicFinished(SDLInstance::MusicEndCallback);
}

void SDLInstance::InitializeAudio(int sampleRate, int bufferSize)
{
	int error = SDL_Init(SDL_INIT_AUDIO);

	if(error)
	{
		throw SDLException();
	}

	error = Mix_OpenAudio(sampleRate, AUDIO_S16, 2, bufferSize);

	if(error)
	{
		throw MixException();
	}
}

SDL_Surface* SDLInstance::GetScreen() const
{
	return screen;
}

int SDLInstance::GetWindowWidth() const
{
	if(screen != NULL)
	{
		return screen->w;
	}
	else
	{
		return 0;
	}
}

int SDLInstance::GetWindowHeight() const
{
	if(screen != NULL)
	{
		return screen->h;
	}
	else
	{
		return 0;
	}
}

GenericEventHandlerCollection& SDLInstance::GetRunStartHandlers()
{
	return *runStartHandlers;
}

GenericEventHandlerCollection& SDLInstance::GetMusicEndHandlers()
{
	return *musicEndHandlers;
}

void SDLInstance::SetCursorEnabled(bool enabled)
{
	AssertVideo();

	int error = SDL_ShowCursor((int)enabled);

	if(error)
	{
		throw SDLException();
	}
}

void SDLInstance::SetCaption(const char* caption)
{
	AssertVideo();

	SDL_WM_SetCaption(caption, NULL);
}

void SDLInstance::SetFocus(SDLForm* childForm)
{
	if(childWithFocus != NULL)
	{
		childWithFocus->SetParent(NULL);
	}

	childWithFocus = childForm;

	if(childWithFocus != NULL)
	{
		childWithFocus->screen = this->screen;
	}
}

FPSmanager* SDLInstance::GetFrameRateManager() const
{
	return frameRateManager;
}

void SDLInstance::PerformScreenEffect(ISDLScreenEffect& screenEffect)
{
	AssertVideo();

	if(screenEffect.RequiresDraw())
	{
		Draw();
	}

	screenEffect.Draw(this);
}

void SDLInstance::PlayMusic(Mix_Music* music, int numberOfLoops, int fadeInLength)
{
	AssertAudio();

	//int error = Mix_FadeInMusic(music, numberOfLoops, fadeInLength);

	int error = Mix_PlayMusic(music, numberOfLoops);

	if(error)
	{
		throw MixException();
	}
}

void SDLInstance::FadeOutMusic(int fadeOutLength)
{
	AssertAudio();

	int success = Mix_FadeOutMusic(fadeOutLength);

	if(!success)
	{
		throw MixException();
	}
}

void SDLInstance::HaltMusic()
{
	AssertAudio();

	int error = Mix_HaltMusic();

	if(error)
	{
		throw MixException();
	}
}

void SDLInstance::PlaySound(Mix_Chunk* sound)
{
	Mix_PlayChannel(-1, sound, 0);
}

void SDLInstance::Run()
{
	AssertVideo();

	running = true;
	SDL_Event currentEvent;

	runStartHandlers->RaiseEvents();

	while(running && childWithFocus != NULL)
	{
		SDL_framerateDelay(frameRateManager);
		childWithFocus->UpdateTimers(SDL_getFramerate(frameRateManager));

		Draw();
		FlipScreen();

		while(SDL_PollEvent(&currentEvent))
		{
			switch(currentEvent.type)
			{
				case SDL_QUIT:
				{
					Quit();
					break;
				}

				case SDL_KEYDOWN:
				{
					int keySymbol = currentEvent.key.keysym.sym;

					if(keySymbol == SDLK_F4 && SDL_GetKeyState(NULL)[SDLK_LALT])
					{
						Quit();
					}
					else
					{
						childWithFocus->KeyDown(currentEvent.key.keysym.sym);
					}

					break;
				}

				case SDL_KEYUP:
				{
					childWithFocus->KeyUp(currentEvent.key.keysym.sym);
					break;
				}
			}
		}
	}
}

bool SDLInstance::KeyIsPressed(int keySymbol)
{
	return SDL_GetKeyState(NULL)[keySymbol] > 0;
}

void SDLInstance::Quit()
{
	if(running)
	{
		running = false;
	}
}


SDLInstance& SDLInstance::GetInstance()
{
	static SDLInstance instance;
	return instance;
}

void SDLInstance::CleanUpSDL()
{
	if(screen != NULL)
	{
		SDL_FreeSurface(screen);
	}
	
	Mix_CloseAudio();
	Mix_Quit();

	TTF_Quit();
	SDL_Quit();
}


void SDLInstance::CleanUp()
{
	delete frameRateManager;
}
