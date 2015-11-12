#ifndef _CTL_HEAP_SORT_HPP_
#define _CTL_HEAP_SORT_HPP_

namespace CTL
{	
	template<typename Iterator, typename Compare>
	void Heapify(Iterator first, Iterator last, Iterator i, Compare comp)
	{
//		typedef typename std::iterator_traits<Iterator>::value_type Type;
		Iterator Top = i;
		int Node = i-first;
		Iterator left=(first+(2*Node+1));
		if(left < last && comp(*Top,*left))
		{
			Top = left;
		}
		Iterator right=(first+(2*Node+2));
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
	
	template<typename Iterator, typename Compare>
	void BuildHeap(Iterator first, Iterator last, Compare comp)
	{
		Iterator i = first+(((last-first)/2)-1);
		while(i>=first)
		{
			CTL::Heapify(first,last,i--,comp);
		}
	}
	
	template<typename Iterator, typename Compare>
	void HeapSort(Iterator first, Iterator last, Compare comp)
	{
		CTL::BuildHeap(first, last, comp);
		while(last>first)
		{
			std::swap(*first,*(last-1));
			CTL::Heapify(first,--last,first,comp);
		}
	}
}
#endif