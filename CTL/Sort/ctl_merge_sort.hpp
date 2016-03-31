/**
 * Conceptual Template Library by Piotr Grudzień
 * Stable MergeSort
 */

#ifndef CTL_MERGE_SORT_HPP
#define CTL_MERGE_SORT_HPP

#include <iterator>
#include <memory>
#include <algorithm>

namespace CTL
{
	template<typename RAIterator>
	void MergeSort(RAIterator first, RAIterator last)
	{
		static_assert(std::is_base_of<std::random_access_iterator_tag, typename std::iterator_traits<RAIterator>::iterator_category>::value, "MergeSort requires a random acces iterator!");
		using std::move;
		using Type = typename std::iterator_traits<RAIterator>::value_type;
		if((last - first) > 1)
		{
			RAIterator mid = first + (last - first) / 2;
			MergeSort(first, mid);
			MergeSort(mid, last);
			void* buffer = ::operator new[]((mid - first)*sizeof(Type));
			Type* bbegin = static_cast<Type*>(buffer);
			Type* bend = bbegin + (mid - first);
			std::uninitialized_copy(std::make_move_iterator(first), std::make_move_iterator(mid), bbegin);
			while(bbegin != bend && mid != last)
			{
				if(*mid < *bbegin)
				{
					(*(first++)) = move(*(mid++));
				}
				else
				{
					(*(first++)) = move(*bbegin);
					(bbegin++)->~Type();
				}
			}
			while(bbegin != bend)
			{
				(*(first++)) = move(*bbegin);
				(bbegin++)->~Type();
			}
			while(mid != last)
			{
				(*(first++)) = move(*(mid++));
			}
			::operator delete[](buffer);
		}
	}

	template<typename RAIterator, typename Compar>
	void MergeSort(RAIterator first, RAIterator last, Compar comp)
	{
		static_assert(std::is_base_of<std::random_access_iterator_tag, typename std::iterator_traits<RAIterator>::iterator_category>::value, "MergeSort requires a random acces iterator!");
		using std::move;
		using Type = typename std::iterator_traits<RAIterator>::value_type;
		if((last - first) > 1)
		{
			RAIterator mid = first + (last - first) / 2;
			MergeSort(first, mid, comp);
			MergeSort(mid, last, comp);
			void* buffer = ::operator new[]((mid - first)*sizeof(Type));
			Type* bbegin = static_cast<Type*>(buffer);
			Type* bend = bbegin + (mid - first);
			std::uninitialized_copy(std::make_move_iterator(first), std::make_move_iterator(mid), bbegin);
			while(bbegin != bend && mid != last)
			{
				if(comp(*(mid), *(bbegin)))
				{
					(*(first++)) = move(*(mid++));
				}
				else
				{
					(*(first++)) = move(*bbegin);
					(bbegin++)->~Type();
				}
			}
			while(bbegin != bend)
			{
				(*(first++)) = move(*bbegin);
				(bbegin++)->~Type();
			}
			while(mid != last)
			{
				(*(first++)) = move(*(mid++));
			}
			::operator delete[](buffer);
		}
	}
}
#endif
