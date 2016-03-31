#ifndef CTL_HEAP_SORT_HPP
#define CTL_HEAP_SORT_HPP
#include <iterator>

namespace CTL
{
	namespace HeapSortHelper
	{
		template<typename RAIterator>
		void Heapify(RAIterator first, RAIterator last, RAIterator i)
		{
			using std::swap;
			typename std::iterator_traits<RAIterator>::difference_type Node = i - first;
			RAIterator Top = i, left = (first + (2 * Node + 1)), right = (first + (2 * Node + 2));
			if(left < last && (*Top < *left))
			{
				Top = left;
			}
			if(right < last && (*Top < *right))
			{
				Top = right;
			}
			if(Top != i)
			{
				swap(*i, *Top);
				Heapify(first, last, Top);
			}
		}

		template<typename RAIterator>
		void BuildHeap(RAIterator first, RAIterator last)
		{
			RAIterator i = first + (((last - first) / 2) - 1);
			while(i >= first)
			{
				Heapify(first, last, i--);
			}
		}

		template<typename RAIterator, typename Compare>
		void Heapify(RAIterator first, RAIterator last, RAIterator i, Compare comp)
		{
			using std::swap;
			typename std::iterator_traits<RAIterator>::difference_type Node = i - first;
			RAIterator Top = i, left = (first + (2 * Node + 1)), right = (first + (2 * Node + 2));
			if(left < last && comp(*Top, *left))
			{
				Top = left;
			}
			if(right < last && comp(*Top, *right))
			{
				Top = right;
			}
			if(Top != i)
			{
				swap(*i, *Top);
				Heapify(first, last, Top, comp);
			}
		}

		template<typename RAIterator, typename Compare>
		void BuildHeap(RAIterator first, RAIterator last, Compare comp)
		{
			RAIterator i = first + (((last - first) / 2) - 1);
			while(i >= first)
			{
				Heapify(first, last, i--, comp);
			}
		}
	}

	template<typename RAIterator>
	void HeapSort(RAIterator first, RAIterator last)
	{
		using std::swap;
		static_assert(std::is_base_of<std::random_access_iterator_tag, typename std::iterator_traits<RAIterator>::iterator_category>::value, "HeapSort requires a random acces iterator!");
		HeapSortHelper::BuildHeap(first, last);
		while(last > first)
		{
			swap(*first, *(last - 1));
			HeapSortHelper::Heapify(first, --last, first);
		}
	}

	template<typename RAIterator, typename Compare>
	void HeapSort(RAIterator first, RAIterator last, Compare comp)
	{
		using std::swap;
		static_assert(std::is_base_of<std::random_access_iterator_tag, typename std::iterator_traits<RAIterator>::iterator_category>::value, "HeapSort requires a random acces iterator!");
		HeapSortHelper::BuildHeap(first, last, comp);
		while(last > first)
		{
			swap(*first, *(last - 1));
			HeapSortHelper::Heapify(first, --last, first, comp);
		}
	}
}
#endif