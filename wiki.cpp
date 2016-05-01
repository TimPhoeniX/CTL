#include <fstream>
#include <string>
#include <iostream>
#include <locale>
#include "CTL/Graph/Graph.hpp"
#include <functional>
#include <cctype>
#include <random>
#include <chrono>
#include <functional>

#ifdef _WIN32
#define FASTLOAD
#define MYLOCALE "Polish_Poland.1250"
#define WIKI "C:/plwiki.txt"
#include <windows.h>
#else
#define MYLOCALE "pl_PL.UTF-8"
#define WIKI "plwiki.txt"
#endif


using Graph = CTL::Graph<std::string,CTL::Directed<std::string>>;
using Vertex = CTL::Vertex<std::string>;

#if defined(FASTLOAD)

#include <unordered_map>
std::unordered_map<std::string,Vertex*> map;

#endif

void PopulateVertices(Graph& g, std::istream& in)
{
	std::string site = "";
	while(std::getline(in,site) && !site.empty())
	{
		if(site.front() == ' ') continue;
//		std::cout << "Read: " << site << std::endl;
		Vertex* v = new Vertex(site);
#if defined(FASTLOAD)
		map.insert(make_pair(site,v));
#endif
		g.AddVertex(v);
	}
}

void PopulateEdges(Graph& g, std::istream& in)
{
	std::locale PL(MYLOCALE);
	bool debug = false;
	std::string site = "";
	std::getline(in, site);
	Vertex* v = g.FindVertex(site);
	Vertex* to = nullptr;
	while(std::getline(in, site) && !site.empty())
	{
		
		if(site.front()==' ')
		{
			site.erase(0,2);
			if(debug) std::cout << v->Label()+" leads to "+site << std::endl;
#if defined(FASTLOAD)
			auto it = map.find(site);
			if(it!=map.end())
			{
				to = it->second;
			}
			else
			{
				site.front()=char(std::toupper(site.front(),PL));
//				if(debug) std::cout << site << ' ' << v->Label() << std::endl;
				it = map.find(site);
				if(it!=map.end())
				{
//					if(debug) std::cout << it->first << std::endl;
					to = it->second;
				}
				else to = nullptr;
			}
#else
			to = g.FindVertex(site);
#endif
//			std::cout << site << std::endl;
//			if(debug) std::cout << "Adding "+to->Label()+" to "+v->Label() << std::endl;
			g.AddEdge(v, to);
		}
		else
		{
			debug = false;
			if(site=="Nyan Cat")
			{
				debug = true;
				std::cout << site << std::endl;
			}
				
#if defined(FASTLOAD)
//			std::cout << "Loading " << site << std::endl;
			auto it = map.find(site);
			if(it!=map.end())
			{
				v = it->second;
			}
			else
			{
				v = nullptr;
			}
#else
			v = g.FindVertex(site);
#endif
		}
	}
}

void printv(Vertex* vert)
{
	if(vert->Parent() != nullptr)
	{
		printv(vert->Parent());
	}
	if(vert->Distance() == -1) std::cout << "No Path to "+vert->Label() << std::endl;
	else std::cout << '[' << vert->Distance() << "] " << vert->Label() << std::endl;
};

int main()
{
//	std::cout << FASTLOAD << std::endl;
#if defined(FASTLOAD)
	std::cout << "Fast Version" << std::endl;
#else
	std::cout << "Slow Version" << std::endl;
#endif

	std::locale PL(MYLOCALE);
	std::locale::global(PL);
	std::cout.imbue(PL);

	std::ifstream data(WIKI);
	data.imbue(PL);
	Graph g;
	PopulateVertices(g, data);
	data.clear();
	data.seekg(0);
	PopulateEdges(g, data);
	data.close();

	std::random_device seeder;
	std::default_random_engine def(seeder());
	std::uniform_int_distribution<> verts(0,map.size());
	std::function<int()> limit = std::bind(verts,def);
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
			if(to=="Adjacent")
			{
				for(auto edge : v->Adjacent())
				{
					std::cout << edge->Label() << std::endl;
				}
				continue;
			}
			if(to=="Longest")
			{
				int max = 0;
				for(auto& i : map)
				{
					if(i.second->Distance()>max)
					{
						vto = i.second;
						max = vto->Distance();
					}
				}
			}
			else if(to=="Random")
			{
				auto rnd = map.begin();
				std::advance(rnd, limit() );
				vto = rnd->second;
			}
			else
			{
				vto = g.FindVertex(to);
				if(vto == nullptr)
				{
					std::cout << "404: Site not found: " << '|' << to << '|' << std::endl;
					std::cout << "To Where?" << std::endl;
					continue;
				}
			}
			std::cout << "No of Jupms: " << vto->Distance() << std::endl;

			printv(vto);
			
			std::cout << "To Where?" << std::endl;
		}
	}
}