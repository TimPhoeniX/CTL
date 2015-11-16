/*
 * Conceptual Template Library by Piotr Grudzień
 * Double Linked list
 * to be reworked (BaseNode,BaseNode optimal algorithms, Get rid of aligned_storage)
 */

#ifndef _CTL_LINKEDLIST_HPP_
#define _CTL_LINKEDLIST_HPP_

#include<memory>
#include<iterator>
#include<iostream>

namespace CTL
{
	template<typename T>
	class LinkedListNode
	{
	public:
		typedef T ValueType;
		typedef ValueType* Pointer;
		typedef const ValueType* ConstPointer;
		
		LinkedListNode<ValueType>* Prev = nullptr;
		LinkedListNode<ValueType>* Next = nullptr;
		typename std::aligned_storage<sizeof(ValueType),alignof(ValueType)>::type Data;
		
		LinkedListNode()
		{
		}
		
		void Hook(LinkedListNode<T>* pos)
		{
			this->Prev = pos->Prev;
			this->Next = pos;
			pos->Prev = this;
			if(this->Prev) this->Prev->Next=this;
		}
		
		void Unhook()
		{
			this->Prev->Next = this->Next;
			if(this->Next) this->Next->Prev = this->Prev;
		}
	};
	
	template<typename T>
	class LinkedListIterator
	{
	public:
		typedef T ValueType;
		typedef ValueType* Pointer;
		typedef ValueType& Reference;
		typedef std::ptrdiff_t DifferenceType;
		typedef std::forward_iterator_tag IteratorCategory;
		typedef LinkedListIterator<T> Self;
		typedef LinkedListNode<T> TypeNode;
		
		// C++ standard reqired typedefs
		typedef ValueType value_type;
		typedef Pointer pointer;
		typedef Reference reference;
		typedef DifferenceType difference_type;
		typedef IteratorCategory iterator_category;
		
		TypeNode* Node = nullptr;

		LinkedListIterator()
		{
		}
		
		LinkedListIterator(TypeNode* p)
		: Node(p)
		{
		}
		
		LinkedListIterator(const Self& x)
		: Node(x.Node)
		{
		}
		
		Self& operator=(const Self& x) 
		{
			this->Node=x.Node;
		}
		
		Reference operator*() const noexcept
		{
			return *reinterpret_cast<ValueType*>(&(this->Node->Data));
		}
		
		Pointer operator->() const noexcept
		{
			return reinterpret_cast<ValueType*>(&(this->Node->Data));
		}
		
		Self& operator++() noexcept
		{
			this->Node=this->Node->Next;
			return *this;
		}
		
		Self operator++(int) noexcept
		{
			Self tmp(*this);
			this->Node=this->Node->Next;
			return tmp;
		}
		
		Self& operator--() noexcept
		{
			this->Node=this->Node->Prev;
			return *this;
		}
		
		Self operator--(int) noexcept
		{
			Self tmp(*this);
			this->Node=this->Node->Prev;
			return tmp;
		}
		
		bool operator==(const Self& x) const noexcept
		{
			return this->Node == x.Node;
		}
		
		bool operator!=(const Self& x) const noexcept
		{
			return this->Node != x.Node;
		}
		
		Self Next() const
		{
			if(this->Node)
				return Self(this->Node->Next);
			else
				return Self(nullptr);
		}
		
		Self Prev() const
		{
			if(this->Node)
				return Self(this->Node->Prev);
			else
				return Self(nullptr);
		}
	};
	
	template<typename T>
	class LinkedListConstIterator
	{
	public:
		typedef T ValueType;
		typedef const ValueType* Pointer;
		typedef const ValueType& Reference;
		typedef std::ptrdiff_t DifferenceType;
		typedef std::forward_iterator_tag IteratorCategory;
		typedef LinkedListConstIterator<ValueType> Self;
		typedef LinkedListNode<ValueType> TypeNode;
		typedef LinkedListIterator<ValueType> Iterator;
		
		// C++ standard reqired typedefs
		typedef ValueType value_type;
		typedef Pointer pointer;
		typedef Reference reference;
		typedef DifferenceType difference_type;
		typedef IteratorCategory iterator_category;
		
		TypeNode* Node = nullptr;
		
		LinkedListConstIterator()
		{
		}
		
