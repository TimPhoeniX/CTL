#ifndef CTL_PRIORITYQUEUE
#define CTL_PRIORITYQUEUE

#include <iterator>
#include <algorithm>

namespace CTL
{
	template<typename T, typename RAContainer, typename Compar>
	class HeapQueue
	{
	public:
		using iterator = typename RAContainer::iterator;
	private:
		RAContainer container;
		Compar comp;
		
		void SiftDown(iterator first, iterator last, iterator i)
		{
			using std::swap;
			typename std::iterator_traits<iterator>::difference_type Node = i - first;
			iterator Top = i, left = (first + (2 * Node + 1)), right = (first + (2 * Node + 2));
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
				SiftDown(first, last, Top);
			}
		}

		void BuildHeap(iterator first, iterator last)
		{
			iterator i = first + (((last - first) / 2) - 1);
			while(i >= first)
			{
				SiftDown(first, last, i--);
			}
		}

		void SiftUp(iterator first, iterator elem)
		{
			using std::swap;
			iterator cpy = elem + 1;
			iterator parent = first + ((elem - first - 1) / 2);
			while(elem > first && comp(*parent, *elem))
			{
				swap(*elem, *parent);
				elem = parent;
				parent = first + ((elem - first - 1) / 2);
			}
		}

		void PopHeap(iterator first, iterator last)
		{
			using std::swap;
			swap(*first, *--last);
			this->container.pop_back();
			SiftDown(first, last, first);
		}

	public:
		HeapQueue() : HeapQueue(Compar()) {}
		explicit HeapQueue(Compar c): comp(c){}

		explicit HeapQueue(const RAContainer& c, Compar comp = Compar()):
			container(c), comp(comp)
		{
			this->BuildHeap(this->container.begin(), this->container.end());
		}

		bool empty() const
		{
			return this->container.empty();
		}

		void push(const T& e)
		{
			this->container.push_back(e);
			this->SiftUp(this->container.begin(), this->container.end() - 1);
		}

		T top()
		{
			return this->container.front();
		}

		void pop()
		{
			this->PopHeap(this->container.begin(), this->container.end());
		}
	};

	template<typename T,typename K>
	struct HeapEntry
	{
		T value;
		K key;
		HeapEntry(const T& t, const K& k) : value(t), key(k) {}
	};
}

#endif
