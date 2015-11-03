#include "ctl_data.hpp"
#include "ctl_pair.hpp"
#include "ctl_sort.hpp"
#include <iostream>
int main()
{
	CTL::Data<int> Set(std::cin);
	CTL::MergeSort(Set.begin(),Set.end(),std::less<int>());
	Set.Print();
	std::cout << std::endl;
}