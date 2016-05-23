//#include "Graph.hpp"
#include <iostream>
#include <string>
#include <limits>
#include "CTL/Graph/Graph.hpp"

int main()
{
	CTL::Graph<char,CTL::Directed<char> > g;
	unsigned int V = 0;
	std::cin >> V;
	char a, b;
	for(unsigned int i = 0; i < V; ++i)
	{
		std::cin >> a;
		g.AddVertex(a);
	}
	std::cin >> V;
	double w = 0.;
	for(unsigned int i = 0; i < V; ++i)
	{
		std::cin >> a >> b >> w;
		g.AddEdge(g.FindVertex(a),g.FindVertex(b), w);
	}
	auto paths = g.FloydWarshall();
	V = g.VertexCount();
	for(unsigned int i = 0; i < V; ++i)
	{
		for(unsigned int j = 0; i < V; ++i)
		{
			std::cout << paths[i][j] << ' ';
		}
		std::cout << '\n';
	}
}