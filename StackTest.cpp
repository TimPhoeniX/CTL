#include "ctl_stack.hpp"
#include <forward_list>
#include <list>
#include <iostream>
#include <chrono>
#include <thread>
#include "Utility/ctl_makearray.hpp"

int main()
{
	{
		std::forward_list<int> li;
		for (int i = 0; i < 10000000; ++i)
		{
			li.push_front(i);
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	{
		std::list<int> li;
		for (int i = 0; i < 10000000; ++i)
		{
			li.push_front(i);
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	auto p = CTL::MakeArray<std::unique_ptr<std::unique_ptr<int>[]>>(2, 3);
	return 0;
}