#include "ctl_stack.hpp"
#include <forward_list>
#include <list>
#include <iostream>
#include <chrono>
#include <thread>
#include "Utility/ctl_makearray.hpp"

int main()
{
	std::list<int> list;
	list.resize(100);
	for(auto& i : list)
	std::cout << &i << std::endl;
	return 0;
}