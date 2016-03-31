#define _SCL_SECURE_NO_WARNINGS
#include "Test_Sort.hpp"

#include <random>
#include <functional>
#include <algorithm>
#include <CTL/Container/ctl_arraylist.hpp>
#include <CTL/Utility/ctl_traits.hpp>
#include <CTL/Sort/ctl_quick_sort.hpp>
#include <Sort/ctl_heap_sort.hpp>
#include <Sort/ctl_merge_sort.hpp>

#include <forward_list>


template class std::forward_list<int>;



int main()
{
//	test(CTL::HeapSort<int*>);
	auto a = CTL::HeapSort<int*, std::greater<int>>;
	testc(a);
}