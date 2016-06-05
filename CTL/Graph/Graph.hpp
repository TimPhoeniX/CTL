#ifndef CTL_GRAPH
#define CTL_GRAPH

#include "../Container/ctl_arraylist.hpp"
#include "../Container/ctl_stack.hpp"
#include <queue> ///TODO: Replace
#include <unordered_map>
#include "../Sort/ctl_quick_sort.hpp"
#include "../Utility/PrintHeap.hpp"
#include "../Container/PriorityQueue.hpp"
#include <cstdlib>

namespace CTL
{
	enum class VertexState
	{
		White,
		Gray,
		Black
	};
	
	template<typename,template<typename> class>
	class Graph;

	template<typename T>
	class VertexT;
	
	template<typename T>
	class PartialEdge
	{
	protected:
		using Vertex = VertexT<T>;
		
		Vertex* to = nullptr;
		double weight = 1;
	public:
		PartialEdge(Vertex* to) : to(to) {}
		PartialEdge(Vertex* to, double weight) : to(to), weight(weight) {}
		
		
		Vertex* getTo() const
		{
			return this->to;
		}
		
		double getWeight() const
		{
			return this->weight;
		}

		void setWeight(double w)
		{
			this->weight = w;
		}
	};
	
	
	template<typename T>
	class Edge : public PartialEdge<T>
	{
	protected:
		using Vertex = typename PartialEdge<T>::Vertex;
		
		Vertex* from = nullptr;
	public:
		Edge(Vertex* from, Vertex* to) : PartialEdge<T>(to), from(from) {}
		Edge(Vertex* from, Vertex* to, double weight) : PartialEdge<T>(to,weight), from(from) {}
		
		
		Vertex* getFrom() const
		{
			return this->from;
		}
		
		template<typename os>
		friend os& operator<<(os& out, const Edge& e)
		{
			return (out << *e.from << ' ' << *e.to << ' ' << e.weight);
		}
	};
	
	template<typename T>
	class VertexT
	{
		template<typename, template<typename> class>
		friend class Graph;
	public:
		using VertexList = ArrayList<PartialEdge<T>>;
//		using VertexList = std::vector<Vertex*>;

	private:
 		T label;
		double distance = 0, estimate = 0;
		VertexState state = VertexState::White;
		VertexT* parent = nullptr;
		VertexList vList = VertexList(8);

	public:
		VertexT() : VertexT(T())
		{
		}

		explicit VertexT(const T& label) :
			label(label)
		{}

		const T& Label() const
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

		VertexT* Parent()
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

		void SetParent(VertexT* par)
		{
			this->parent = par;
		}

		VertexList& Adjacent()
		{
			return this->vList;
		}

		void AddVertex(VertexT* vertex)
		{
			this->vList.push_back(PartialEdge<T>(vertex));
		}
		
		void AddVertex(VertexT* vertex, double weight)
		{
			this->vList.push_back(PartialEdge<T>(vertex,weight));
		}

		void Reset()
		{
			this->distance = std::numeric_limits<double>::infinity();
			this->d = 0;
			this->f = 0;
			this->parent = nullptr;
			this->state = VertexState::White;
		}

		template<typename os>
		friend os& operator<<(os& out, const VertexT v)
		{
			return out << v.label;
		}
	};

	namespace Graphs
	{
		template<typename T>
		class Undirected
		{
		protected:
			using Vertex = VertexT<T>;
			using GraphType = ArrayList<Vertex*>;
			const static constexpr bool directed = false;

			void clear(GraphType& graph)
			{
				for (auto it = graph.begin(), end = graph.end(); it != end; ++it)
				{
					delete *it;
				}
			}

		public:
			void AddEdge(Vertex* a, Vertex* b)
			{
				if (a&&b)
				{
					a->AddVertex(b);
					b->AddVertex(a);
				}
			}

			void AddEdge(Vertex* a, Vertex* b, double weight)
			{
				if (a&&b)
				{
					a->AddVertex(b, weight);
					b->AddVertex(a, weight);
				}
			}
		};

		template<typename T>
		class Directed
		{
		protected:
			using Vertex = VertexT<T>;
			using GraphType = ArrayList<Vertex*>;
			const static constexpr bool directed = true;

			void clear(GraphType& graph)
			{
				for (auto it = graph.begin(), end = graph.end(); it != end; ++it)
				{
					delete *it;
				}
			}
			
		public:
			void AddEdge(Vertex* a, Vertex* b)
			{
				if (a&&b) a->AddVertex(b);
			}
			
