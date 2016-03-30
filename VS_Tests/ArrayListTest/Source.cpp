#include <vector>
#include <CTL/Container/ctl_arraylist.hpp>
#include <CTL/Utility/ctl_traits.hpp>

int main()
{
	std::cout << CTL::IsSizeEnabled<std::vector<int>>::value;
}