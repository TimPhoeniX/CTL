/**
 * Conceptual Template Library by Piotr Grudzień
 * QuickSort
 */

#ifndef CTL_QUICK_SORT_HPP
#define CTL_QUICK_SORT_HPP

#include <thread>
#include <mutex>
#include <atomic>
#include <stack>
#include <condition_variable>
#include "../ctl_pair.hpp"
#include "../Container/ctl_stack.hpp"
#include "../Container/ctl_arraylist.hpp"

namespace CTL
{
	template<typename RAIterator>
	inline RAIterator LomutoPartition(RAIterator first, RAIterator last)
	{
		using Type = typename std::iterator_traits<RAIterator>::value_type;
		Type pivot = *(last - 1);
		--last;
		for(RAIterator It = first; first != last; ++It)
		{
			if(*It < pivot)
			{
				std::swap(*first, *It);
				++first;
			}
		}
		std::swap(*first, *(last));
		return first;
	}

	template<typename RAIterator, typename Compar>
	inline RAIterator LomutoPartition(RAIterator first, RAIterator last, Compar comp)
	{
		using Type = typename std::iterator_traits<RAIterator>::value_type;
		Type Pivot = *(last - 1);
		--last;
		for(RAIterator It = first; It != last; ++It)
		{
			if(comp(*It, Pivot))
			{
				std::swap(*first, *It);
				++first;
			}
		}
		std::swap(*first, *(last));
		return first;
	}

	template<typename RAIterator>
	inline RAIterator RightmostPivot(RAIterator first, RAIterator last)
	{
		return --last;
	}

	template<typename RAIterator, typename Compar>
	inline RAIterator RightmostPivot(RAIterator first, RAIterator last, Compar comp)
	{
		return --last;
	}

	template<typename RAIterator>
	inline RAIterator HoarePartition(RAIterator first, RAIterator last)
	{
		using std::swap;
		using Type = typename std::iterator_traits<RAIterator>::value_type;
		Type pivot = *(RightmostPivot<RAIterator>(first, last--));
		while(true)
		{
			while(*first < pivot) ++first;
			while(pivot < *last) --last;
			if(first < last) swap(*first, *last);
			else return last;
		}
	}

	template<typename RAIterator, RAIterator(*Pivot)(RAIterator, RAIterator)>
	inline RAIterator HoarePartition(RAIterator first, RAIterator last)
	{
		using std::swap;
		using Type = typename std::iterator_traits<RAIterator>::value_type;
		Type pivot = *(Pivot(first, last--));
		while(true)
		{
			while(*first < pivot) ++first;
			while(pivot < *last) --last;
			if(first < last) swap(*first, *last);
			else return last;
		}
	}

	template<typename RAIterator, typename Compar>
	inline RAIterator HoarePartition(RAIterator first, RAIterator last, Compar comp)
	{
		using std::swap;
		using Type = typename std::iterator_traits<RAIterator>::value_type;
		Type pivot = *(RightmostPivot<RAIterator,Compar>(first, last--, comp));
		while(true)
		{
			while(comp(*first, pivot)) ++first;
			while(comp(pivot, *last)) --last;
			if(first < last) swap(*first, *last);
			else return last;
		}
	}

	template<typename RAIterator, typename Compar, RAIterator(*Pivot)(RAIterator, RAIterator, Compar)>
	inline RAIterator HoarePartition(RAIterator first, RAIterator last, Compar comp)
	{
		using std::swap;
		using Type = typename std::iterator_traits<RAIterator>::value_type;
		Type pivot = *(Pivot(first, last--, comp));
		while(true)
		{
			while(comp(*first, pivot)) ++first;
			while(comp(pivot, *last)) --last;
			if(first < last) swap(*first, *last);
			else return last;
		}
	}

