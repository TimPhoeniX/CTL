/**
 * Conceptual Template Library by Piotr Grudziem
 * University Course Assignment. 
 * Do not judge me by quality of this code.
 */
#ifndef CTL_VIRTUAL_DISJOINTSET_HPP
#define CTL_VIRTUAL_DISJOINTSET_HPP
#include "ctl_forwardlist.hpp"
#include "ctl_pair.hpp"

namespace CTL
{
	

	template<typename T>
	class DisjointSet;

	template<typename T>
	class DNode
	{
	public:
		virtual ~DNode() = default;
		
		virtual DisjointSet<T>* MakeSet() = 0;
		virtual DisjointSet<T>* FindSet() = 0;
		virtual const T& getVertex() = 0;
	};

	template<typename T>
	using Edge = Pair<DNode<T>*, DNode<T>*>;

	template<typename T>
	class DisjointSet
	{
	public:
		virtual ~DisjointSet<T>() = default;

		virtual DisjointSet<T>* Union(DisjointSet<T>* s) = 0;
		virtual void PrintSet() = 0;
	};

	template<typename T>
	ForwardList<DisjointSet<T>*> ConnectedComponents(ForwardList<Edge<T>>& edges, ForwardList<DNode<T>*>& vertices)
	{
		ForwardList<DisjointSet<T>*> components;
		for(auto v : vertices)
		{
			components.PushBack(v->MakeSet());
		}
		for(auto& e : edges)
		{
			auto a = e.GetFirst()->FindSet();
			auto b = e.GetSecond()->FindSet();
			if(a!=b)
			{
				auto r = a->Union(b);
				components.Erase(r);
			}
		}
		return components;
	}

	template<typename T>
	class InTree : public DisjointSet<T>, public DNode<T>
	{
	private:
		static ForwardList<InTree<T>*> intreenodes;
		InTree<T>* parent = nullptr;
		T vertex;
	public:
		InTree(const T& vert) : parent(this), vertex(vert)
		{
			intreenodes.PushFront(this);
		}

		InTree<T>* getSet()
		{
			if(this->parent == this)
				return this;
			return parent->getSet();
		}

		DisjointSet<T>* MakeSet() override
		{
			this->parent = this;
			return this;
		}

		DisjointSet<T>* FindSet() override
		{
			auto tmp = this->getSet();
			if(this->parent != tmp) this->parent = tmp;
			return tmp;
		}
		
		const T& getVertex() override
		{
			return this->vertex;
		}

		DisjointSet<T>* Union(DisjointSet<T>* s) override
		{
			InTree<T>* t = dynamic_cast<InTree<T>*>(s);
			if(t)
			{
				t->parent = this;
			}
			return s;
		}

		void PrintSet() override
		{
			for(InTree<T>* n : intreenodes)
			{
				auto tmp = n->getSet();
				if(tmp == this)
					std::cout << n->getVertex() << ' ';
			}
			std::cout << std::endl;
		}
	};

	template<typename T>
	ForwardList<InTree<T>*> InTree<T>::intreenodes;

	template<typename T>
	class ListSet;

	template<typename T>
	class DListNode : public DNode<T>
	{
		friend class ListSet<T>;
	private:
		DListNode<T>* next = nullptr;
		DisjointSet<T>* parent = nullptr;
		T vertex;

	public:
		DListNode() = default;
		DListNode(const T& vert) : vertex(vert)
		{
		}

		DisjointSet<T>* MakeSet() override
		{
			return new ListSet<T>(this);
		}

		DisjointSet<T>* FindSet() override
		{
			return this->parent;
		}

		const T& getVertex() override
		{
			return this->vertex;
		}
	};

	template<typename T>
	class ListSet : public DisjointSet<T>
	{
	private:
		std::size_t size = 0;
		DListNode<T>* head = nullptr;
		DListNode<T>* tail = nullptr;

	public:
		ListSet() = default;
		ListSet(DListNode<T>* p) : size(1), head(p), tail(p)
		{
			p->parent = this;
		}

		DisjointSet<T>* Union(DisjointSet<T>* s) override
		{
			auto l = dynamic_cast<ListSet<T>*>(s);
			if(this->size < l->size)
			{
				return l->Union(this);
			}
			auto it = l->head;
			while(it)
			{
				it->parent = this;
				it = it->next;
			}
			this->tail->next = l->head;
			this->tail = l->tail;
			delete l;
			return s;
		}

		void PrintSet() override
		{
			auto it = this->head;
			while(it)
			{
				std::cout << it->vertex << ' ';
				it = it->next;
			}
			std::cout << std::endl;
		}
	};

}

#endif
