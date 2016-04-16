#pragma once
#include <iostream>

class A
{
public:
	static char getID()
	{
		return 'A';
	}
};

class B : public A
{
public:
	static char getID()
	{
		return 'B';
	}
};

class C : public A
{

};

template<typename typ>
char getID()
{
	return typ::getID();
}

void test_static_func()
{
	std::cout <<
		getID<A>() << std::endl <<
		getID<B>() << std::endl <<
		getID<C>() << std::endl;
	A* a = new B();
	std::cout << a->getID() << std::endl;
}

/**
 * Conclusion : static function is inherited and will be called if not shadowed.
 */