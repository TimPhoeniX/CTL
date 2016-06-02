#include<CTL/Graph/Graph.hpp>
#include <iostream>

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
};

using GraphMaze = CTL::Graph<Cell,CTL::Graphs::UndirectedExtern>;

class Maze
{
public:
	using Vertex = CTL::Vertex<Cell>;

private:
	size_t X, Y;
	Vertex* maze;

public:
	Maze(size_t x, size_t y) :X(x), Y(y), maze(new Vertex[X*Y])
	{
		for (y = 0; y < Y; ++y)
		{
			for (x = 0; x < X; ++x)
			{
				maze[y*Y + x].SetLabel(Cell{ x,y });
			}
		}
	}

	~Maze()
	{
		delete[] maze;
	}

	Vertex& operator()(size_t x, size_t y)
	{
		return maze[y*Y + x];
	}

	Vertex& operator()(Cell c)
	{
		return maze[c.y*Y + c.x];
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
	for (auto b = m.begin(), e = m.end(); b != e; ++b)
	{
		g.AddVertex(b);
	}
	const size_t X = m.x() - 1, Y = m.y()-1;
	for (size_t y = 0; y < Y; ++y)
	{
		for (size_t x = 0; x < X; ++x)
		{
			g.AddEdge(&m(x, y), &m(x + 1, y));
			g.AddEdge(&m(x, y), &m(x, y + 1));
		}
		g.AddEdge(&m(X, y), &m(X , y+1));
	}
	for (size_t x = 0; x < X; ++x)
	{
		g.AddEdge(&m(x, Y), &m(x + 1, Y));
	}
	return g;
}

int main(int argc, char** argv)
{
	size_t X = 10, Y = 10;
	if (argc > 2)
	{
		X = std::atoll(argv[1]);
		Y = std::atoll(argv[2]);
	}
	Maze m(X, Y);
	GraphMaze g = Graphify(m);
	auto mst_e = g.KruskalMST();
	g.ClearEdges();
	g.AddEdges(mst_e);
	g.Dijkstra(&m(0, 0));
	g.PrintPath(&m(0, 0), &m(X - 1, Y - 1), std::cout);
}