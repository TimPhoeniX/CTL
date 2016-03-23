#include "ctl_stack.hpp"
#include <forward_list>
#include <list>
#include <iostream>
#include <chrono>
#include <thread>
#include "Utility/ctl_makearray.hpp"
#include <Sort/ctl_quick_sort.hpp>

int main()
{
	std::list<int> list;
	list.resize(1000000);
	std::this_thread::sleep_for(std::chrono::seconds(15));
//	for(auto& i : list)
//	std::cout << &i << std::endl;
	return 0;
}