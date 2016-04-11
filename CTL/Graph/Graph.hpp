#ifndef CTL_GRAPH
#define CTL_GRAPH

#include "../Container/ctl_arraylist.hpp"
#include "../Container/ctl_stack.hpp"
#include <queue> //TODO: Replace
#include "ctl_pair.hpp"

namespace CTL
{
	enum class VertexState
	{
		White,
		Gray,
		Black
	};

	template<typename T>
	class Vertex
	{
	public:
		using VertexList = ArrayList<Vertex<T>*>;
		using VertexType = Vertex<T>;

	private:
		T label;
		size_t distance = 0;
		VertexState state = VertexState::White;
		VertexType* parent = nullptr;
		VertexList vList;

	public:
		Vertex() : Vertex(T())
		{
		}

		explicit Vertex(const T& label) :
			label(label)
		{}

		const T& Label()
		{
			return this->label;
		}

		size_t Distance()
		{
			return this->distance;
		}

		VertexState State()
		{
			return this->state;
		}

		VertexType* Parent()
		{
			return this->parent;
		}

		void SetLabel(const T& label)
		{
			this->label = label;
		}
		
		void SetDistance(const size_t dist)
		{
			this->distance = dist;
		}

		void SetState(VertexState state)
		{
			this->state = state;
		}

		void SetParent(VertexType* par)
		{
			this->parent = par;
		}

		VertexList& Adjacent()
		{
			return this->vList;
		}

		void AddVertex(VertexType* vertex)
		{
			this->vList.push_back(vertex);
		}

		void Reset()
		{
			this->distance = 0;
			this->parent = nullptr;
			this->state = VertexState::White;
		}
	};

	template<typename T>
	class Undirected
	{
	public:
		using VertexType = Vertex<T>;

		void AddEdge(VertexType* a, VertexType* b)
		{
			a->AddVertex(b);
			b->AddVertex(a);
		}
	};

	template<typename T>
	class Directed
	{
	public:
		using VertexType = Vertex<T>;

		void AddEdge(VertexType* a, VertexType* b)
		{
			a->AddVertex(b);
		}


	};


	template<typename T, typename Direction = Undirected<T>>
	class Graph : public Direction
	{
	public:
		using VertexType = Vertex<T>;
		using GraphType = ArrayList<VertexType*>;
		using iterator = typename GraphType::iterator;

	private:
		GraphType graph;

	public:
		iterator begin()
		{
			return this->graph.begin();
		}

		iterator end()
		{
			return this->graph.end();
		}

		void AddVertex(VertexType* v)
		{
			this->graph.push_back(v);
		}

		VertexType* FindVertex(const T& label)
		{
			for (auto v : this->graph)
			{
				if (v->Label() == label) return v;
			}
			return nullptr;
		}

		void BFS(VertexType* begin)
		{
			for (auto v : this->graph)
			{
				v->Reset();
			}
			//TODO: Implement Queue Adapter;
			std::queue<VertexType*> queue;
			queue.push(begin);
			begin->SetState(VertexState::Gray);
			while (!queue.empty())
			{
				auto vert = queue.front();
				queue.pop();
				for (auto v : vert->Adjacent())
				{
					if (v->State() == VertexState::White)
					{
						v->SetState(VertexState::Gray);
						v->SetDistance(vert->Distance() + 1);
						v->SetParent(vert);
						queue.push(v);
					}
				}
				vert->SetState(VertexState::Black);
			}
		}

		void DFS(VertexType* begin)
		{
			for (auto v : this->graph)
			{
				v->Reset();
			}
			Stack<VertexType*,ArrayList<VertexType*>> stack;
			stack.push(begin);
			{
				while (!stack.empty())
				{
					auto vert = stack.top();
					stack.pop();
					vert->SetState(VertexState::Gray);
					for (auto v : vert->Adjacent())
					{
						if (v->State() == VertexState::White)
						{
							stack.push(v);
							v->SetParent(vert);
							v->SetDistance(vert->Distance() + 1);
						}
					}
					vert->SetState(VertexState::Black);
				}
			}
		}

		template<typename os>
		os& PrintPath(VertexType* begin, VertexType* end, os& stream)
		{
			if (begin == end)
			{
				return stream << begin->Label() << ' ';
			}
			if (end->Parent() == nullptr)
			{
				return stream << "No Path" << '\n';
			}
			return this->PrintPath<os>(begin, end->Parent(), stream) << "<- " << end->Label() << ' ';
		}
	};
}
#endif // !CTL_GRAPH
