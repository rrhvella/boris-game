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

#include <SDLInterface/SDLEffects.h>
#include <SDLInterface/SDLComponent.h>
#include <SDLInterface/SDLForm.h>

using namespace SDLInterfaceLibrary;

void SDLComponent::Blit()
{
	if(image != NULL)
	{
		int effectOffsetX = 0;
		int effectOffsetY = 0;

		SDL_Surface* imageToUpdate;

		if(currentEffect != NULL)
		{
			imageToUpdate = currentEffect->GetEffectImage(this, effectOffsetX, effectOffsetY);
		}
		else
		{
			imageToUpdate = GetImage();
		}

		SDL_Rect offset;

		offset.x = GetLeftFromOrigin() + effectOffsetX;
		offset.y = GetTopFromOrigin() + effectOffsetY;

		if(SDL_BlitSurface(imageToUpdate, NULL, GetScreen(), &offset) == -1)
		{
			throw SDLException();
		}

		previousOffsetToOrigin = Vector2D<int>(GetLeftFromOrigin() + effectOffsetX, GetTopFromOrigin() + effectOffsetY);

		//The following two conditionals are a safety catch to prevent the
		//the clear method from overlapping the screen.
		if(previousOffsetToOrigin.x < 0)
		{
			previousOffsetToOrigin.x = 0;
		}

		if(previousOffsetToOrigin.y < 0)
		{
			previousOffsetToOrigin.y = 0;
		}

		previousDimensions = Dimensions2D<int>(GetLeftFromParent(), GetTopFromParent(), imageToUpdate->w, imageToUpdate->h);

		//In the case of effects, the drawn image must be cleared because it will
		//not be used again.
		if(currentEffect != NULL)
		{
			SDL_FreeSurface(imageToUpdate);
		}
	}
}

void SDLComponent::Clear()
{
	//The clear method relies on there being a parent, as the parent
	//image is used to cover the area previously occupied by this component.
	if(GetParent() != NULL && GetParent()->GetImage() != NULL)
	{
		ConfirmPositionInParent();

		SDL_Rect offset;

		offset.x = previousOffsetToOrigin.x;
		offset.y = previousOffsetToOrigin.y;

		SDL_Rect segmentRect;

		segmentRect.x = previousDimensions.position.x;
		segmentRect.y = previousDimensions.position.y;

		segmentRect.w = previousDimensions.size.width;
		segmentRect.h = previousDimensions.size.height;

		if(SDL_BlitSurface(GetParent()->GetImage(), &segmentRect, GetScreen(), &offset) == -1)
		{
			throw SDLException();
		}
	}
}

void SDLComponent::ConfirmPositionInParent()
{
	if(GetLeftFromParent() < 0 || GetTopFromParent() < 0 ||
		GetLeftFromParent() + GetWidth() > parent->GetWidth() ||
		GetTopFromParent() + GetHeight() > parent->GetHeight())
	{
		throw SDLInterfaceLibraryException(string(name + " is outside of parent bounds").c_str());
	}
}


SDLComponent::SDLComponent(const string& name, const Vector2D<int>& position, SDL_Surface* image)
{
	this->name = name;

	this->position = position;

	this->screen = NULL;
	this->parent = NULL;

	this->image = image;

	Update();

	currentEffect = NULL;
	currentEffectInfo = NULL;

	requiresClear = false;
}


void SDLComponent::SetParent(SDLForm* parent)
{
	if(this->parent != NULL && parent != this->parent)
	{
		//Inform the previous parent that this child
		//must be removed from it's component list.

		//This component does not need to be cleared in the
		//next cycle, as it will be automatically cleared
		//when the old parent updates itself.
		requiresClear = false;

		//The current parent is set to NULL so that
		//a cyclical call doesn't occur.
		SDLForm* oldParent = this->parent;
		this->parent = NULL;

		oldParent->RemoveChild(this);
	}

	if(parent != NULL)
	{
		this->parent = parent;
		ConfirmPositionInParent();

		this->parent->AddChild(this);

		Update();
	}
	else
	{
		this->parent = NULL;
	}
}

SDLForm* SDLComponent::GetParent() const
{
	if(parent != NULL)
	{
		return parent;
	}
	else
	{
		return NULL;
	}
}

int SDLComponent::GetWidth() const
{
	if(image != NULL)
	{
		return image->w;
	}
	else
	{
		return 0;
	}
}

int SDLComponent::GetHeight() const
{

	if(image != NULL)
	{
		return image->h;
	}
	else
	{
		return 0;
	}
}

SDL_Surface* const SDLComponent::GetImage() const
{
	return image;
}

void SDLComponent::SetEffect(const ISDLEffect* effect)
{
	currentEffect = effect;

	//If there was an old effect, then the information related to the old effect
	//must be cleared from memory.
	if(currentEffectInfo != NULL)
	{
		delete currentEffectInfo;
		currentEffectInfo = NULL;
	}

	if(currentEffect != NULL)
	{
		currentEffectInfo = currentEffect->GetEffectInfo();
	}

	Update();
}

int SDLComponent::GetLeftFromOrigin() const
{
	if(parent != NULL)
	{
		return GetLeftFromParent() + GetParent()->GetLeftFromOrigin();
	}
	else
	{
		return GetLeftFromParent();
	}
}

int SDLComponent::GetTopFromOrigin() const
{
	if(parent != NULL)
	{
		return GetTopFromParent() + GetParent()->GetTopFromOrigin();
	}
	else
	{
		return GetTopFromParent();
	}
}

int SDLComponent::GetLeftFromParent() const
{
	return position.x;
}

int SDLComponent::GetTopFromParent() const
{
	return position.y;
}

const string& SDLComponent::GetName() const
{
	return name;
}

GenericEventHandlerCollection& SDLComponent::GetDrawBeginHandlers()
{
	return drawBeginHandlers;
}

GenericEventHandlerCollection& SDLComponent::GetDrawFinishHandlers()
{
	return drawFinishHandlers;
}

SDL_Surface* SDLComponent::GetScreen() const
{
	if(parent == NULL)
	{
		stringstream error;

		error << "Error in '" << this->GetName() << "': ";
		error << "Attempted to draw without setting valid parent";

		throw SDLInterfaceLibraryException(error.str().c_str());
	}

	return parent->GetScreen();
}

ISDLEffectInfo* SDLComponent::GetEffectInfo() const
{
	return currentEffectInfo;
}

void SDLComponent::AddToLeft(int increment)
{
	position.x += increment;
	Update();
}

void SDLComponent::AddToTop(int increment)
{
	position.y += increment;
	Update();
}


void SDLComponent::SetLeft(int newLeft)
{
	position.x = newLeft;
	Update();
}

void SDLComponent::SetTop(int newTop)
{
	position.y = newTop;
	Update();
}

void SDLComponent::Draw()
{
	drawBeginHandlers.RaiseEvents();

	if(updated)
	{
		updated = false;

		if(requiresClear)
		{
			Clear();
		}

		Blit();

		requiresClear = true;
	}

	drawFinishHandlers.RaiseEvents();
}

void SDLComponent::Update()
{
	updated = true;
}

SDLComponent::~SDLComponent()
{
	if(currentEffectInfo != NULL)
	{
		delete currentEffectInfo;
	}

	if(parent != NULL)
	{
		parent->RemoveChild(this);
	}
}
