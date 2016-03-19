#include "ctl_stack.hpp"
#include <forward_list>
#include <list>
#include <iostream>

int main()
{
	{
		CTL::Stack<int, std::forward_list<int>> li;
		for(int i = 0; i < 1000000; ++i)
		{
			li.push(i);
		}
		std::cout << li.top() << std::endl;
	}

	{
		CTL::Stack<int, std::list<int>> li;
		for(int i = 0; i < 1000000; ++i)
		{
			li.push(i);
		}
		std::cout << li.top() << std::endl;
	}

	return 0;
}