#include <fstream>
#include <string>
#include <iostream>
#include <locale>
#include "CTL/Graph/Graph.hpp"
#include <functional>

#ifdef _WIN32
#define MYLOCALE "Polish_Poland.1250"
#include <windows.h>
#else
#define MYLOCALE "pl.UTF-8"
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
			auto l = site.find_first_not_of(" ");
			std::string newsite = site.substr(l,site.size()-l);
			to = g.FindVertex(newsite);
			g.AddEdge(v, to);
		}
		else
		{
			v = g.FindVertex(site);
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

	std::ifstream data("C:/plwiki.txt");
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