#include<CTL/ctl_tester.hpp>
#include<CTL/Utility/ctl_singleton.hpp>

int main()
{
	using Test = CTL::ArcSingleton<Tester>;
	
	Test* a = new Test[50];

	std::cout << Tester::count << ' ' << a[0].GetCount() << std::endl;
	delete[] a;
	std::cout << Tester::count << ' ' << a[0].GetCount() << std::endl;

}