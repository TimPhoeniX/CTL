#include <fstream>
#include <string>
#include <iostream>
#include <locale>
#include "CTL/Graph/Graph.hpp"
#include <functional>

#ifdef _WIN32
#define MYLOCALE "Polish_Poland.1250"
#define WIKI "C:/plwiki.txt"
#include <windows.h>
#else
#define MYLOCALE "pl_PL.UTF-8"
#define WIKI "plwiki.txt"
#endif

using Graph = CTL::Graph<std::string,CTL::Directed<std::string>>;
using Vertex = CTL::Vertex<std::string>;

void PopulateVertices(Graph& g, std::istream& in)
{
	std::string site = "";
	while(std::getline(in,site) && !site.empty())
	{
		if(site.front() == ' ') continue;
//		std::cout << "Read: " << site << std::endl;
		g.AddVertex(new Vertex(site));
	}
}

void PopulateEdges(Graph& g, std::istream& in)
{
	std::string site = "";
	std::getline(in, site);
	Vertex* v = g.FindVertex(site);
	Vertex* to = nullptr;
	while(std::getline(in, site) && !site.empty())
	{
		if(site.front()==' ')
		{
			site.erase(0,2);
			to = g.FindVertex(site);
//			std::cout << site << std::endl;
			g.AddEdge(v, to);
		}
		else
		{
			v = g.FindVertex(site);
			std::cout << "Now "+site << std::endl;
		}
	}
}

void printv(Vertex* vert)
{
	if(vert->Parent() != nullptr)
	{
		printv(vert->Parent());
	}
	std::cout << '[' << vert->Distance() << "] " << vert->Label() << std::endl;
};

int main()
{
	std::locale PL(MYLOCALE);
	std::locale::global(PL);
	std::cout.imbue(PL);
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
#else
#endif

	std::ifstream data(WIKI);
	data.imbue(PL);
	Graph g;
	PopulateVertices(g, data);
	data.clear();
	data.seekg(0);
	PopulateEdges(g, data);
	data.close();

	std::string from, to;
	Vertex* v = nullptr, *vto = nullptr;

	while(true)
	{
		std::cout << "Start from Where?" << std::endl;
		std::getline(std::cin, from);
		if(from.empty())
		{
			std::cout << "No site given" << std::endl;
			std::exit(0);
		}

		v = g.FindVertex(from);
		if(v == nullptr)
		{
			std::cout << "404: Site not found: " + from << std::endl;
			continue;
		}

		std::cout << "Wait while we prepare the graph for you" << std::endl;
		g.BFS(v);

		std::cout << "To Where?" << std::endl;

		while(std::getline(std::cin, to) && !to.empty())
		{
			vto = g.FindVertex(to);
			if(vto == nullptr)
			{
				std::cout << "404: Site not found: " << '|' << to << '|' << std::endl;
				std::cout << "To Where?" << std::endl;
				continue;
			}
			std::cout << "No of Jupms: " << vto->Distance() << std::endl;

			printv(vto);
		}
	}
}