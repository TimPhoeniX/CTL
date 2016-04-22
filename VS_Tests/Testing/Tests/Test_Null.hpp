#pragma once
#include <iostream>

class nulltest
{
public:
	bool isNull()
	{
		return this == nullptr;
	}
};

void test_null()
{
	nulltest* ptr = nullptr;

	std::cout << "ptr is " << (ptr->isNull() ? "nullptr" : "nulltest*") << std::endl;
}

