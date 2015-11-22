#include "ctl_sort.hpp"
#include "ctl_matrix.hpp"
#include <fstream>
#include <iostream>
#include <list>

int main()
{
	std::ifstream file("matrices.txt");
	CTL::Matrix<double> m(file);
	file.close();
	unsigned int* P = new unsigned int[4]{};
	m.InplaceLU(P);
	std::cout << m << std::endl;
	auto m2 = m.ReassembleFromLU();
	std::cout << m2 << std::endl;
	std::cout << m2.InplacePermutateRows(P) << std::endl;
	return 0;
}