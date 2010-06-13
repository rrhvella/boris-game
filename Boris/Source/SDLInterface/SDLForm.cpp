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

#include <SDLInterface/SDLForm.h>
#include <SDLInterface/SDLInstance.h>

using namespace SDLInterfaceLibrary;
using namespace EventHandling;

SDLForm::SDLForm(const string& name, const Vector2D<int>& position, SDL_Surface* image):
	SDLComponent(name, position, image)
	{
		componentWithFocus = NULL;
	}


void SDLForm::AddChild(SDLComponent* child)
{
	vector<SDLComponent*>::iterator existingChild = find(children.begin(), children.end(), child);

	 if(existingChild == children.end())
	 {
		children.push_back(child);
		child->SetParent(this);
	 }
}


void SDLForm::RemoveChild(SDLComponent* child)
{
	 vector<SDLComponent*>::iterator childToRemove = find(children.begin(), children.end(), child);

	 if(childToRemove != children.end())
	 {
		 children.erase(childToRemove);
		 child->SetParent(NULL);
	 }

	 Update();
}

void SDLForm::AddTimer(SDLTimer& timer)
{
	timers.push_back(&timer);
}

void SDLForm::Draw()
{
	SDLComponent::Draw();

	for(vector<SDLComponent*>::iterator currentChild = children.begin();
		currentChild != children.end();
		currentChild++)
	{
		(*currentChild)->Draw();
	}
}

void SDLForm::UpdateTimers(int frameRate)
{
	for(vector<SDLTimer*>::iterator currentTimer = timers.begin();
		currentTimer != timers.end(); currentTimer++)
	{
		(*currentTimer)->Tick(frameRate);
	}
}


void SDLForm::SetFocus(KeyEventComponent* component)
{
	componentWithFocus = component;
}

void SDLForm::KeyUp(int keySymbol)
{
	if(componentWithFocus != NULL)
	{
		componentWithFocus->KeyUp(keySymbol);
	}
	else
	{
		KeyEventComponent::KeyUp(keySymbol);
	}
}


void SDLForm::KeyDown(int keySymbol)
{
	if(componentWithFocus != NULL)
	{
		componentWithFocus->KeyDown(keySymbol);
	}
	else
	{
		KeyEventComponent::KeyDown(keySymbol);
	}
}

void SDLForm::Update()
{
	for(vector<SDLComponent*>::iterator currentChild = children.begin();
		currentChild != children.end();
		currentChild++)
	{
		(*currentChild)->Update();
	}

	SDLComponent::Update();
}

SDL_Surface* SDLForm::GetScreen() const
{
	if(screen == NULL)
	{
		stringstream error;

		error << "Error in '" << this->GetName() << "': ";
		error << "Attempted to draw form without setting focus on it";

		throw SDLInterfaceLibraryException(error.str().c_str());
	}

	return screen;
}

SDLForm::~SDLForm()
{
	for(vector<SDLComponent*>::iterator currentChild = children.begin();
		currentChild != children.end();
		currentChild++)
	{
		(*currentChild)->SetParent(NULL);
	}
}