	template<typename RAIterator>
	void QuickSort(RAIterator first, RAIterator last)
	{
		static_assert(std::is_base_of<std::random_access_iterator_tag, typename std::iterator_traits<RAIterator>::iterator_category>::value, "QuickSort requires a random access iterator");
		if(last - first < 2) return;
		RAIterator p = HoarePartition<RAIterator>(first, last);
		QuickSort<RAIterator>(first, p);
		QuickSort<RAIterator>(p, last);
	}

	template<typename RAIterator, RAIterator(*Partition)(RAIterator, RAIterator)>
	void QuickSort(RAIterator first, RAIterator last)
	{
		static_assert(std::is_base_of<std::random_access_iterator_tag, typename std::iterator_traits<RAIterator>::iterator_category>::value, "QuickSort requires a random access iterator");
		if(last - first < 2) return;
		RAIterator p = Partition(first, last);
		QuickSort<RAIterator, Partition>(first, p);
		QuickSort<RAIterator, Partition>(p, last);
	}

	template<typename RAIterator, typename Compar>
	void QuickSort(RAIterator first, RAIterator last, Compar comp)
	{
		static_assert(std::is_base_of<std::random_access_iterator_tag, typename std::iterator_traits<RAIterator>::iterator_category>::value, "QuickSort requires a random access iterator");
		if(last - first < 2) return;
		RAIterator p = HoarePartition<RAIterator,Compar>(first, last, comp);
		QuickSort<RAIterator, Compar>(first, p, comp);
		QuickSort<RAIterator, Compar>(p, last, comp);
	}

	template<typename RAIterator, typename Compar, RAIterator(*Partition)(RAIterator, RAIterator, Compar)>
	void QuickSort(RAIterator first, RAIterator last, Compar comp)
	{
		static_assert(std::is_base_of<std::random_access_iterator_tag, typename std::iterator_traits<RAIterator>::iterator_category>::value, "QuickSort requires a random access iterator");
		if(last - first < 2) return;
		RAIterator p = Partition(first, last, comp);
		QuickSort<RAIterator, Compar, Partition>(first, p, comp);
		QuickSort<RAIterator, Compar, Partition>(p, last, comp);
	}

	template<typename RAIterator, RAIterator(*Partition)(RAIterator, RAIterator) = &HoarePartition<RAIterator>>
	class ParallelQuickSort
	{
	private:
		using Part = Pair<RAIterator, RAIterator>;
		ArrayList<std::thread> Threadpool;
		std::atomic_uint Working;
		std::atomic_bool Standby;
		std::stack<Part> Partitions;
		std::mutex Sync;
		std::mutex Queue;
		std::condition_variable Workers;
		std::condition_variable Controller;

		void Worker()
		{
			std::unique_lock<std::mutex> SortLock(this->Sync);
			while(this->Standby)
			{
				if(this->Partitions.empty())
				{
					if(this->Working)
					{
						this->Workers.wait(SortLock);
					}
					else
					{
						this->Controller.notify_one();
						this->Workers.wait(SortLock);
					}
				}
				else
				{
					auto partition = this->Partitions.top();
					this->Partitions.pop();
					++this->Working;
					SortLock.unlock();
					RAIterator left = partition.GetFirst(), right = partition.GetSecond();
					while(right - left > 1024)
					{
						auto p = Partition(left, right);
						if(right - p < 1025)
						{
							QuickSort<RAIterator, Partition>(p + 1, right);
						}
						else
						{
							SortLock.lock();
							this->Partitions.push(Part(p + 1, right));
							SortLock.unlock();
							this->Workers.notify_one();
						}
						right = p;
					}
					QuickSort<RAIterator, Partition>(left, right);
					SortLock.lock();
					--this->Working;
				}
			}
		}

	public:
		ParallelQuickSort(unsigned int pool = std::thread::hardware_concurrency()):
			Threadpool(pool),
			Working(0),
			Standby(true)
		{
			while(pool--)
			{
				this->Threadpool.push_back(std::thread(&ParallelQuickSort::Worker, this));
			}
		}

