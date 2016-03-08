/**
 * Conceptual Template Library by Piotr Grudzień
 * Binary Search Tree
 * TODO: Include allocator support
 */
#ifndef _CTL_BST_HPP_
#define _CTL_BST_HPP_

#include <functional>
#include <stdexcept>
#include <iostream>

namespace CTL
{
	template<typename T>
	class BSTNode
	{
	public:
		using ValueType = T;
		using NodeType = BSTNode<ValueType>;
		
		ValueType Value;
		NodeType* Parent = nullptr;
		NodeType* Left = nullptr;
		NodeType* Right = nullptr;
		
		BSTNode(const ValueType& t, NodeType*const p)
		: Value(t), Parent(p)
		{
		}
		
		BSTNode(ValueType&& t, NodeType*const p)
		: Value(std::move(t)), Parent(p)
		{
		}
		
		BSTNode(const NodeType& n) //Copies node, shares subtrees with original, may need to delete
		: Value(n.Value), Parent(n.Parent), Left(n.Left), Right(n.Right)
		{
		}
		
		BSTNode(NodeType&& n)
		: Value(std::move(n.Value)), Parent(n.Parent), Left(n.Left), Right(n.Right)
		{
			n.Parent = nullptr;
			n.Right = nullptr;
			n.Left = nullptr;
		}
		
		~BSTNode()
		{
			delete this->Left;
			delete this->Right;
			//Nice recursive deletion of whole tree. Unhook before manual deletion.
		}
		
		void UnhookLeft()
		{
			this->Left = nullptr;
		}
		
		void UnhookRight()
		{
			this->Right = nullptr;
		}
		
		void Unhook()
		{
			this->Right = nullptr;
			this->Left = nullptr;
		}
		
		void HookLeft(BSTNode* l)
		{
			this->Left=l;
		}
		
		void HookRight(BSTNode* r)
		{
			this->Right=r;
		}
		
		void HookLeft(BSTNode* l, BSTNode* r)
		{
			this->Left=l;
			this->Right=r;
		}
		
		void PreorderPrint(std::ostream& out)
		{
			out << this->Value << ' ';
			if(this->Left) this->Left->PreorderPrint(out);
			if(this->Right) return this->Right->PreorderPrint(out);
		}
		
		void PostorderPrint(std::ostream& out)
		{
			if(this->Left) this->Left->PostorderPrint(out);
			if(this->Right) this->Right->PostorderPrint(out);
			out << this->Value<< ' ';
		}
		
		void ValueSwap(NodeType*const n)
		{
			std::swap(this->Value,n->Value); //May be costly, but easier to implement.
		}
	};
	
	template<typename T>
	class BSTInorderIterator
	{
	public:
		using ValueType = T;
		using Self = BSTInorderIterator<ValueType>;
		using NodeType = BSTNode<ValueType>;
		using Reference = ValueType&;
		
	private:
		NodeType* Current = nullptr;
		NodeType* Previous = nullptr;
		
		void TraverseDownLeft()
		{
			while(this->Current->Left)
			{
				this->Previous=this->Current;
				this->Current=this->Current->Left;
			}
		}
		
		void TraverseUpRight()
		{
			do
			{
				this->Previous=this->Current;
				this->Current=this->Current->Parent;
			}
			while(this->Current && this->Current->Left != this->Previous);
		}
		
	public:
		BSTInorderIterator(NodeType*const n) //Constructs iterator of given subtree
		: Current(n), Previous(nullptr)
		{
			this->TraverseDownLeft();
		}
		
		BSTInorderIterator(NodeType*const n, NodeType*const p)
		: Current(n), Previous(p)
		{
		}
			
		Reference operator*()
		{
			return this->Current->Value;
		}
		
		Self& operator++()
		{
			if(this->Current != nullptr)
			{
				if(this->Previous!=nullptr)
				{
					if(this->Previous->Left == this->Current)
					{
						if(this->Current->Right)
						{
							this->Previous=this->Current;
							this->Current=this->Current->Right;
						}
						else
						{
							this->Previous=this->Current;
							this->Current=this->Current->Parent;
						}
					}
					else if(this->Previous == this->Current->Left)
					{
						if(this->Current->Right)
						{
							this->Previous=this->Current;
							this->Current=this->Current->Right;
							this->TraverseDownLeft();
						}
						else
						{
							this->TraverseUpRight();
						}
					}
					else if(this->Previous->Right == this->Current)
					{
						if(this->Current->Right)
						{
							this->Previous=this->Current;
							this->Current=this->Current->Right;
							this->TraverseDownLeft();
						}
						else
						{
							this->TraverseUpRight();
						}
					}
				}
				else
				{
					this->Previous=this->Current;
					this->Current=nullptr;
				}
			}
			return *this;
		}
		
		Self operator++(int)
		{
			auto it = Self(this->Current,this->Previous);
			this->operator++();
			return it;
		}
		
