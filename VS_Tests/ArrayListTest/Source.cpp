#include <CTL/Container/ctl_arraylist.hpp>

template class CTL::ArrayList<int>;

int main()
{
	CTL::ArrayList<int> s;
	CTL::ArrayList<int> o(s);
	std::cout << s.size();
}