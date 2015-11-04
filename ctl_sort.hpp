/* 
	Conceptual Template Library by Piotr Grudzień
	Sorting algorithms 
*/

#ifndef _CTL_SORT_HPP_
#define _CTL_SORT_HPP_

#include <utility>
#include <iterator>

namespace CTL
{
	template<typename T, typename Compar>
	void MergeSort(T first, T last, Compar comp)
	{
		typedef typename std::iterator_traits<T>::value_type Type;
		if((last-first)>1)
		{
			T mid=first+(last-first)/2;
			MergeSort(first,mid,comp);
			MergeSort(mid,last,comp);
			T result = new Type[last-first];
			T left=first, right=mid, resultIt=result;
			while(left != mid && right != last)
			{
				
				if(comp(*(right),*(left)))
				{
					(*(resultIt++))=std::move(*(right++));
				}
				else
				{
					(*(resultIt++))=std::move(*(left++));
				}
			}
			while(left != mid)
			{
				(*(resultIt++))=std::move(*(left++));
			}
			while(right != last)
			{
				(*(resultIt++))=std::move(*(right++));
			}
			left = first; resultIt=result;
			while(left!=last)
			{
				(*(left++))=std::move(*(resultIt++));
			}
			delete[] result;
		}
	}
	
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