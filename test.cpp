#include "ctl_sort.hpp"
#include "ctl_data.hpp"
#include "ctl_pair.hpp"
#include "ctl_list.hpp"
#include <iostream>

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
	CTL::ArrayList<Tester> List;
	for(int i = 0; i<10; ++i)
	{
		List.PushBack(Tester());
	}
	std::cout << List << std::endl;
	List.Insert(5,List.PopBack());
	List.Erase(0);
	std::cout << List << std::endl;
	std::cout << *(List.Find(List[5])) << std::endl;
	std::cout << Tester::count << std::endl;
	return 0;
}