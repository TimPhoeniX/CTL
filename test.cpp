#include "ctl_virtual_disjointset.hpp"

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		std::cout << "Illegal argc!\n";
		return 0;
	}
	CTL::ForwardList<CTL::DNode<char>*> nodes;
	int n = 0;
	std::cin >> n;
	for(int i = 0; i < n; ++i)
	{
		char c = ' ';
		std::cin >> c;
		if(argv[1][0]=='t')
		{
			nodes.PushBack(new CTL::InTree<char>(c));
		}
		else if(argv[1][0]=='l')
		{
			nodes.PushBack(new CTL::DListNode<char>(c));
		}
		else
		{
			std::cout << "Illegal argument\n";
			return 0;
		}
	}
	CTL::ForwardList<CTL::Edge<char>> edges;
	std::cin >> n;
	for(int i = 0; i < n; ++i)
	{
		char a = ' ';
		char b = ' ';
		std::cin >> a >> b;
		CTL::Edge<char> e;
		for(auto& v : nodes)
		{
			if(v->getVertex() == a) e.GetFirst() = v;
			if(v->getVertex() == b) e.GetSecond() = v;
		}
		edges.PushBack(e);
	}

	auto comps = ConnectedComponents(edges, nodes);

	for(auto s : comps)
	{
		s->PrintSet();
	}

	return 0;
}