		LinkedListConstIterator(TypeNode* p)
		: Node(p)
		{
		}
		
		LinkedListConstIterator(const Self& x)
		: Node(x.Node)
		{
		}
		
		LinkedListConstIterator(const Iterator& x)
		: Node(x.Node)
		{
		}
		
		Self& operator=(const Self& x) 
		{
			this->Node=x.Node;
		}
		
		Reference operator*() const noexcept
		{
			return *reinterpret_cast<ValueType*>(&(this->Node->Data));
		}
		
		Pointer operator->() const noexcept
		{
			return reinterpret_cast<ValueType*>(&(this->Node->Data));
		}
		
		Self& operator++() noexcept
		{
			this->Node=this->Node->Next;
			return *this;
		}
		
		Self operator++(int) noexcept
		{
			Self tmp(*this);
			this->Node=this->Node->Next;
			return tmp;
		}
		
		Self& operator--() noexcept
		{
			this->Node=this->Node->Prev;
			return *this;
		}
		
		Self operator--(int) noexcept
		{
			Self tmp(*this);
			this->Node=this->Node->Prev;
			return tmp;
		}
		
		bool operator==(const Self& x) const noexcept
		{
			return this->Node == x.Node;
		}
		
		bool operator!=(const Self& x) const noexcept
		{
			return this->Node != x.Node;
		}
		
		Self Next() const
		{
			if(this->Node)
				return Self(this->Node->Prev);
			else
				return Self(nullptr);
		}
		
		Self Prev() const
		{
			if(this->Node)
				return Self(this->Node->Next);
			else
				return Self(nullptr);
		}
	};
	
	template<typename T>
	inline bool operator==(const LinkedListIterator<T>& a, const LinkedListIterator<T>& b) noexcept
	{
		return a.Node==b.Node;
	}
	
	template<typename T>
	inline bool operator!=(const LinkedListIterator<T>& a, const LinkedListIterator<T>& b) noexcept
	{
		return a.Node!=b.Node;
	}
	
	template<typename T>
	class LinkedList
	{
	public:
		typedef T ValueType;
		typedef ValueType& Reference;
		typedef const ValueType& ConstReference;
		typedef std::size_t SizeType;
		typedef LinkedListIterator<ValueType> Iterator;
		typedef LinkedListIterator<ValueType> ConstIterator;
		
	private:
		typedef LinkedListNode<ValueType> Node;
		typedef typename std::allocator<ValueType>::template rebind<ValueType>::other TypeAllocator;
		typedef typename std::allocator<ValueType>::template rebind<Node>::other NodeAllocator;
		
		Node* Head = nullptr;
		Node* Tail = nullptr;
		SizeType Size = 0;
		TypeAllocator TAlloc = TypeAllocator();
		NodeAllocator NAlloc = NodeAllocator();
		
		Node* GetNode()
		{
			auto tmp = this->NAlloc.allocate(1);
			return std::addressof(*tmp);
		}
		
		template<typename... Args>
		Node* MakeNode(Args&&... args)
		{
			Node* tmp = this->GetNode();
			::new ((void*)(tmp)) Node;
			this->TAlloc.construct((ValueType*)&(tmp->Data),std::forward<Args>(args)...);
			return tmp;
		}
		
		void FreeNode(Node* n)
		{
			this->NAlloc.deallocate(n,1);
		}
		
		Node* EraseAfter(Node* pos)
		{
			auto tmp = pos->Next;
			if(tmp==this->Tail) this->Tail=pos;
			tmp->Unhook();
			this->TAlloc.destroy((ValueType*)&tmp->Data);
			tmp->~Node();
			this->FreeNode(tmp);
			--this->Size;
			return pos->Next;
		}
		
	public:
		LinkedList()
		{
		}
		
		LinkedList(LinkedList<ValueType>&& f)
		: Head(f.Head), Size(f.Size)
		{
			f.Head == nullptr;
			f.Size == 0;
		}
		
		~LinkedList()
		{
			this->Clear();
		}
		
		SizeType GetSize() const
		{
			return this->Size;
		}
		
		bool Empty() const
		{
			return !bool(this->Size);
		}
		
		Iterator Begin()
		{
			return Iterator(Head);
		}
		
		Iterator End()
		{
			return Iterator(nullptr);
		}
		
