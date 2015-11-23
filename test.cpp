#include "ctl_matrix.hpp"
#include "ctl_stack.hpp"
#include <fstream>
#include <iostream>
#include <list>

int main()
{
// 	std::ifstream file("matrices.txt");
// 	CTL::Matrix<double> m(file);
// 	file.close();
// 	unsigned int* P = new unsigned int[4]{};
// 	m.InplaceLU(P);
// 	std::cout << m << std::endl;
// 	auto m2 = m.ReassembleFromLU();
// 	std::cout << m2 << std::endl;
// 	std::cout << m2.InplacePermutateRows(P) << std::endl;
	CTL::Stack<int> s;
	s.Push(1);
	s.Push(2);
	auto s1 = CTL::Stack<int>(std::move(s));
	std::cout << s1.GetSize() << std::endl;
	while(!s1.Empty())
		std::cout << s1.Pop() << std::endl;
	return 0;
}