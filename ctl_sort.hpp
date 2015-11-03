#ifndef _CTL_SORT_HPP_
#define _CTL_SORT_HPP_

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
				std::cerr << *resultIt<< '\n';
				(*(left++))=std::move(*(resultIt++));
			}
			delete[] result;
		}
	}
}

#endif