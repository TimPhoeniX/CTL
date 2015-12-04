#include "ctl_matrix.hpp"
#include "ctl_stack.hpp"
#include "ctl_proto_hashmap.hpp"
#include <fstream>
#include <iostream>
#include <list>

int main()
{
	CTL::Matrix<double> m(6,6);
	auto data = m[0];
	for(double i :
	{	19./12.,	13./12.,	10./12.,	10./12.,	13./12.,	-17/12.,
		13./12.,	13./12.,	10./12.,	10./12.,	-11./12.,	13./12.,
		10./12.,	10./12.,	10./12.,	-2./12.,	10./12.,	10./12.,
		10./12.,	10./12.,	-2./12.,	10./12.,	10./12.,	10./12.,
		13./12.,	-11./12.,	10./12.,	10./12.,	13./12.,	13./12.,
		-17./12.,	13./12.,	10./12.,	10./12.,	13./12.,	19./12.})
	{
		*(data++)=i;
	}
	auto E1 = m.LeadingEigenvector();
	std::cout << E1 << std::endl;
	auto E2 = m.SubleadingEigenvector(E1);
	std::cout << E2 << std::endl;
	return 0;
}