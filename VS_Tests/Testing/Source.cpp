#include <random>
#include <functional>
#include <algorithm>
#include <CTL/Container/ctl_arraylist.hpp>
#include <CTL/Utility/ctl_traits.hpp>
#include <CTL/Sort/ctl_quick_sort.hpp>

int main()
{
	std::random_device r;
	std::uniform_int_distribution<> f;
	std::function<int()> roll = std::bind(f, std::ref(r));
	int a[1024];
	std::generate(a, a + 1024, roll );
	CTL::QuickSort(a, a + 1024);
	std::cout << std::is_sorted(a, a + 1024) << std::endl;
}