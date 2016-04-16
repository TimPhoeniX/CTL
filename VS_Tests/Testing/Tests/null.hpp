#pragma once
#include <iostream>

class nulltest
{
public:
	int a, b;
	virtual ~nulltest() {};
	void print() const
	{
		std::cout << "Null Print";
	}
};

void test_null()
{
	nulltest* g = nullptr;

	g->print();
}

