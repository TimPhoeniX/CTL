#include "ctl_sort.hpp"
#include "ctl_data.hpp"
#include "ctl_pair.hpp"
#include "ctl_arraylist.hpp"
#include "ctl_forwardlist.hpp"
#include "ctl_linkedlist.hpp"
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
	CTL::LinkedList<int> List;
	List.PushFront(0);
	List.PushFront(1);
	List.PushFront(2);
	List.PushBack(3);
	List.Insert(4,4);
	List.Insert(0,5);
	List.Insert(3,6);
	List.Erase(1);
	std::cerr << "PostErase" << std::endl;
//	std::cout << *List.Find(3) << std::endl;
//	std::cout << List.Get(3) << std::endl;
	std::cout << List.GetSize() << std::endl;
	std::cout << List << std::endl;
	std::cout << List.PopBack() << std::endl;
	std::cout << List.PopFront() << std::endl;
	std::cout << List.Get(3) << std::endl;
	List.Clear();
	std::cout << List.Empty() << std::endl;
// 	CTL::ForwardList<Tester> List1;
// 	List1.PushFront(Tester());
// 	List1.PushFront(Tester());
// 	List1.PushFront(Tester());
// 	List1.PushBack(Tester());
// 	List1.Insert(4,Tester());
// 	List1.Insert(0,Tester());
// 	List1.Insert(3,Tester());
// 	std::cout << List.Get(3) << std::endl;
// 	std::cout << List1.GetSize() << std::endl;
// 	std::cout << List1 << std::endl;
// 	std::cout << Tester::count << std::endl;
	return 0;
}