		ConstIterator Begin() const
		{
			return ConstIterator(Head);
		}
		
		ConstIterator End() const
		{
			return ConstIterator(nullptr);
		}
		
		ConstIterator CBegin()
		{
			return ConstIterator(Head);
		}
		
		ConstIterator CEnd()
		{
			return ConstIterator(nullptr);
		}
		
		ValueType PopFront()
		{
			if(this->Size == 0) throw std::out_of_range("PopFront called on empty list");
			auto val = reinterpret_cast<ValueType&&>(this->Head->Data);
			auto tmp = this->Head;
			if(tmp==this->Tail) this->Tail=nullptr;
			this->Head=tmp->Next;
			if(this->Head) this->Head->Prev = nullptr;
			this->TAlloc.destroy(&tmp->Data);
			tmp->~Node();
			this->FreeNode(tmp);
			--this->Size;
			return val;
		}
		
		ValueType PopBack()
		{
			if(this->Size == 0) throw std::out_of_range("PopBack called on empty list");
			if(Head!=Tail)
			{
				auto val = reinterpret_cast<ValueType&&>(this->Tail->Data);
				auto i = this->Begin();
				while(i.Next() != Tail) ++i;
				this->EraseAfter(i.Node);
				return val;
			}
			else
			{
				return this->PopFront();
			}
		}
		
		void PushFront(const ValueType& e)
		{
			auto tmp = this->MakeNode(e);
			tmp->Next=this->Head;
			if(tmp->Next) tmp->Next->Prev=tmp;
			this->Head=tmp;
			++this->Size;
			if(this->Tail == nullptr) this->Tail = this->Head;
		}
		
		void PushBack(const ValueType& e)
		{
			if(this->Tail == nullptr) return this->PushFront(e);
			auto tmp = this->MakeNode(e);
			this->Tail->Next=tmp;
			tmp->Prev=this->Tail;
			this->Tail=tmp;
			++Size;
		}
		
		//Ugly, but will do for now.
		void Clear()
		{
			if(this->Size == 0) return;
			while(this->Head != this->Tail)
			{
				this->EraseAfter(this->Head);
			}
			this->PopFront();
		}
		
		void Insert(SizeType i, const ValueType& e)
		{
			if(i==0) return this->PushFront(e);
			else if(i==this->Size) return this->PushBack(e);
			else if(i > this->Size) return;
			else
			{
				auto it = this->Begin();
				while(--i) ++it;
				auto tmp = this->MakeNode(e);
				tmp->Hook(it.Node->Next);
				++this->Size;
			}
		}
		
		Reference Get(SizeType i)
		{
			if(i >= this->Size) throw std::out_of_range("Get called with i >= Size");
			auto tmp = this->Begin();
			while(i--) tmp++;
			return *tmp;
		}
		
		ConstReference Get(SizeType i) const 
		{
			if(i >= this->Size) throw std::out_of_range("Get called with i >= Size");
			auto tmp = this->Begin();
			while(i--) tmp++;
			return *tmp;
		}
		
		Iterator Find(const ValueType& e)
		{
			for(auto tmp = this->Begin(); tmp != this->End(); ++tmp)
			{
				if(*tmp == e) return tmp;
			}
			return Iterator(nullptr);
		}
		
		ConstIterator Find(const ValueType& e) const
		{
			for(auto tmp = this->Begin(); tmp != this->End(); ++tmp)
			{
				if(*tmp == e) return tmp;
			}
			return ConstIterator(nullptr);
		}
		
		void Erase(SizeType i)
		{
			if(i == 0)
			{
				this->PopFront();
				return;
			}
			else if(i == this->Size-1)
			{
				this->PopBack();
				return;
			}
			else if(i >= this->Size)
			{
				return;
			}
			else 
			{
				auto it = this->Begin();
				while(--i) ++it;
				this->EraseAfter(it.Node);
			}
		}
		
		void Print(std::ostream& out = std::cout) const
		{
			auto i = this->Begin();
			while(i!=this->End())
			{
				out << *(i++) << ' ';
			}
		}
		
		friend std::ostream& operator<<(std::ostream& out, const CTL::LinkedList<ValueType>& L)
		{
			L.Print(out);
			return out;
		}
	};
}

#endif