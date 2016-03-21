#ifndef _CTL_HEAP_SORT_HPP_
#define _CTL_HEAP_SORT_HPP_
#include <type_info>
#include <iterator>

namespace CTL
{	
	template<typename RAIterator, typename Compare>
	void Heapify(RAIterator first, RAIterator last, RAIterator i, Compare comp)
	{
//		typedef typename std::iterator_traits<RAIterator>::value_type Type;
		RAIterator Top = i;
		int Node = i-first;
		RAIterator left=(first+(2*Node+1));
		if(left < last && comp(*Top,*left))
		{
			Top = left;
		}
		RAIterator right=(first+(2*Node+2));
		if(right < last && comp(*Top,*right))
		{
			Top = right;
		}
		if(Top!=i)
		{
			std::swap(*i,*Top);
			CTL::Heapify(first,last,Top,comp);
		}
	}
	
	template<typename RAIterator, typename Compare>
	void BuildHeap(RAIterator first, RAIterator last, Compare comp)
	{
		RAIterator i = first+(((last-first)/2)-1);
		while(i>=first)
		{
			CTL::Heapify(first,last,i--,comp);
		}
	}
	
	template<typename RAIterator, typename Compare>
	void HeapSort(RAIterator first, RAIterator last, Compare comp)
	{
		static_assert(std::is_base_of<std::random_acces_iterator_tag,std::iterator_traits<RAIterator>::iterator_category>::value,"HeapSort requires random acces iterator!");
		CTL::BuildHeap(first, last, comp);
		while(last>first)
		{
			std::swap(*first,*(last-1));
			CTL::Heapify(first,--last,first,comp);
		}
	}
}
#endif