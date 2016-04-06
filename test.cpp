#include "Graph.hpp"
#include <iostream>
#include <string>

template class Vertex<std::string>;
template class Graph<std::string>;

int main()
{
	Vertex<std::string> A("A");
	Vertex<std::string> B("B");
	
	std::cout << "Graph" << std::endl;
	Graph<std::string> G;
	
	std::cout << "adding edge" << std::endl;
	A.AddEdge(&B);
	
	std::cout << "Adding Vertex" << std::endl;
	G.AddVertex(&A);
	G.AddVertex(&B);
	
	std::cout << "Graph" << std::endl;
	
	for(auto& v : G)
	{
		std::cout << v->getLabel() << std::endl;
	}
	
	return 0;
}