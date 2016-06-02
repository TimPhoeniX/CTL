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

using GraphMaze = CTL::Graph<Cell>;

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

void main(int argc, char** argv)
{
	size_t X = 10, Y = 10;
	if (argc > 2)
	{
		X = std::atoll(argv[1]);
		Y = std::atoll(argv[2]);
	}
	Maze m(5, 5);
	GraphMaze g = Graphify(m);
	g.BFS(&m(0, 0));
	g.PrintPaths(&m(0, 0),std::cout);
}