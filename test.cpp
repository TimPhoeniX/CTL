//#include "Graph.hpp"
#include <iostream>
#include <string>
#include <limits>
#include <iterator>
#include <cassert>
#include "CTL/Graph/Graph.hpp"

int main()
{
	CTL::Graph<char, CTL::Undirected<char> > g;\
	unsigned int V = 0;
	std::cin >> V;
	char a, b;
	for (unsigned int i = 0; i < V; ++i)
	{
		std::cin >> a;
		g.AddVertex(a);
	}
	std::cin >> V;
	double w = 0.;
	for (unsigned int i = 0; i < V; ++i)
	{
		std::cin >> a >> b >> w;
		g.AddEdge(g.FindVertex(a), g.FindVertex(b), w);
	}
	w = 0.;
	//	auto mst = g.KruskalMST();
	g.PrimMST();
	auto mst = g.PrimMSTE(w);
	std::cerr << (g.ConnectedComponentTest(mst) ? "OK\n" : "Error\n");
	CTL::QuickSort(mst.begin(), mst.end(), [](const CTL::Edge<char>& lhs, const CTL::Edge<char>& rhs)->bool
	{
		return lhs.getFrom()->Label() < rhs.getFrom()->Label()
			|| (!(rhs.getFrom()->Label() < lhs.getFrom()->Label())
				&& lhs.getTo()->Label() < rhs.getTo()->Label());
	});

	for (auto e : mst)
	{
		std::cout << e.getFrom()->Label() << '-' << e.getTo()->Label() << ' ' << e.getWeight() << '\n';
		w += e.getWeight();
	}
	std::cout << w << std::endl;
	g.Dijkstra(g.FindVertex('A'));
	g.PrintPaths(g.FindVertex('A'),std::cout);
}