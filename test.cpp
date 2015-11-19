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
	CTL::Matrix<double> m2(File);
	File.close();
	std::cout << m2 << std::endl;
	m2.QRAlgorithmSymmetricalTridiagonal();
	m2.PrintDiagonal();
	std::cout << '\n' << std::endl;
	return 0;
}