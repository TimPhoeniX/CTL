/**
 * Conceptual Template Library by Piotr Grudzień
 * Stack Adapter
 */
#ifndef _CTL_STACK_HPP_
#define _CTL_STACK_HPP_

#include "ctl_forwardlist.hpp"

namespace CTL
{
	template<typename T>
	class Stack
	{
	public:
		typedef typename CTL::ForwardList<T> Storage;
		typedef typename Storage::ValueType ValueType;
		typedef typename Storage::SizeType SizeType;
		typedef typename Storage::Reference Reference;
		
	private:
		Storage Store;
		
	public:
		Stack()
		: Store()
		{
		}
		
// 		Stack(const Stack<T>& s)
// 		: Store(s.Store)
// 		{
// 		}
		
		Stack(Stack<T>&& s)
		: Store(std::move(s.Store))
		{
		}
		
		bool Empty()
		{
			return Store.Empty();
		}
		
		SizeType GetSize()
		{
			return Store.GetSize();
		}
		
		template<typename... Args>
		void Push(Args&&... args)
		{
			Store.PushFront(std::forward<Args>(args)...);
		}
		
		ValueType Pop()
		{
			return Store.PopFront();
		}
	};
}

#endif