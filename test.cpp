#include "ctl_sort.hpp"
#include "ctl_data.hpp"
#include "ctl_pair.hpp"
#include <iostream>
int main()
{
	CTL::Data<int> Set(std::cin);
	CTL::HeapSort(Set.begin(),Set.end(),std::less<int>());
	std::cout << Set << std::endl;
	CTL::HeapSort(Set.begin(),Set.end(),std::greater<int>());
	std::cout << Set << std::endl;
}