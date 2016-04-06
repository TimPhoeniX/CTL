#include "Container/ctl_forwardlist.hpp"
#include "Container/ctl_arraylist.hpp"
#include "ctl_pair.hpp"

enum class VertexState
{
	White,
	Black,
	Gray
};

template<typename T>
class Vertex
{
public:
	using VertexList = CTL::ForwardList<Vertex<T>*>;
	
private:
	T Label;
	size_t distance = 0;
//	VertexState = VertexState::White;
	VertexList vList;
	
public:
	Vertex(const T& label):
		Label(label)
	{}
	
	void AddEdge(Vertex<T>* vertex)
	{
		this->vList.PushFront(vertex);
//		vertex->AddDirectedEdge(this);
	}
	
	void AddDirectedEdge(Vertex<T>* vertex)
	{
		this->vList.PushFront(vertex);
	}
	
	const T& getLabel()
	{
		return this->Label;
	}
	
	VertexList& Adjacent()
	{
		return this->vList;
	}
};
	
template<typename T>
class Graph
{
public:
	using GraphType = CTL::ForwardList<Vertex<T>*>;
	
private:
	GraphType graph;
	
public:
	typename GraphType::iterator begin()
	{
		return this->graph.begin();
	}
	
	typename GraphType::iterator end()
	{
		return this->graph.end();
	}
	
	void AddVertex(Vertex<T>* v)
	{
		this->graph.PushBack(v);
	}
	
	Vertex<T>* find(const T& label)
	{
		for(auto v : this->graph)
		{
			if(v->getLabel()==label) return v;
		}
		return nullptr;
	}
};