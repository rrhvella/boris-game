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

#ifndef SDL_SCREEN_EFFECTS_H
#define SDL_SCREEN_EFFECTS_H

#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_framerate.h>

#include <SDLInterface/SDLException.h>
#include <Helpers/GeometricHelperFunctions.h>
#include <Helpers/GeometricStructures2D.h>
#include <Helpers/IUncopyable.h>
#include <Helpers/SDLHelperFunctions.h>

using std::vector;
/*
	File: SDLScreenEffects.h

	Contains the declarations of ISDLScreenEffect, and it's children.
*/
namespace SDLInterfaceLibrary
{
	class SDLInstance;

	/*
		Enum: SDLFadeDirection

		Defines the direction of a fade effect.

		SDL_FD_IN - Fade in.
		SDL_FD_OUT - Fade out.
	*/
	enum SDLFadeDirection
	{
		SDL_FD_IN,
		SDL_FD_OUT
	};

	/*
		Class: ISDLScreenEffect

		The screen effect differs from a component effect, in that a component effect
		creates and draws a new frame each time the component it is applied to is drawn.
		On the other hand, a screen effects creates all it frames and draws them in sequence
		upon being called. This requires it to steal the rendering pipeline from the application
		temporarily, and to flush away any events in the message loop. This is the ideal base
		class for fade in/fade out and other similar effects.

		Note:
			All the derived classes of ISDLScreenEffect must implement the pure virtual function
			GenerateFrames.

		See Also:
			<ISDLEffect>
			<ISDLScreenEffect::GenerateFrames>
	*/
	class ISDLScreenEffect
	{
		private:
			bool requiresDraw;
			vector< Dimensions2D<int> > portions;
			int numberOfIterations;

			int delayBetweenFrames;

			//Returns a copy of the main screen.
			SDL_Surface* CreateScreenCopy(SDL_Surface* screen, const Vector2D<int>& copyPosition,
							const Bounds2D<int>& copyWidth);

			//Blits all the frames in [frames] to the screen.
			void DrawFrames(SDL_Surface* screen, const Vector2D<int>& copyPosition,
						const vector<SDL_Surface*>& frames, FPSmanager* frameRateManager);

			//Blits a frame to the screen.
			void DrawFrame(SDL_Surface* screen, const Vector2D<int>& copyPosition, SDL_Surface* frame);

		protected:
			/*
				Function: GenerateFrames

				Parameters:
					originalImage - The original image of the screen from which the frames will be derived.

				Returns:
					A vector of sdl surfaces which represent the frames which will be blitted to the screen,
					and their order.
			*/
			virtual vector<SDL_Surface*> GenerateFrames(SDL_Surface* originalImage) = 0;
		public:
			/*
				Constructor: ISDLScreenEffect

				Parameters:
					portions - The portions which will be copied from the screen.

					numberOfIterations - The number of times all the frames will be drawn to the screen.

					delayBetweenFrames - The number of milli-seconds that the animation is delayed between frames.

					requiresDraw - Is true if a draw cycle should occur before this effect takes place. This
								   is helpful for effects such as fadeouts, which immediatly follow fadeins
								   and therefore require a copy of what should be on the screen rather than
								   what is (Which in the case of a fadein, is a black rectangle).

			*/
			ISDLScreenEffect(const vector<Dimensions2D<int> >& portions, int numberOfFrames,
						int delayBetweenFrames, bool requiresDraw);
			/*
				Function: RequiresDraw

				Returns true if a draw cycle should occur before this effect takes place.

			*/
			bool RequiresDraw();
			/*
				Function: Draw

				Draws the effect on the screen.

				Parameters:
					instance - The SDLInstance object from which the frames will be derived.

			*/
			void Draw(SDLInstance* instance);
			
			virtual ~ISDLScreenEffect();
	};
	/*
		Class: FlashScreenEffect

		This effect will draw the original image [numberOfFrames] times. Each copy will be overlaid
		with a white layer of differing alpha values, from 0 to 255, depending on the number of frames.
		This creates the effect of a portion of the screen flashing.
	*/
	class FlashScreenEffect: public ISDLScreenEffect
	{
		private:
			int numberOfFrames;

		protected:
			/*
				Function: GenerateFrames

				Parameters:
					originalImage - The original image of the screen from which the frames will be derived.

				Returns:
					A vector of sdl surfaces which represent the frames which will be blitted to the screen,
					and their order.
			*/
			vector<SDL_Surface*> GenerateFrames(SDL_Surface* originalImage);

		public:
			/*
				Constructor: ISDLScreenEffect

				Parameters:
					portions - The portions which will be copied from the screen.

					numberOfFrames - The number of frames in the given flash effect, the higher this number
									 is, the more dense the flash animation will appear.

					numberOfIterations - The number of times all the frames will be drawn to the screen.

			*/
			FlashScreenEffect(const vector< Dimensions2D<int> >& portions,
					int numberOfFrames, int numberOfIterations);
	};
	/*
		Class: OverlayScreenEffect

		This effect will draw a colored overlay on the screen.
	*/
	class OverlayScreenEffect: public ISDLScreenEffect
	{
		private:
			int red;
			int green;
			int blue;

			int alpha;

		protected:
			/*
				Function: GenerateFrames

				Parameters:
					originalImage - The original image of the screen from which the frames will be derived.

				Returns:
					A vector of sdl surfaces which represent the frames which will be blitted to the screen,
					and their order.
			*/
			vector<SDL_Surface*> GenerateFrames(SDL_Surface* originalImage);

		public:
			/*
				Constructor: OverlayScreenEffect

				Parameters:
					portions - The portions which will be copied from the screen.

					red - The R value which will be alpha blended to the screen.
					green - The G value which will be alpha blended to the screen.
					blue - The B value which will be alpha blended to the screen.
					alpha - The A value which will be alpha blended to the screen.

			*/
			OverlayScreenEffect(const vector< Dimensions2D<int> >& portions,
						int red, int green, int blue, int alpha);
	};
	/*
		Class: FadeScreenEffect

		This effect will draw the original image [numberOfFrames] times. Each copy will be overlaid
		with a black layer of differing alpha values, from 0 to 255, depending on the number of frames.
		Also, the rate the alpha value increases/decreases depends on the fade direction specified.
		This creates the effect of a portion of the screen fading in or out.
	*/
	class FadeScreenEffect: public ISDLScreenEffect
	{
		private:
			int numberOfFrames;
			SDLFadeDirection fadeDirection;

		protected:
			/*
				Function: GenerateFrames

				Parameters:
					originalImage - The original image of the screen from which the frames will be derived.

				Returns:
					A vector of sdl surfaces which represent the frames which will be blitted to the screen,
					and their order.
			*/
			vector<SDL_Surface*> GenerateFrames(SDL_Surface* originalImage);

		public:
			/*
				Constructor: FadeScreenEffect

				Parameters:
					portions - The portions which will be copied from the screen.

					numberOfFrames - The number of frames in the given fade effect, the higher this number
									 is, the more dense the fade animation will appear.

					animationLength - The number of milli-seconds it takes for this animation to finish.

					fadeDirection - The direction in which the screen will fade.

				See Also:
					<SDLFadeDirection>
			*/
			FadeScreenEffect(const vector< Dimensions2D<int> >& portions, int numberOfFrames, unsigned int animationLength,
						SDLFadeDirection fadeDirection);
	};
}
#endif
