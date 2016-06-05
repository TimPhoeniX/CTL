#include "CTL/Graph/Graph.hpp"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <random>

struct Cell
{
	size_t x;
	size_t y;

	template<typename os>
	friend os& operator <<(os& out, const Cell& c)
	{
		return out << '[' << c.x << ',' << c.y << ']';
	}

	friend bool operator<(const Cell& lhs, const Cell& rhs)
	{
		return (lhs.x == rhs.x ? lhs.y < rhs.y : lhs.x < rhs.x);
	}

	friend bool operator==(const Cell lhs, const Cell rhs)
	{
		return lhs.x == rhs.x&&lhs.y == rhs.y;
	}

	Cell up()
	{
		return Cell{ x, y + 1 };
	}

	Cell down()
	{
		return Cell{ x, y - 1 };
	}

	Cell left()
	{
		return Cell{ x - 1, y };
	}

	Cell right()
	{
		return Cell{ x + 1, y };
	}
};

using GraphMaze = CTL::Graph<Cell, CTL::Graphs::UndirectedExtern>;

class Maze
{
public:
	using Vertex = typename GraphMaze::Vertex;

private:
	size_t X, Y;
	Vertex* maze;

public:
	Maze(size_t x, size_t y) :X(x), Y(y), maze(new Vertex[X*Y])
	{
		for(y = 0; y < Y; ++y)
		{
			for(x = 0; x < X; ++x)
			{
				maze[y*X + x].SetLabel(Cell{ x,y });
			}
		}
	}

	~Maze()
	{
		delete[] maze;
	}

	Vertex& operator()(size_t x, size_t y)
	{
		return maze[y*X + x];
	}

	Vertex& operator()(Cell c)
	{
		return maze[c.y*X + c.x];
	}

	Vertex* begin()
	{
		return maze;
	}

	Vertex* end()
	{
		return maze + (X*Y);
	}

	size_t x() const
	{
		return this->X;
	}

	size_t y() const
	{
		return this->Y;
	}

};

GraphMaze Graphify(Maze& m)
{
	GraphMaze g;
	for(auto b = m.begin(), e = m.end(); b != e; ++b)
	{
		g.AddVertex(b);
	}
	const size_t X = m.x() - 1, Y = m.y() - 1;
	for(size_t y = 0; y < Y; ++y)
	{
		for(size_t x = 0; x < X; ++x)
		{
			g.AddEdge(&m(x, y), &m(x + 1, y));
			g.AddEdge(&m(x, y), &m(x, y + 1));
		}
		g.AddEdge(&m(X, y), &m(X, y + 1));
	}
	for(size_t x = 0; x < X; ++x)
	{
		g.AddEdge(&m(x, Y), &m(x + 1, Y));
	}
	return g;
}

void MakeImperfect(Maze& m, GraphMaze& g, const size_t x, const size_t y,const bool r)
{
	auto Breach = [&m, &g, x, y,r](Maze::Vertex* u) //Adds edges to nearby cells that are not yet connected with given cell
	{
		std::default_random_engine e;
		std::uniform_real_distribution<double> d(1., 5.);
		Cell t = u->Label();
		Cell c;
		bool MissingEdge = true;
		for(int i = 4; i; --i)
		{
			MissingEdge = true;
			switch(i)
			{
				case 4: c = t.up(); break;
				case 3: c = t.right(); break;
				case 2: c = t.down(); break;
				case 1: c = t.left(); break;
				default: break;
			}
			if(!(c.x < x && c.y < y)) continue;
			for(auto p : u->Adjacent())
			{
				if(p.getTo()->Label() == c)
				{
					MissingEdge = false;
					break;
				}
			}
			if(MissingEdge) g.AddEdge(u, &m(c.x, c.y),r?d(e):1.);
		}
	};
	size_t breaches = g.VertexCount() / 10;
	for(size_t i = 0; i < breaches; ++i)
	{
		Breach(&m(std::rand() % x, std::rand() % y));
	}
}

struct Taxi
{
	double operator()(Maze::Vertex* begin, Maze::Vertex* end)
	{
		const Cell& a = begin->Label();
		const Cell& b = end->Label();
		return double((a.x > b.x ? a.x - b.x : b.x - a.x) + (a.y > b.y ? a.y - b.y : b.y - a.y));
	}
};

struct Euclid
{
	double operator()(Maze::Vertex* begin, Maze::Vertex* end)
	{
		const Cell& a = begin->Label();
		const Cell& b = end->Label();
		double X = double(a.x > b.x ? a.x - b.x : b.x - a.x);
		double Y = double(a.y > b.y ? a.y - b.y : b.y - a.y);
		return std::sqrt(X*X + Y*Y);
	}
};

struct Dijkstra
{
	double operator()(Maze::Vertex*, Maze::Vertex*)
	{
		return 0;
	}
};

