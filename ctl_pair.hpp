/* 
	Conceptual Template Library by Piotr Grudzień
	A simple pair implementation.
*/

#ifndef _CTL_PAIR_HPP_
#define _CTL_PAIR_HPP_

#include <iostream>
#include <utility>

namespace CTL
{
	template<typename A, typename B>
	class Pair
	{
	private:
		A First;
		B Second;
	public:
		std::istream& operator>>(std::istream& stream)
		{
			return stream >> this->First >> this->Second;
		}
		
		friend std::istream& operator>>(std::istream& stream, CTL::Pair<A,B>& pair)
		{
			return pair.operator>>(stream);
		}
		
		std::ostream& operator<<(std::ostream& stream) const
		{
			return stream << this->First << " " << this->Second;
		}
		
		friend std::ostream& operator<<(std::ostream& stream, const CTL::Pair<A,B>& pair)
		{
			return pair.operator<<(stream);
		}
		
		Pair()
		{
		}
		
		Pair(A first, B second)
		: First(first), Second(second)
		{
		}
		
		Pair(CTL::Pair<A,B>&& pair)
		: First(std::move(pair.First)), Second(std::move(pair.Second))
		{
		}
		
		Pair(const CTL::Pair<A,B>& pair)
		: First(pair.First), Second(pair.Second)
		{
		}
		
		CTL::Pair<A,B>& operator=(const CTL::Pair<A,B>& pair)
		{
			if(this != &pair)
			{
				this->First=pair.First;
				this->Second=pair.Second;
			}
			return *this;
		}
		
		CTL::Pair<A,B>& operator=(const CTL::Pair<A,B>&& pair)
		{
			if(this != &pair)
			{
				this->First=std::move(pair.First);
				this->Second=std::move(pair.Second);
			}
			return *this;
		}
		
		const A& GetFirst() const 
		{
			return this->First;
		}
		
		const B& GetSecond() const
		{
			return this->Second;
		}
		
		void Swap(CTL::Pair<A,B> pair)
		{
			std::swap(this->First,pair.First);
			std::swap(this->Second,pair.Second);
		}		
	};
}
#endif