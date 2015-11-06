/* 
	Conceptual Template Library by Piotr Grudzień
	QuickSort 
*/

#ifndef _CTL_QUICK_SORT_HPP_
#define _CTL_QUICK_SORT_HPP_

#include <iterator>

namespace CTL
{
	template<typename T, typename Compar>
	void QuickSort(T first, T last, Compar comp)
	{
		typedef typename std::iterator_traits<T>::value_type Type;
		if((last-first)>1)
		{
			Type Pivot = *(last-1);
			T iIt = first, jIt = first;
			while(jIt!=(last-1))
			{
				if(comp(*jIt, Pivot))
				{
					std::swap(*iIt,*jIt);
					++iIt;
				}
				++jIt;
			}
			std::swap(*iIt,*(last-1));
			QuickSort(first,iIt,comp);
			QuickSort(iIt+1,last,comp);
		}
	}
}

#endif