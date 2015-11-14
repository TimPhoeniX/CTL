/*  
 * Conceptual Template Library by Piotr Grudzień
 * Implementing list
 */
#ifndef _CTL_LIST_HPP_
#define _CTL_LIST_HPP_

#include <memory>
#include <iostream>
//#include <vector> //Get own destroyer later

namespace CTL
{
	template<typename T>
	class ArrayList
	{
	public:
		typedef T ValueType;
		typedef std::size_t SizeType;
		typedef std::ptrdiff_t DifferenceType;
		typedef T* Pointer;
		typedef const T* ConstPointer;
		typedef T& Reference;
		typedef const T& ConstReference;
		typedef T* Iterator;
		typedef const T* ConstIterator;
		typedef std::allocator<T> TypeAllocator;
	private:
		TypeAllocator Alloc = TypeAllocator();
		Pointer Head = nullptr;
		SizeType Size = 0 ;
		SizeType MaxSize = 0;
		
		
		void MoveBack(unsigned int begin)
		{
			this->Alloc.construct(this->Head+Size,std::move(this->Head[Size-1]));
			for(unsigned int i = this->Size-1; i > begin; --i)
			{
				this->Head[i]=std::move(this->Head[i-1]);
			}
			++(this->Size);
		}
		
		void MoveFront(unsigned int begin)
		{
			for(unsigned int i = begin; i < Size-1; ++i)
			{
				this->Head[i]=std::move(this->Head[i+1]);
			}
			this->Alloc.destroy(this->Head+(Size--)-1);
		}
		
	public:
		ArrayList(SizeType max = 1024)
//		: Head(new ValueType[max]), MaxSize(max)
		: Head(Alloc.allocate(max)), MaxSize(max)
		{
		}
		
		ArrayList(const CTL::ArrayList<ValueType>& l)
		: Head(Alloc.allocate(l.MaxSize)), Size(l.Size), MaxSize(l.MaxSize)
		{
			for(SizeType i = 0; i < Size; ++i)
			{
				this->Head[i]=l.Head[i];
			}
		}
		
		ArrayList(CTL::ArrayList<ValueType>&& l)
		: Head(l.Head), Size(l.Size), MaxSize(l.MaxSize)
		{
			l.Head = nullptr;
		}
		
		~ArrayList()
		{
			std::_Destroy(Head,Head+Size,Alloc);
			this->Alloc.deallocate(this->Head,MaxSize);
		}
		
		bool Empty() const
		{
			return bool(this->Size);
		}
		
		SizeType GetSize() const
		{
			return this->Size;
		}
		
		SizeType GetMaxSize() const
		{
			return this->MaxSize;
		}
		
		void PushFront(const ValueType& e)
		{
			if(this->Size<this->MaxSize)
			{
				this->MoveBack(0);
				*(this->Head)=e;
			}
		}
		
		void PushFront(ValueType&& e)
		{
			if(this->Size<this->MaxSize)
			{
				this->MoveBack(0);
				(*this->Head)=e;
			}
		}
		
		void PushBack(const ValueType& e)
		{
			if(this->Size<this->MaxSize)
			{
				this->Alloc.construct(this->Head+(Size++),e);
			}
		}
		
		void PushBack(ValueType&& e)
		{
			if(this->Size<this->MaxSize)
			{
				this->Alloc.construct(this->Head+(Size++),e);
			}
		}
		
		//Standarize later;
		ValueType PopBack()
		{
			if(this->Size==0) throw std::out_of_range("PopBack called on empty list");
			ValueType tmp = std::move(this->Head[Size-1]);
			this->Alloc.destroy(this->Head+((Size--)-1));
			return tmp;
		}
		
		ValueType PopFront()
		{
			if(this->Size==0) throw std::out_of_range("PopFront called on empty list");
			ValueType tmp = std::move(*(this->Head));
			this->MoveFront(0);
			return tmp;
		}
		
		Reference Get(const SizeType i)
		{
			if(this->Size<=i) throw std::out_of_range("Get called with i >= Size");
			return this->Head[i];
		}
		
		ConstReference Get(const SizeType i) const
		{
			if(this->Size<=i) throw std::out_of_range("Get called with i >= Size");
			return this->Head[i];
		}
		
		void Insert(const SizeType i, const ValueType& e)
		{
			if(this->Size<this->MaxSize)
			{
				this->MoveBack(i);
				this->Head[i]=e;
			}
		}
		
		void Insert(const SizeType i, ValueType&& e)
		{
			if(this->Size<this->MaxSize)
			{
				this->MoveBack(i);
				this->Head[i]=std::move(e);
			}
		}
		
		void Clear()
		{
			std::_Destroy(this->Head,this->Head+this->Size,Alloc);
			this->Size=0;
		}
		
		void Print(std::ostream& out = std::cout)
		{
			for(SizeType i = 0; i < this->Size; ++i)
			{
				out << this->Head[i] << ' ';
			}
		}
	};
}

#endif