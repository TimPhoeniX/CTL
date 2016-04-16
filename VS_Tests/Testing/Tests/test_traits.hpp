#pragma once
#include <CTL/Utility/ctl_traits.hpp>
#include <forward_list>
#include <vector>
#include <list>
#include <iostream>

void test_traits()
{
	using tv = std::vector<int>;
	using tf = std::forward_list<int>;
	using tl = std::list<int>;
	std::cout << CTL::IsSizeEnabled<tv>::value << CTL::IsBackEnabled<tv>::value << CTL::IsFrontEnabled<tv>::value << std::endl;
	std::cout << CTL::IsSizeEnabled<tl>::value << CTL::IsBackEnabled<tl>::value << CTL::IsFrontEnabled<tl>::value << std::endl;
	std::cout << CTL::IsSizeEnabled<tf>::value << CTL::IsBackEnabled<tf>::value << CTL::IsFrontEnabled<tf>::value << std::endl;
}