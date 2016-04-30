#pragma once
#include <iostream>

class nulltest
{
public:
	bool isNull() const
	{
		return this == nullptr;
	}
};

void test_null()
{
	nulltest* ptr = nullptr;
	bool (nulltest::*f)() const = &nulltest::isNull;

	std::cout << "ptr is " << ( (ptr->*f) () ? "nullptr" : "nulltest*") << std::endl;
	std::cout << "ptr is " << ( (*ptr.*f) () ? "nullptr" : "nulltest*") << std::endl;
	std::cout << "ptr is " << (ptr->isNull() ? "nullptr" : "nulltest*") << std::endl;
}

