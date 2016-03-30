#ifndef CTL_HEAP_SORT_HPP
#define CTL_HEAP_SORT_HPP
#include <iterator>

namespace CTL
{	
	template<typename RAIterator, typename Compare>
	void Heapify(RAIterator first, RAIterator last, RAIterator i, Compare comp)
	{
		using std::swap;
//		typedef typename std::iterator_traits<RAIterator>::value_type Type;
		RAIterator Top = i;
		typename std::iterator_traits<RAIterator>::difference_type Node = i-first;
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
			swap(*i,*Top);
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
		using std::swap;
		static_assert(std::is_base_of<std::random_access_iterator_tag,typename std::iterator_traits<RAIterator>::iterator_category>::value,"HeapSort requires a random acces iterator!");
		CTL::BuildHeap(first, last, comp);
		while(last>first)
		{
			swap(*first,*(last-1));
			CTL::Heapify(first,--last,first,comp);
		}
	}
}
#endif