		//		ParallelQuickSort(ParallelQuickSort<RAIterator,Compar,Partition>&& sort)

		~ParallelQuickSort()
		{
			this->Standby = false;
			this->Workers.notify_all();
			for(auto& th : this->Threadpool)
			{
				th.join();
			}
		}

		void operator()(RAIterator first, RAIterator last)
		{
			std::lock_guard<std::mutex>(this->Queue); //Prevents acces from other threads
			std::unique_lock<std::mutex> Lock(this->Sync);
			this->Partitions.push(Part(first, last));
			this->Workers.notify_one();
			this->Controller.wait(Lock);
		}

	};

	template<typename RAIterator, typename Compar, RAIterator(*Partition)(RAIterator, RAIterator, Compar) = &HoarePartition<RAIterator, Compar>>
	class ParallelQuickSortC
	{
	private:
		using Part = Pair<RAIterator, RAIterator>;
		ArrayList<std::thread> Threadpool;
		std::atomic_uint Working;
		std::atomic_bool Standby;
		std::stack<Part> Partitions;
		std::mutex Sync;
		std::mutex Queue;
		std::condition_variable Workers;
		std::condition_variable Controller;
		Compar Comp = Compar();

		void Worker()
		{
			std::unique_lock<std::mutex> SortLock(this->Sync);
			while(this->Standby)
			{
				if(this->Partitions.empty())
				{
					if(this->Working)
					{
						this->Workers.wait(SortLock);
					}
					else
					{
						this->Controller.notify_one();
						this->Workers.wait(SortLock);
					}
				}
				else
				{
					auto partition = this->Partitions.top();
					this->Partitions.pop();
					++this->Working;
					SortLock.unlock();
					RAIterator left = partition.GetFirst(), right = partition.GetSecond();
					while(right - left > 1024)
					{
						auto p = Partition(left, right, this->Comp);
						if(right - p < 1025)
						{
							QuickSort<RAIterator, Compar, Partition>(p, right, this->Comp);
						}
						else
						{
							SortLock.lock();
							this->Partitions.push(Part(p, right));
							SortLock.unlock();
							this->Workers.notify_one();
						}
						right = p;
					}
					QuickSort<RAIterator, Compar, Partition>(left, right, this->Comp);
					SortLock.lock();
					--this->Working;
				}
			}
		}

	public:
		ParallelQuickSortC(unsigned int pool = std::thread::hardware_concurrency())
			: Threadpool(pool), Working(0), Standby(true)
		{
			while(pool--)
			{
				this->Threadpool.push_back(std::thread(&ParallelQuickSortC::Worker, this));
			}
		}

		//		ParallelQuickSort(ParallelQuickSort<RAIterator,Compar,Partition>&& sort)

		~ParallelQuickSortC()
		{
			this->Standby = false;
			this->Workers.notify_all();
			for(auto& th : this->Threadpool)
			{
				th.join();
			}
		}

		void operator()(RAIterator first, RAIterator last)
		{
			std::lock_guard<std::mutex>(this->Queue); //Prevents acces from other threads
			std::unique_lock<std::mutex> Lock(this->Sync);
			this->Partitions.Push(Part(first, last));
			this->Workers.notify_one();
			this->Controller.wait(Lock);
		}

		void operator()(RAIterator first, RAIterator last, Compar comp)
		{
			std::lock_guard<std::mutex>(this->Queue); //Prevents acces from other threads
			std::unique_lock<std::mutex> Lock(this->Sync);
			this->Comp = comp;
			this->Partitions.Push(Part(first, last));
			this->Workers.notify_one();
			this->Controller.wait(Lock);
		}

		void SetCompare(Compar c)
		{
			std::lock_guard<std::mutex>(this->Queue); //Prevents mid-sorting comparator change.
			this->Comp=c;
		}
	};

} // !CTL
#endif
