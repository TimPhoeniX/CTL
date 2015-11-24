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
	unsigned int* P1 = new unsigned int[5]{4,1,0,2,3};
	unsigned int* P2 = new unsigned int[5]{2,1,3,4,0};
// 	m.InplaceLU(P);
// 	for(int i = 0; i<5; ++i)
// 	{
// 		std::cout << i << ' ' << P[i] << std::endl;
// 	}
// 	std::cout << m << std::endl;
// 	auto m2 = m.ReassembleFromLU();
// 	std::cout << m2 << std::endl;
	std::cout << m.InplacePermutateRows(P1) << std::endl;
	std::cout << m.InplaceReversePermutateRows(P1) << std::endl;


	return 0;
}