struct Exact
{
	static double* distance;
	static size_t x;
	double operator()(Maze::Vertex* begin, Maze::Vertex*)
	{
		const Cell& a = begin->Label();
		return distance[a.y*x + a.x];
	}
	static void Calculate(GraphMaze& g, Maze::Vertex* s)
	{
		g.Dijkstra(s);
		distance = new double[g.VertexCount()];
		Cell c;
		for(Maze::Vertex* v : g)
		{
			c = v->Label();
			distance[c.y*x + c.x] = v->Distance();
		}
	}
};
double* Exact::distance = nullptr;
size_t Exact::x = 0;

int main(int argc, char** argv)
{
	std::srand((unsigned int)(::time(nullptr)));
	std::cout.precision(10);
	size_t X = 5, Y = 5;
	bool P = false;
	bool B = false;
	bool C = false;
	bool R = false;
	Cell s, e;
	for(int arg = 1; arg < argc; ++arg)
	{
		switch(argv[arg][1])
		{
			case 'x':
				X = atoi(argv[++arg]);
				Exact::x = X;
				break;
			case 'y':
				Y = atoi(argv[++arg]);
				break;
			case 'p':
				P = true;
				break;
			case 'b':
				B = true;
				break;
			case 'c':
				s.x = atoi(argv[++arg]);
				s.y = atoi(argv[++arg]);
				e.x = atoi(argv[++arg]);
				e.y = atoi(argv[++arg]);
				C = true;
				break;
			case 'r':
				R = true;
			default:
				break;
		}
	}
	if(!C)
	{
		s = { 0,Y - 1 };
		e = { X - 1,0 };
	}
	std::cout << "Solving " << (P ? "Prim's " : "Kruskal's ") << (B ? "imperfect maze " : "perfect maze ") << (R?"with randomized weights\n":"\n");
	std::cout << "From " << s << " to " << e << std::endl;

	Maze m(X, Y);
	GraphMaze g = Graphify(m);
	auto end = std::chrono::high_resolution_clock::now();
	auto begin = std::chrono::high_resolution_clock::now();
	auto MazeEdges = (P ? g.PrimMaze() : g.KruskalMaze());
	end = std::chrono::high_resolution_clock::now();
	double Di = std::chrono::duration<double>(end - begin).count();
	std::cout << std::fixed << Di << "s " << (P ? "Prim" : "Kruskal") << " with " << X*Y << " Nodes" << std::endl;

	g.ClearEdges();
	if(R)
	{
		std::default_random_engine r;
		std::uniform_real_distribution<double> d(1., 5.);
		for(auto e : MazeEdges)
		{
			e.setWeight(d(r));
		}
	}
	g.AddEdges(MazeEdges);
	MazeEdges.clear();
	MazeEdges.shrink_to_fit();//No need for this anymore

	if(B) MakeImperfect(m, g, X, Y,R);

	auto Start = &m(s), End = &m(e);
	Exact::Calculate(g, End);

	begin = std::chrono::high_resolution_clock::now();
	g.Dijkstra(Start, End);
	end = std::chrono::high_resolution_clock::now();
	double Dij = std::chrono::duration<double>(end - begin).count();
	std::cout << std::fixed << Dij << "s " << "Dijkstra\n";

	Dij = 100. / Dij;

	begin = std::chrono::high_resolution_clock::now();
	g.AStar(Start, End, Dijkstra());
	end = std::chrono::high_resolution_clock::now();
	Di = std::chrono::duration<double>(end - begin).count();
	std::cout << std::fixed << Di << "s " << "A* using Dijkstra metric: " <<std::setprecision(2)<< Di*Dij << "% of Dijkstra" << std::endl;


	begin = std::chrono::high_resolution_clock::now();
	g.AStar(Start, End, Taxi());
	end = std::chrono::high_resolution_clock::now();
	Di = std::chrono::duration<double>(end - begin).count();
	std::cout << std::setprecision(10) << Di << "s " << "A* using Taxi metric: " <<std::setprecision(2)<< Di*Dij << "% of Dijkstra" << std::endl;


	begin = std::chrono::high_resolution_clock::now();
	g.AStar(Start, End, Euclid());
	end = std::chrono::high_resolution_clock::now();
	Di = std::chrono::duration<double>(end - begin).count();
	std::cout << std::setprecision(10) << Di << "s " << "A* using Euclid metric: " <<std::setprecision(2)<< Di*Dij << "% of Dijkstra" << std::endl;


	begin = std::chrono::high_resolution_clock::now();
	g.AStar(Start, End, Exact());
	end = std::chrono::high_resolution_clock::now();
	Di = std::chrono::duration<double>(end - begin).count();
	std::cout << std::setprecision(10) << Di << "s " << "A* using Exact metric: " <<std::setprecision(2)<< Di*Dij << "% of Dijkstra" << std::endl;


	delete[] Exact::distance;
}