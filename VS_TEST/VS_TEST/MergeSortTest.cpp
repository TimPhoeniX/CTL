#include <Sort/ctl_merge_sort.hpp>
#include <ctl_tester.hpp>
#include <iostream>

int main()
{
	auto tests = new Tester[9];
	CTL::MergeSort(tests,tests + 9, std::less<Tester>());
	for(int i = 0; i < 9; ++i)
	{
		std::cout << tests[i] << std::endl;
	}
	std::cout << Tester::ID << ' ' << Tester::count << std::endl;
}