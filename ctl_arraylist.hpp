/*  
 * Conceptual Template Library by Piotr Grudzień
 * Simple ArrayList
 */
#ifndef _CTL_ARRAYLIST_HPP_
#define _CTL_ARRAYLIST_HPP_

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
		typedef std::allocator<ValueType> TypeAllocator;
		typedef std::size_t SizeType;
		typedef std::ptrdiff_t DifferenceType;
		typedef typename std::allocator_traits<TypeAllocator>::pointer Pointer;
		typedef typename std::allocator_traits<TypeAllocator>::const_pointer ConstPointer;
		typedef ValueType& Reference;
		typedef const ValueType& ConstReference;
		typedef ValueType* Iterator;
		typedef const ValueType* ConstIterator;
		
	private:
		TypeAllocator Alloc = TypeAllocator();
		Pointer Head = nullptr;
		SizeType Size = 0 ;
		SizeType MaxSize = 0;
		
		void MoveBack(SizeType begin)
		{
			if(this->Size > 0)
			{
				this->Alloc.construct(this->Head+this->Size,std::move(this->Head[Size-1]));
				for(SizeType i = this->Size-1; i > begin; --i)
				{
					this->Head[i]=std::move(this->Head[i-1]);
				}
			}
			++(this->Size);
		}
		
		void MoveFront(SizeType begin)
		{
			for(SizeType i = begin; i < Size-1; ++i)
			{
				this->Head[i]=std::move(this->Head[i+1]);
			}
			this->Alloc.destroy(this->Head+(Size--)-1);
		}
		
	public:
		ArrayList(SizeType max = 1024)
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
			l.Size = 0;
		}
		
		~ArrayList()
		{
			std::_Destroy(this->Head,this->Head+this->Size,this->Alloc);
			this->Alloc.deallocate(this->Head,this->MaxSize);
		}
		
		Iterator Begin()
		{
			return this->Head;
		}
		
		Iterator End()
		{
			return this->Head+this->Size;
		}
		
		ConstIterator Begin() const
		{
			return this->Head;
		}
		
		ConstIterator End() const
		{
			return this->Head+this->Size;
		}
		
		ConstIterator CBegin()
		{
			return this->Head;
		}
		
		ConstIterator CEnd()
		{
			return this->Head+this->Size;
		}
		
		bool Empty() const
		{
			return !bool(this->Size);
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
			if(this->Size < this->MaxSize)
			{
				this->MoveBack(0);
				*(this->Head)=e;
			}
		}
		
		void PushFront(ValueType&& e)
		{
			if(this->Size < this->MaxSize)
			{
				this->MoveBack(0);
				(*this->Head)=std::move(e);
			}
		}
		
		void PushBack(const ValueType& e)
		{
			if(this->Size < this->MaxSize)
			{
				this->Alloc.construct(this->Head+(Size++),e);
			}
		}
		
		void PushBack(ValueType&& e)
		{
			if(this->Size < this->MaxSize)
			{
				this->Alloc.construct(this->Head+(Size++),std::move(e));
			}
		}
		
		//Standarize later;
		ValueType PopBack()
		{
			if(this->Size == 0) throw std::out_of_range("PopBack called on empty list");
			ValueType tmp = std::move(this->Head[Size-1]);
			this->Alloc.destroy(this->Head+((Size--)-1));
			return tmp;
		}
		
		ValueType PopFront()
		{
			if(this->Size == 0) throw std::out_of_range("PopFront called on empty list");
			ValueType tmp = std::move(*(this->Head));
			this->MoveFront(0);
			return tmp;
		}
		
		Reference Get(const SizeType i)
		{
			if(this->Size <= i) throw std::out_of_range("Get called with i >= Size");
			return this->Head[i];
		}
		
		ConstReference Get(const SizeType i) const
		{
			if(this->Size <= i) throw std::out_of_range("Get called with i >= Size");
			return this->Head[i];
		}
		
		void Insert(const SizeType i, const ValueType& e)
		{
			if(this->Size < this->MaxSize)
			{
				this->MoveBack(i);
				this->Head[i]=e;
			}
		}
		
		void Insert(const SizeType i, ValueType&& e)
		{
			if(this->Size < this->MaxSize)
			{
				this->MoveBack(i);
				this->Head[i]=std::move(e);
			}
		}
		
		void Insert(ConstIterator i, const ValueType& e)
		{
			this->Insert(i-this->Head,e);
		}
		
		void Insert(ConstIterator i, ValueType&& e)
		{
			this->Insert(i-this->Head,std::move(e));
		}
		
		Iterator Find(const ValueType& e)
		{
			for(SizeType i = 0; i < this->Size; ++i)
			{
				if(this->Head[i] == e) return this->Head+i;
			}
			return Iterator(nullptr);
		}
		
		void Erase(const SizeType i)
		{
			if(i < this->Size) this->MoveFront(i);
		}
		
		void Clear()
		{
			std::_Destroy(this->Head,this->Head+this->Size,Alloc);
			this->Size=0;
		}
		
		void Print(std::ostream& out = std::cout) const
		{
			for(SizeType i = 0; i < this->Size; ++i)
			{
				out << this->Head[i] << ' ';
			}
		}
		
		Reference operator[](const SizeType i)
		{
			return this->Head[i];
		}
		
		ConstReference operator[](const SizeType i) const
		{
			return this->Head[i];
		}
		
		friend std::ostream& operator<<(std::ostream& out, const CTL::ArrayList<ValueType>& l)
		{
			l.Print(out);
			return out;
		}
	};
}

#endif