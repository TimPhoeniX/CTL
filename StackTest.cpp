#include "ctl_stack.hpp"
#include <forward_list>
#include <list>
#include <iostream>
#include <chrono>
#include <thread>
#include "Utility/ctl_makearray.hpp"

int main()
{
	CTL::Stack<int, std::forward_list<int>> stack;
	stack.push(5);
	std::cout << stack.empty() << std::endl;
	return 0;
}