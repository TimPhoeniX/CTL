#include <iostream>
#include <string>
#include <limits>
#include <iterator>
#include <cassert>
#include <vector>
#include "CTL/Graph/Graph.hpp"

class A
{
	int a = 5;
public:
	int getA() const
	{
		return this->a;
	}
};
class B
{
	int b = 4;
public:
	int getB()
	{
		return this->b;
	}

};

template<typename T,typename R, typename ...Args>
union MemberToFunction
{
	using mfptr = R(T::*)(Args...);
	using fptr = R(*)(T*,Args...);
	mfptr mptr;
	fptr ptr;
public:
	R operator()(T* t, Args... args)
	{
		return this->ptr(t, std::forward<Args>(args)...);
	}

	fptr getPtr()
	{
		return this->ptr;
	}
};
template<typename T, typename R, typename ...Args>
union MemberToFunctionConst
{
	using mfptr = R(T::*)(Args...) const;
	using fptr = R(*)(const T*, Args...);
	mfptr mptr;
	fptr ptr;
public:
	R operator()(const T* t, Args... args)
	{
		return this->ptr(t, std::forward<Args>(args)...);
	}

	fptr getPtr()
	{
		return this->ptr;
	}
};
template<typename T, typename R, typename ...Args>
MemberToFunction<T,R,Args...> MakeMemberToFunction(R(T::*mptr)(Args...))
{
	return MemberToFunction<T, R, Args...>{mptr};
}
template<typename T, typename R, typename ...Args>
MemberToFunctionConst<T, R, Args...> MakeMemberToFunction(R(T::*mptr)(Args...) const)
{
	return MemberToFunctionConst<T, R, Args...>{mptr};
}

using getint = int(*)(A*);
using getBint = int(B::*)();

int main()
{
	A a;
	B b;
	getBint get = (int(B::*)())(&A::getA);
	std::cout << sizeof(getint) << std::endl;
	std::cout << (b.*get)() << std::endl;

	std::cout << *(reinterpret_cast<int*>(&a)) << std::endl;

	auto va = MakeMemberToFunction(&A::getA);
	std::cout << va(&a) << std::endl;
	std::cout << std::endl;
}