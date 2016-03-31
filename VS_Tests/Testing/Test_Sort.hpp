#ifndef Test_Sort_HPP
#define Test_Sort_HPP
#include <iostream>
#include <random>
#include <algorithm>
#include <functional>


void test(std::function<void(int*,int*)> S)
{
	std::random_device r;
	int a[1024];
	generate(a, a + 1024, ref(r));
	S(a, a + 1024);
	std::cout << std::is_sorted(a, a + 1024) << std::endl;
}

void testc(std::function<void(int*, int*,std::greater<int>)> S)
{
	std::random_device r;
	int a[1024];
	generate(a, a + 1024, ref(r));
	S(a, a + 1024, std::greater<int>());
	std::cout << std::is_sorted(a, a + 1024, std::greater<int>()) << std::endl;
}
#endif // !Test_Sort_HPP

