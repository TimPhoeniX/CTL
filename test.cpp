//#include "Graph.hpp"
#include <iostream>
#include <string>
#include <limits>

int main()
{
	double a = std::numeric_limits<double>::max();
	double b = std::numeric_limits<double>::infinity();
	double c = b+1;
	std::cout << (a < c? "OK":"Overflow")<<std::endl; 
	return 0;
}