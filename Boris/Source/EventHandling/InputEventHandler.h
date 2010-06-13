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

#ifndef INPUT_EVENT_HANDLER_H
#define INPUT_EVENT_HANDLER_H

#include <vector>

#include <EventHandling/LuaEventHandler.h>
#include <Helpers/IUncopyable.h>

using namespace std;

/*
File: InputEventHandler.h

Contains the declaration of IInputEventHandler, and it's children
*/

namespace EventHandling
{
	/*
		Class: IInputEventHandler

		Interface for input event handlers. An input event handler takes a pointer to a function
		which returns void and accepts a string.

		Example:
			void Frobnicate(string input);

		It then calls this function through the method RaiseEvent (Which accepts a string
		which it will then pass on to the function). All classes which inherit this interface
		should follow this procedure, and must implement the pure virtual function RaiseEvent(string& input).

		See Also:
			<IInputEventHandler::RaiseEvent>
	*/
	class IInputEventHandler
	{
		public:
			/*
				Function: RaiseEvent

				Calls the function contained within the class.

				Parameters:
					input - The string which will be passed to the callback function.
			*/
			virtual void RaiseEvent(const char* input) = 0;
			virtual ~IInputEventHandler()
			{
			}
	};

	/*
		Class: LuaInputEventHandler

		Lua implementation of the IInputEventHandler interface.

		See Also:
			<LuaEventHandler>

	*/
	class LuaInputEventHandler: public LuaEventHandler, public IInputEventHandler
	{
		public:
			/*
				Constructor: LuaInputEventHandler

				Parameters:
					luaVM - a lua_State pointer with a lua function/function container at
							top of it's stack. The function must be in the following form:
								function [Name]([string parameter])
									[Implementation]
								end

				See Also:
					<LuaEventHandler>
			*/
			LuaInputEventHandler(lua_State* luaVM):
			  LuaEventHandler(luaVM)
			{
			}


			/*
				Function: RaiseEvent

				This method will retrieve the registered function from the global lua Registry.
				It will then call that function (Unsafely, as an error should be raised to
				notify the lua-developer of any mishaps) and pass it's container
				(The table where the function is contained) if it exists.
			*/
			void RaiseEvent(const char* input)
			{
				int argumentsPassed = 1;

				//Push the registered function unto the stack
				lua_rawgeti(luaVM, LUA_REGISTRYINDEX, function);

				//If there is a function container registered, push it unto the stack
				if(functionContainer != 0)
				{
					lua_rawgeti(luaVM, LUA_REGISTRYINDEX, functionContainer);
					//If there is a function container, then it must be passed to the
					//function--So that the lua-developer can use self in that function.
					argumentsPassed++;
				}

				lua_pushstring(luaVM, input);

				//Call the function.
				lua_call(luaVM, argumentsPassed, 0);
			}
	};


	/*
		Class: CPPGenericEventHandler

		C++ implementation of the IInputEventHandler interface.
	*/
	template<class FunctionContainerType>
	class CPPInputEventHandler: public IInputEventHandler
	{
		private:
			void (FunctionContainerType::* callBackFunction) (const char*);
			FunctionContainerType* functionContainer;

		public:

			/*
				Contructor: CPPInputEventHandler

				Template:
					FunctionContainerType - The class which contains the function.

				Parameters:
					functionContainer - Pointer to the object which contains the callback function.
										It's type must be the one specified in the template FunctionContainerType.

					callBackFunction - Pointer to the actual callback function. The callback function
									   must be in the following form:

									   [Class]::[FunctionName](string input)
									   {
									    	[implementation]
									   }
				Note:

					It is the responsibility of the developer to ensure that the object passed is still
					in memory when it is called.
			*/
			CPPInputEventHandler(FunctionContainerType* functionContainer,
						void (FunctionContainerType::* callBackFunction) (const char*))
			{
				this->callBackFunction = callBackFunction;
				this->functionContainer = functionContainer;
			}

			/*
				Function: RaiseEvent

				This method will call the previously registered function, as a member of
				the previously registered object, and pass along the string "input".

				Thus:

					If &fooInstance, and &Foo::Frobnicate were passed, the following command
					would be processed fooInstance->Frobnicate(input)
			*/
			void RaiseEvent(string& input)
			{
				if(callBackFunction != NULL && functionContainer != NULL)
				{
					(*functionContainer.*callBackFunction)(input);
				}
			}
	};


	/*
		Class: InputEventHandlerCollection

		This class enables you to collect different callback functions through multiple
		interfaces (For example Lua and C++), and call them all at the same time. It
		is the reccomended type for event handlers. The registered functions are always
		called in the order they are added.


	*/
	class InputEventHandlerCollection: public IUncopyable
	{
		private:
			vector<IInputEventHandler*> handlerCollection;

		public:
			InputEventHandlerCollection()
			{

			}

			/*
				Function: AddCppEventHandler

				Adds a C/C++ event handler to the callback list.

				Parameters:
					functionContainer - Pointer to the object which contains the callback function.
										It's type must be the one specified in the template FunctionContainerType.

					callBackFunction - Pointer to the actual callback function. The callback function
									   must be in the following form:

									   [Class]::[FunctionName](string input)
									   {
									    	[implementation]
									   }

				Template:
					FunctionContainerType - The class which contains the function.

				See Also:
					<CPPInputEventHandler>
			*/
			template<class FunctionContainerType>
			void AddCppEventHandler(FunctionContainerType* functionContainer,
				void (FunctionContainerType::* callBackFunction) (const char*))
			{
				handlerCollection.push_back(
					new CPPInputEventHandler<FunctionContainerType>(functionContainer, callBackFunction)
					);
			}

			/*
				Function: AddLuaEventHandler

				Parameters:
					luaVM - a lua_State pointer with a lua function/function container at
							top of it's stack. The function must be in the following form:
								function [Name]([string parameter])
									[Implementation]
								end
				See Also:
					<LuaInputEventHandler>
			*/
			void AddLuaEventHandler(lua_State* luaVM)
			{
				handlerCollection.push_back(new LuaInputEventHandler(luaVM));
			}

			/*
				Function: RaiseEvents

				Calls all the registered functions in the order they were added.
			*/
			void RaiseEvents(const char* input)
			{
				for(vector<IInputEventHandler*>::iterator currentHandler = handlerCollection.begin();
						currentHandler != handlerCollection.end();
						currentHandler++)
				{
					(*currentHandler)->RaiseEvent(input);
				}
			}

			/*
				Destructor: InputEventHandlerCollection

				Cleans all the event handlers from memory.
			*/
			~InputEventHandlerCollection()
			{
				for(vector<IInputEventHandler*>::iterator currentHandler = handlerCollection.begin();
						currentHandler != handlerCollection.end();
						currentHandler++)
				{
						delete *currentHandler;
				}
			}
	};
}



#endif
