#ifndef PRINTHEAP_HPP
#define PRINTHEAP_HPP

namespace CTL
{
	template<typename it, typename os>
	void PrintHeap(it first, it last, os& out)
	{
		size_t lvl = 1;
		size_t current = 0;
		while (first != last)
		{
			out << (*(first++))->Distance() << ' ';
			if (current%2) std::cout << '|';
			current = (++current) % lvl;
			if (current == 0)
			{
				lvl *= 2;
				out << std::endl;
			}
		}
		std::cout << std::endl;
	}
}

#endif // !PRINTHEAP_HPP
