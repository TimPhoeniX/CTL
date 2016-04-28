#ifndef CTL_GRAPH
#define CTL_GRAPH

#include "../Container/ctl_arraylist.hpp"
#include "../Container/ctl_stack.hpp"
#include <queue> //TODO: Replace
#include "../ctl_pair.hpp"

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
	//	using VertexList = ArrayList<Vertex*>;
		using VertexList = std::vector<Vertex*>;
		using VertexType = Vertex;

	private:
		T label;
		long distance = 0;
		long d =0, f = 0;
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

		long Distance()
		{
			return this->distance;
		}
		
		long D()
		{
			return this->d;
		}
		
		long F()
		{
			return this->f;
		}
		
		VertexState State() const
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
		
		void SetDistance(const long dist)
		{
			this->distance = dist;
		}

		void SetD(const long d)
		{
			this->d=d;
		}
		
		void SetF(const long f)
		{
			this->f=f;
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
			this->distance = -1;
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
			if(a&&b)
			{
				a->AddVertex(b);
				b->AddVertex(a);
			}
		}
	};

	template<typename T>
	class Directed
	{
	public:
		using VertexType = Vertex<T>;

		void AddEdge(VertexType* a, VertexType* b)
		{
			if(a&&b) a->AddVertex(b);
		}
	};


	template<typename T, typename Direction = Undirected<T>>
	class Graph : public Direction
	{
	public:
		using VertexType = Vertex<T>;
//		using GraphType = ArrayList<VertexType*>;
		using GraphType = std::vector<VertexType*>;
		using iterator = typename GraphType::iterator;

	private:
		GraphType graph;
		long DFSTime = 0;
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
			begin->SetDistance(0);
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

		void DFS()
		{
			for (auto v : this->graph)
			{
				v->Reset();
			}
			this->DFSTime=0;
			for(auto v : this->graph)
			{
				if(v->State()==VertexState::White)
				{
					this->DFSVisit(v);
				}
			}
		}
		
		void IterativeDFS()
		{
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
							v->SetParent(vert);
							v->SetDistance(vert->Distance() + 1);
							stack.push(v);
						}
					}
					vert->SetState(VertexState::Black);
				}
			}
		}
		
		void DFSVisit(VertexType* v)
		{
			++this->DFSTime;
			v->SetD(this->DFSTime);
			v->SetState(VertexState::Gray);
			for(auto u : v->Adjacent())
			{
				if(u->State()==VertexState::White)
				{
					u->SetParent(v);
					this->DFSVisit(u);
				}
			}
			v->SetState(VertexState::Black);
			++this->DFSTime;
			v->SetF(this->DFSTime);
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
		
		
		
		Graph StronglyConnectedComponents();
		//{
		//	this->DFS();
		//	Graph inverted = this->Inverted();
		//	//Sort descending by f;
		//	inverted.DFS();
		//	return inverted;
		//}
	};
}
#endif // !CTL_GRAPH
