#include "ctl_matrix.hpp"
#include "ctl_stack.hpp"
#include <fstream>
#include <iostream>
#include <list>

int main()
{
	std::ifstream file("matrices.txt");
	CTL::Matrix<double> m(file);
	file.close();
	unsigned int* P = new unsigned int[5]{};
	m.InplaceLU(P);
	for(int i = 0; i<5; ++i)
	{
		std::cout << i << ' ' << P[i] << std::endl;
	}
	std::cout << m << std::endl;
	auto m2 = m.ReassembleFromLU();
	std::cout << m2 << std::endl;
	std::cout << m2.InplacePermutateRows(P) << std::endl;
	unsigned int* P1 = new unsigned int[5] {4,3,0,1,2};
	std::cout << m2.InplacePermutateCols(P1) << std::endl;
	unsigned int* P2 = new unsigned int[5] {2,3,4,1,0};
	std::cout << m2.InplacePermutateCols(P2) << std::endl;
	return 0;
}