			void AddEdge(Vertex* a, Vertex* b, double weight)
			{
				if (a&&b) a->AddVertex(b, weight);
			}
		};

		template<typename T>
		class UndirectedExtern : public Undirected<T>
		{
		protected:
			using Vertex = typename Undirected<T>::Vertex;
			using GraphType = typename Undirected<T>::GraphType;
			void clear(GraphType&)
			{}
		};
	}

	template<typename T, template <typename> class P = Graphs::Undirected >
	class Graph : public P<T>
	{
	public:
		using Policy = P<T>;
		using Vertex = typename Policy::Vertex;
		using GraphType = typename Policy::GraphType;
		using EdgeList = ArrayList<Edge<T>>;
		using size_type = typename GraphType::size_type;
		using iterator = typename GraphType::iterator;
		using QueueEntry = HeapEntry<Vertex*, double>;
		using EntryList = ArrayList<QueueEntry>;

	private:
		GraphType graph;
		long DFSTime = 0;
		
		void initialize(Vertex* v)
		{
			for(Vertex* u : this->graph)
			{
				u->state = VertexState::White;
				u->SetDistance(std::numeric_limits<double>::infinity());
				u->SetParent(nullptr);
			}
			v->SetDistance(0.);
		}
		
		//Adapts Vertex to use as InTree
		struct DisjointSet
		{
			static void MakeSet(Vertex* v)
			{
				v->parent = v;
			}

			static Vertex* FindSet(Vertex* v)
			{
				if (v == v->parent) return v;
				return (v->parent = FindSet(v->parent));
			}

			static void Union(Vertex* u, Vertex* v)
			{
				v->parent = u;
			}
		};

		//Allows for using list of vertexes as Priority Queue
		struct Heap
		{
			//using Iter = int*;
			using Iter = typename GraphType::iterator;
			template<typename Compare>
			static void SiftDown(Iter first, Iter last, Iter i, Compare comp)
			{
				using std::swap;
				typename std::iterator_traits<Iter>::difference_type Node = i - first;
				Iter Top = i, left = (first + (2 * Node + 1)), right = (first + (2 * Node + 2));
				if (left < last && comp(*Top, *left))
				{
					Top = left;
				}
				if (right < last && comp(*Top, *right))
				{
					Top = right;
				}
				if (Top != i)
				{
					swap(*i, *Top);
					SiftDown(first, last, Top, comp);
				}
			}

			template<typename Compare>
			static void BuildHeap(Iter first, Iter last, Compare comp)
			{
				Iter i = first + (((last - first) / 2) - 1);
				while (i >= first)
				{
					SiftDown(first, last, i--, comp);
				}
			}

			template<typename Compare>
			static void SiftUp(Iter first, Iter elem, Compare comp)
			{
				using std::swap;
				Iter cpy = elem + 1;
				Iter parent = first + ((elem - first-1) / 2);
				while (elem > first && comp(*parent, *elem))
				{
					swap(*elem, *parent);
					elem = parent;
					parent = first + ((elem - first-1) / 2);
				}
			}

			template<typename Compare>
			static void PopHeap(Iter first, Iter last, Compare comp)
			{
				using std::swap;
				swap(*first, *--last);
				SiftDown(first, last, first,comp);
			}
		};

	public:
		Graph() = default;
		Graph(Graph&& g) : graph(std::move(g.graph)) {}
		Graph(const Graph&) = default;

		~Graph()
		{
			this->Policy::clear(this->graph);
		}

