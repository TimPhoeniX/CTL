#include "ctl_sort.hpp"
#include "ctl_matrix.hpp"
#include <fstream>
#include <iostream>
#include <list>

class Tester
{
public:
	static unsigned int ID;
	unsigned int id;
	static int count;
	
	Tester() : id(ID++) { ++count; }
	Tester(const Tester& t) : id(t.id){ ++count;}
	Tester(Tester&& t) : id(t.id)
	{
		++count;
	}
	~Tester() { --count; }
	Tester& operator=(const Tester& t)
	{
		this->id=t.id;
		return *this;
	}
	Tester& operator=(Tester&& t)
	{
		this->id=t.id;
		return *this;
	}
	
	bool operator==(const Tester& t) const
	{
		return this->id==t.id;
	}
	
	friend std::ostream& operator<<(std::ostream& out,const Tester& t)
	{
		return out << t.id;
	}
};

unsigned int Tester::ID=0;
int Tester::count=0;

int main()
{
	std::ifstream File("matrices.dat");
	CTL::Matrix<double> m1(File);
	CTL::Matrix<double> m5(File);
	CTL::Matrix<double> m6(File);
	File.close();
	std::cout << m5 << '\n' << m6 << std::endl;
	auto m8 = m6*m5;
	std::cout << m8 << std::endl;
	return 0;
}