		template<typename U>
		friend bool operator!=(const BSTInorderIterator<U>& lhs, const BSTInorderIterator<U>& rhs);
	};
	
	template<typename T>
	inline bool operator!=(const BSTInorderIterator<T>& lhs, const BSTInorderIterator<T>& rhs)
	{
		return lhs.Current!=rhs.Current;
	}
	
	template<typename T, typename Compar = std::less<T>>
	class BSTree
	{
	public:
		using ValueType = T;
		using NodeType = BSTNode<ValueType>;
		using Reference = ValueType&;
		using ConstReference = const ValueType&;
		using NodeReference = NodeType&;
		using ConstNodeReference = const NodeType&;
		using SizeType = std::size_t;
		using Iterator = BSTInorderIterator<T>;
		
	private:
		NodeType* Root = nullptr;
		SizeType Size = 0;
		Compar Comparator = Compar();
		
	public:
		BSTree(const Compar& c)
		: Comparator(c)
		{
		}
		
		BSTree(Compar&& c = Compar())
		: Comparator(std::move(c))
		{
		}
		
		BSTree(BSTree&& t)
		: Root(t.Root), Size(t.Size), Comparator(std::move(t.Comparator))
		{
			t.Root = nullptr;
			t.Size = 0;
		}
		
		~BSTree()
		{
			delete this->Root;
		}
		
		bool Insert(const ValueType& e)
		{
			auto Current = &this->Root;
			auto Previous = &this->Root;
			while(*Current)
			{
				Previous=Current;
				if(!this->Comparator(e,(*Current)->Value) && !this->Comparator((*Current)->Value,e))
				{
					return false;
				}
				else if(this->Comparator(e, (*Current)->Value))
				{
					Current=&((*Current)->Left);
				}
				else
				{
					Current=&((*Current)->Right);
				}
			}
			*Current = new NodeType(e,*Previous);
			++this->Size;
			return true;
		}
		
		
		NodeType* Find(const ValueType& e) //Easier to return null, i'm not in mood for throwing
		{
			auto Current = this->Root;
			while(Current)
			{
				if(this->Comparator(e, Current->Value))
				{
					Current=Current->Left;
				}
				else if(this->Comparator(Current->Value, e))
				{
					Current=Current->Right;
				}
				else return Current;
			}
			return Current;
		}
		
		void Delete(NodeType*const n)
		{
			if(!n) return;
			auto& link = (n->Parent ? (n==n->Parent->Left ? n->Parent->Left : n->Parent->Right) : this->Root);
			if(n->Left)
			{
				if(n->Right)
				{
					auto succesor = n->Right;
					while(succesor->Left)
					{
						succesor=succesor->Left;
					}
					n->ValueSwap(succesor);
					this->Delete(succesor);
				}
				else
				{
					link =  n->Left;
					n->Left->Parent=n->Parent;
					n->UnhookLeft();
					delete n;
				}
			}
			else
			{
				link = n->Right;
				if(n->Right) n->Right->Parent=n->Parent;
				n->UnhookRight();
				delete n;
			}
		}
		
		ValueType& Minimum()
		{
			if(!this->Size) throw std::out_of_range("Tree is empty");
			auto Current = this->Root;
			while(Current->Left)
			{
				Current=Current->Left;
			}
			return Current->Value;
		}
		
		ValueType& Maximum()
		{
			if(!this->Size) throw std::out_of_range("Tree is empty");
			auto Current = this->Root;
			while(Current->Right)
			{
				Current=Current->Right;
			}
			return Current->Value;
		}
		
		void InorderPrint(std::ostream& out)
		{
			auto end = this->End();
			for(auto i = this->Begin(); i != end; ++i)
			{
				out << *i << ' ';
			}
			out << '\n';
		}
		
		void PreorderPrint(std::ostream& out)
		{
			if(this->Root) this->Root->PreorderPrint(out);
			out << '\n';
		}
		
		void PostorderPrint(std::ostream& out)
		{
			if(this->Root) this->Root->PostorderPrint(out);
			out << '\n';
		}
		
		void RestoreWithPreorder(std::istream& in)
		{
			this->Clear();
			int size = 0;
			ValueType val;
			in >> size;
			while(size--)
			{
				in >> val;
				this->Insert(val);
			}
		}
		
		void RestoreWithPostorder(std::istream& in)
		{
			this->Clear();
			int size = 0;
			in >> size;
			ValueType* queue = new ValueType[size];
			for(int i = 0; i < size; ++i)
			{
				in >> queue[i];
			}
			for(int i = size-1; i >= 0; --i)
			{
				this->Insert(queue[i]);
			}
			delete[] queue;
		}
		
		void Clear()
		{
			delete this->Root;
			this->Root = nullptr;
		}
		
		Iterator Begin()
		{
			return Iterator(this->Root);
		}
		
		Iterator End()
		{
			return Iterator(nullptr,this->Root);
		}
		
		Iterator begin()
		{
			return this->Begin();
		}
		
		Iterator end()
		{
			return this->End();
		}
	};
}

#endif