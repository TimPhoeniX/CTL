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
		using VertexList = ArrayList<Vertex*>;
//		using VertexList = std::vector<Vertex*>;
		using VertexType = Vertex;

	private:
		T label;
		double distance = 0;
		long d =0, f = 0;
		VertexState state = VertexState::White;
		VertexType* parent = nullptr;
		VertexList vList = VertexList(8);

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

		double Distance()
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
		
		void SetDistance(const double dist)
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
			this->distance = std:numeric_limits<double>::infinity();
			this->d = 0;
			this->f = 0;
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
		using GraphType = ArrayList<VertexType*>;
//		using GraphType = std::vector<VertexType*>;
		
		using size_type = typename GraphType::size_type;
		using iterator = typename GraphType::iterator;
		
	private:
		GraphType graph;
		long DFSTime = 0;
		
		initialize(VertexType* v)
		{
			for(auto u : this->graph)
			{
				u->SetDistance(std::numeric_limits<double>::infinity());
				u->SetParent(nullptr);
			}
			v->SetDistance(0.);
		}
		
	public:
		~Graph()
		{
			for(auto it = graph.begin(), end = graph.end(); it!=end; ++it)
			{
				delete *it;
			}
		}
		
		size_t VertexCount()
		{
			return this->graph.size();
		}
		
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
			begin->SetDistance(0.);
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
						v->SetDistance(vert->Distance() + 1.);
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
			this->DFSTime=1;
			for(auto v : this->graph)
			{
				if(v->State()==VertexState::White)
				{
					this->DFSVisit(v);
				}
			}
		}
		
		//Test it
		void IterativeDFS()
		{
			Stack<VertexType*,ArrayList<VertexType*>> stack;
			this->DFSTime=1;
			for(auto v : this->graph)
			{
				v->Reset();
			}
			for(auto v : this->graph)
			{
				if(v->VertexState()==VertexState::White)
				{
					v->setState(VertexState::Gray);
					v->setD(this->DFSTime++);
					stack.push(*v);
					while(!stack.empty())
					{
						auto u = stack.top();
						stack.pop();
						for(auto w : u->Adjacent())
						{
							if(w->VertexState()==VertexState::White)
							{
								w->setState(VertexState::Gray);
								w->setD(this->DFSTime++);
								w->setParent(*u);
								stack.push(*w);
							}
						}
						u->setState(VertexState::Black);
						u->setF(this->DFSTime++);
					}
				}
			}
		}
		
		void DFSVisit(VertexType* v)
		{
			v->SetD(this->DFSTime++);
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
			v->SetF(this->DFSTime++);
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
		
		void Bellman-Ford(VertexType* begin);
// 		{
// 			this->initialize(begin);
// 			for(size_type i = 0; i < graph.size(); ++i)
// 			{
// 				for(Edge e : Edges)
// 				{
// 					Relax(e.f,e.s,W);
// 				}
// 			}
// 			for(Edge e : Edges)
// 			{
// 				if(e.v.d > e.u.d + W(u,v))
// 					return false;
// 			}
// 			return true;
// 		}
		
		void Dijkstra(VertexType* begin);
// 		{
// 			this->initialize(begin);
// 			std::set<VertexType*> s;
// 			std::priority_queue<VertexType*> q;
// 			for(auto v : this->graph)
// 			{
// 				q.push(v);
// 			}
// 			while(!q.empty())
// 			{
// 				auto u = q.top();
// 				s.insert(u);
// 				for(auto v : u->Adjacent())
// 					this->Relax(u,v);
//				q.pop();
// 			}
// 		}
	};
}
#endif // !CTL_GRAPH