		void ClearEdges()
		{
			for(Vertex* v : this->graph)
			{
				v->Adjacent().clear();
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

		void AddVertex(Vertex* v)
		{
			this->graph.push_back(v);
		}
		
		void AddVertex(const T& label)
		{
			this->graph.push_back(new Vertex(label));
		}
		
		Vertex* FindVertex(const T& label)
		{
			for (auto v : this->graph)
			{
				if (v->Label() == label) return v;
			}
			return nullptr;
		}

		void BFS(Vertex* begin)
		{
			for (auto v : this->graph)
			{
				v->Reset();
			}
			//TODO: Implement Queue Adapter;
			std::queue<Vertex*> queue;
			queue.push(begin);
			begin->distance = 0.;
			begin->state=VertexState::Gray;
			while (!queue.empty())
			{
				auto vert = queue.front();
				queue.pop();
				for (auto partial : vert->Adjacent())
				{
					auto v = partial.getTo();
					if (v->state == VertexState::White)
					{
						v->state = VertexState::Gray;
						v->distance = vert->distance + 1.;
						v->parent = vert;
						queue.push(v);
					}
				}
				vert->state = VertexState::Black;
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
			Stack<Vertex*,ArrayList<Vertex*>> stack;
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
					stack.push(v);
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
		
		void DFSVisit(Vertex* v)
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
		os& PrintPath(Vertex* begin, Vertex* end, os& stream)
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

		template<typename os>
		void PrintPaths(Vertex* begin, os& out)
		{
			for (auto v : this->graph)
			{
				this->PrintPath(begin, v, out) << v->distance << std::endl;
			}
		}

		void AddEdges(const EdgeList& e)
		{
			for(auto edge : e)
			{
				this->Policy::AddEdge(edge.getFrom(), edge.getTo(), edge.getWeight());
			}
		}

		EdgeList KruskalMST()
		{
			static_assert(!Policy::directed, "Cannot use KruskalMST on directed graph!");
			EdgeList MST;
			for(auto v : this->graph)
			{
				v->state = VertexState::White; //Needed to get unique edges
				DisjointSet::MakeSet(v);
			}
			EdgeList Edges;
			for(auto v : this->graph)
			{
				for(auto partial : v->Adjacent())
				{
					if(partial.getTo()->state == VertexState::Black) continue;
					Edges.push_back(Edge<T>(v,partial.getTo(),partial.getWeight()));
				}
				v->state = VertexState::Black;
			}
			QuickSort(Edges.begin(), Edges.end(), [](PartialEdge<T>& lhs, PartialEdge<T>& rhs)->bool {return lhs.getWeight() < rhs.getWeight(); });
			for(auto& edge : Edges)
			{
				auto u = DisjointSet::FindSet(edge.getFrom()), v = DisjointSet::FindSet(edge.getTo());
				if(u!=v)
				{
					MST.push_back(edge);
					DisjointSet::Union(u, v);
				}
			}
			return MST;
		}

		EdgeList KruskalMaze()
		{
			static_assert(!Policy::directed, "Cannot use KruskalMazeMST on directed graph!");
			EdgeList Maze;
			for(auto v : this->graph)
			{
				v->state = VertexState::White; //Needed to get unique edges
				DisjointSet::MakeSet(v);
			}
			EdgeList Edges;
			for(auto v : this->graph)
			{
				for(auto partial : v->Adjacent())
				{
					if(partial.getTo()->state == VertexState::Black) continue;
					Edges.push_back(Edge<T>(v, partial.getTo(), partial.getWeight()));
				}
				v->state = VertexState::Black;
			}
			Maze.reserve(Edges.size() / 4);
			std::random_shuffle(Edges.begin(), Edges.end());
			for(auto& edge : Edges)
			{
				auto u = DisjointSet::FindSet(edge.getFrom()), v = DisjointSet::FindSet(edge.getTo());
				if(u != v)
				{
					Maze.push_back(edge);
					DisjointSet::Union(u, v);
				}
			}
			return Maze;
		}

		void PrimMST()
		{
			using std::swap;
			static_assert(!Policy::directed, "Cannot use PrimMST on directed graph!");
			for(auto v : this->graph)
			{
				v->distance = std::numeric_limits<double>::infinity();
				v->parent = nullptr;
				v->state = VertexState::White; //Eliminates linear cost of lookup in array or extra space for lookup in set
			}
			this->graph[0]->distance = 0;
			auto cmp = [](Vertex* lhs, Vertex* rhs)->bool { return lhs->Distance() > rhs->Distance(); };
			auto b = this->graph.begin(), e = this->graph.end();
			Heap::BuildHeap(b,e, cmp);
			while(b<e)
			{
				Vertex* v = *b;
				v->state = VertexState::Black;
				Heap::PopHeap(b, e--, cmp);\
				for(auto partial : v->Adjacent())
				{
					Vertex* u = partial.getTo();
					if (u->state != VertexState::Black && partial.getWeight() < u->distance)
					{
						u->parent = v;
						u->distance = partial.getWeight();
						auto changed = std::find(b, e, u);
						Heap::SiftUp(b,changed,cmp);
					}
				}
			}
		}

		EdgeList PrimMaze()
		{
			using std::swap;
			for(auto v : this->graph)
			{
				v->state = VertexState::White; //Eliminates linear cost of lookup in array or extra space for lookup in set
			}
			EdgeList Maze;
			GraphType Frontier;
			EdgeList Possible;
			Frontier.reserve(size_t(std::sqrt(this->graph.size())));
			this->graph.front()->state = VertexState::Black;
			for(auto p : this->graph.front()->Adjacent())
			{
				Frontier.push_back(p.getTo());
			}
			while(!Frontier.empty())
			{
				std::swap(Frontier.back(),Frontier[std::rand()%Frontier.size()]);
				Vertex* v = Frontier.back(), *u = nullptr;
				Frontier.pop_back();
				v->state = VertexState::Black;
				for(auto partial : v->Adjacent())
				{
					u = partial.getTo();
					if(u->state == VertexState::White)
					{
						Frontier.push_back(u);
						u->state = VertexState::Gray;
					}
					if(u->state == VertexState::Black)
					{
						Possible.push_back(Edge<T>(v,u,partial.getWeight()));
					}
				}
				Maze.push_back(Possible[std::rand()%Possible.size()]);
				Possible.clear();
			}
			return Maze;
		}

		EdgeList PrimMSTE(double& total)
		{
			EdgeList mst;
			total = 0.;
			for(Vertex* v : this->graph)
			{
				Vertex* u = v->parent;
				if(u == nullptr) continue;
				if(u->label < v->label)
					mst.push_back(Edge<T>(u, v, v->distance));
				else
					mst.push_back(Edge<T>(v, u, v->distance));
			}
			return mst;
		}

		bool ConnectedComponentTest(EdgeList& edges)
		{
			static_assert(Policy::directed, "Cannot use ConnectedComponent on directed graph!");
			for(Vertex* e : this->graph)
			{
				e->state = VertexState::White;
				DisjointSet::MakeSet(e);
			}
			for(Edge<T> e : edges)
			{
				Vertex* a = DisjointSet::FindSet(e.getFrom());
				Vertex* b = DisjointSet::FindSet(e.getTo());
				if (a != b)
				{
					e.getFrom()->state = VertexState::Black;
					e.getTo()->state = VertexState::Black;
					DisjointSet::Union(a, b);
				}
			}
			for (Vertex* e : this->graph)
			{
				if (e->state == VertexState::White) return false;
			}
			return true;
		}
		
		Graph StronglyConnectedComponents();
		//{
		//	this->DFS();
		//	Graph inverted = this->Inverted();
		//	//Sort descending by f;
		//	inverted.DFS();
		//	return inverted;
		//}
		
		template<typename os>
		void PrintShortest(os& out)
		{
			QuickSort(this->graph.begin(), this->graph.end(), [](Vertex* lhs, Vertex* rhs)->bool { return lhs->label < rhs->label; });
			for (Vertex* v : this->graph)
			{
				out << v->label << ' ' << v->distance << '\n';
			}
			out << std::endl;
		}

		bool BellmanFord(Vertex* begin)
 		{
 			this->initialize(begin);
			EdgeList Edges;
			for (auto v : this->graph)
			{
				for (auto partial : v->Adjacent())
					Edges.push_back(Edge<T>(v, partial.getTo(), partial.getWeight()));
			}
 			for(size_type i = 1; i < graph.size(); ++i)
 			{
 				for(Edge<T> e : Edges)
 				{
					Vertex* v = e.getFrom(), *u = e.getTo();
					if (u->distance > v->distance + e.getWeight())
					{
						u->distance = v->distance + e.getWeight();
						u->parent = v;
					}
 				}
 			}
 			for(Edge<T> e : Edges)
 			{
 				if(e.getTo()->distance > e.getFrom()->distance + e.getWeight())
 					return false;
 			}
 			return true;
 		}
		
		void Dijkstra(Vertex* begin)
 		{
 			this->initialize(begin);
			Vertex* u = nullptr, *v = nullptr;
			auto cmp = [](const QueueEntry lhs, const QueueEntry rhs)->bool {return lhs.key > rhs.key; };
			HeapQueue<QueueEntry, EntryList, decltype(cmp)> queue(cmp);
			queue.push(QueueEntry(begin, 0));
 			while(!queue.empty())
 			{
				auto ve = queue.top();
				v = ve.value;
				queue.pop();
				if(v->state == VertexState::Black) continue;//Already removed from queue;
				v->state = VertexState::Black;
				for (auto partial : v->Adjacent())
				{
					u = partial.getTo();
					if (u->distance > v->distance + partial.getWeight())
					{
						u->distance = v->distance + partial.getWeight();
						u->parent = v;
						queue.push(QueueEntry(u, u->distance));
					}
				}
 			}
 		}
		
		void Dijkstra(Vertex* begin,Vertex* end)
		{
			for(Vertex* v : this->graph)
			{
				v->distance = std::numeric_limits<double>::infinity();
				v->estimate = std::numeric_limits<double>::infinity();
				v->parent = nullptr;
				v->state = VertexState::White;
			}
			begin->distance = 0;
			Vertex* u = nullptr, *v = nullptr;
			auto cmp = [](const QueueEntry lhs, const QueueEntry rhs)->bool {return lhs.key > rhs.key; };
			HeapQueue<QueueEntry, EntryList, decltype(cmp)> queue(cmp);
			queue.push(QueueEntry(begin, 0));
			while(!queue.empty())
			{
				auto ve = queue.top();
				v = ve.value;
				if(v == end) return;
				queue.pop();
				if(v->state == VertexState::Black) continue;//Already removed from queue;
				v->state = VertexState::Black; //Tells that vertex is removed from queuel
				for(auto partial : v->Adjacent())
				{
					u = partial.getTo();
					if(u->state != VertexState::Black && u->distance > v->distance + partial.getWeight())
					{
						u->distance = v->distance + partial.getWeight();
						u->parent = v;
						queue.push(QueueEntry(u,u->distance));
					}
				}
			}
		}

		template<typename Heuristic>
		void AStar(Vertex* begin, Vertex* end, Heuristic H)
		{
			for(Vertex* v: this->graph)
			{
				v->distance = std::numeric_limits<double>::infinity();
				v->estimate = std::numeric_limits<double>::infinity();
				v->parent = nullptr;
				v->state = VertexState::White;
			}
			begin->distance = 0;
			begin->estimate = H(begin, end);
			auto cmp = [](const QueueEntry lhs, const QueueEntry rhs)->bool {return lhs.key > rhs.key; };
			HeapQueue<QueueEntry, EntryList, decltype(cmp)> queue(cmp);
			queue.push(QueueEntry(begin, begin->estimate));
			Vertex *u = nullptr, *v = nullptr;
			double score = 0;
			while(!queue.empty())
			{
				auto ve = queue.top();
				v = ve.value;
				if(v == end) return;
				queue.pop();
				v->state = VertexState::Black;
				for(auto partial : v->Adjacent())
				{
					u = partial.getTo();
					if(u->state == VertexState::Black) continue;
					score = v->distance + partial.getWeight();
					if(u->state == VertexState::White)
					{
						u->state = VertexState::Gray;
					//	queue.push(u);
					}
					else if(score >= u->distance) continue;
					u->parent = v;
					u->distance = score;
					u->estimate = score + H(u, end);
					queue.push(QueueEntry(u, u->estimate));
				}
			}
		}

		double** FloydWarshall()
		{
			std::unordered_map<Vertex*, unsigned int> index;
			const auto V = this->graph.size();
			double** paths = new double*[V]();
			for(unsigned int i = 0; i < V; ++i)
			{
				paths[i]=new double[V]();
			}
			unsigned int ind = 0;
			for(auto v : this->graph)
			{
				index[v]=ind++;
			}
			for(unsigned int i = 0; i < V; ++i)
			{
				for(unsigned int j = 0; j < V; ++j)
				{
					paths[i][j] = std::numeric_limits<double>::infinity();
				}
			}
			for(auto v : this->graph)
			{
				unsigned I = index[v];
				paths[I][I] = 0;
				for(auto u : v->Adjacent())
				{
					paths[I][index[u.getTo()]]=u.getWeight();
				}
			}
 			for(unsigned int k = 0; k < V; ++k)
			{
				for(unsigned int i = 0; i < V; ++i)
				{
					for(unsigned int j = 0; j< V; ++j)
					{
						paths[i][j]=std::min(paths[i][j],paths[i][k]+paths[k][j]);
					}
				}
			}
			return paths;
		}
		
		bool TransitiveClosure();
// 		{
// 			const auto V = this->graph.size();
//			bool** closure = new bool[V][V]();
// 			for(unsigned int i = 0; i < V; ++i)
// 			{
// 				for(unsigned int j = 0;; j< V; ++j)
// 				{
// 					if(i==j || Edge(i,j)) closure[i][j] = true;
// 					else closure[i][j] = false;
// 				}
// 			}
// 			for(unsigned int k = 0; k < V; ++k)
// 			{
// 				for(unsigned int i = 0; i < V; ++i)
// 				{
// 					for(unsigned int j = 0;; j< V; ++j)
// 					{
// 						paths[i][j]=(paths[i][j] || (paths[i][k] && paths[k][j]));
// 					}
// 				}
// 			}
// 		}
	};
	
	
}
#endif // !CTL_GRAPH
