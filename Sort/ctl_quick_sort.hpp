/**
	Conceptual Template Library by Piotr Grudzień
	QuickSort
*/

#ifndef _CTL_QUICK_SORT_HPP_
#define _CTL_QUICK_SORT_HPP_

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "ctl_pair.hpp"
#include "ctl_stack.hpp"
#include "ctl_arraylist.hpp"

namespace CTL
{
	template<typename RAIterator, typename Compar>
	inline RAIterator LomutoPartition(RAIterator first, RAIterator last, Compar& comp)
	{
		using Type = typename std::iterator_traits<RAIterator>::value_type;
//		std::cerr << last-first << std::endl;
		Type Pivot = *(last - 1);
		RAIterator iIt = first;
		--last;
//		while (jIt != (last - 1))
		for(RAIterator jIt = first; jIt != last; ++jIt)
		{
			if(comp(*jIt, Pivot))
			{
				std::swap(*iIt, *jIt);
				++iIt;
			}
//			++jIt;
		}
		std::swap(*iIt, *(last));
		return iIt;
	}

	template<typename RAIterator, typename Compar, RAIterator(*Partition)(RAIterator, RAIterator, Compar&) = LomutoPartition>
	void QuickSort(RAIterator first, RAIterator last, Compar comp)
	{
		if(last - first < 2) return;
//		std::cerr << "Partitioning " << last-first << '\n';
		RAIterator p = Partition(first, last, comp);
		QuickSort<RAIterator, Compar, Partition>(first, p, comp);
		QuickSort<RAIterator, Compar, Partition>(p + 1, last, comp);
	}

	template<typename RAIterator, typename Compar, RAIterator(*Partition)(RAIterator, RAIterator, Compar&) = LomutoPartition>
	void NaiveQuickSort(RAIterator first, RAIterator last, Compar comp)
	{
		if(last - first < 2) return;
		if(last - first < 1024) return QuickSort<RAIterator, Compar, Partition>(first, last, comp);
		RAIterator p = Partition(first, last, comp);
		std::thread left(NaiveQuickSort<RAIterator, Compar, Partition>, first, p, comp);
		std::thread right(NaiveQuickSort<RAIterator, Compar, Partition>, p+1, last, comp);
		left.join();
		right.join();
	}

	template<typename RAIterator, typename Compar, RAIterator(*Partition)(RAIterator, RAIterator, Compar&) = LomutoPartition>
	void LimitedQuickSort(RAIterator first, RAIterator last, Compar comp, unsigned int limit)
	{
		if(last - first < 2) return;
		if(limit < 2) return QuickSort<RAIterator, Compar, Partition>(first, last, comp);
		RAIterator p = Partition(first, last, comp);
		std::thread right(LimitedQuickSort<RAIterator, Compar, Partition>, p + 1, last, comp, limit / 2 + (p - first < last - p ? limit % 2 : 0));
		LimitedQuickSort<RAIterator, Compar, Partition>(first, p, comp, limit / 2 + (p - first < last - p ? 0 : limit % 2));
		right.join();
	}

//	template<typename RAIterator, typename Compar, RAIterator(*Partition)(RAIterator, RAIterator, Compar&) = LomutoPartition>

	template<typename RAIterator, typename Compar, RAIterator(*Partition)(RAIterator, RAIterator, Compar&) = LomutoPartition>
	class ParallelQuickSort
	{
	private:
		using Part = Pair<RAIterator, RAIterator>;
		ArrayList<std::thread> Threadpool;
		std::atomic_uint Working;
		std::atomic_bool Standby;
		Stack<Part> Partitions;
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
				if(this->Partitions.Empty())
				{
					//std::cerr << this->Working << '\n';
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
//					std::cerr << "Popping\n";
					auto partition = this->Partitions.Pop();
//					std::cerr << "Popped\n";
					++this->Working;
//					std::cerr << this->Working << '\n';
					SortLock.unlock();
					RAIterator left = partition.GetFirst(), right = partition.GetSecond();
					while(right - left > 1024)
					{
//						std::cerr << "Partitioning\n";
						auto p = Partition(left, right, this->Comp);
//						std::cerr << "Partitioned\n";
						if(right - p < 1025)
						{
//							std::cerr << "RQSing\n";
							QuickSort<RAIterator, Compar, Partition>(p + 1, right, this->Comp);
//							std::cerr << "RQSed\n";
						}
						else
						{
							SortLock.lock();
//							std::cerr << "Pushing\n";
							this->Partitions.Push(Part(p + 1, right));
//							std::cerr << "Pushed\n";
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
		ParallelQuickSort(unsigned int pool = std::thread::hardware_concurrency())
		: Threadpool(pool), Working(0), Standby(true)
		{
			while(pool--)
			{
				this->Threadpool.PushBack(std::thread(&ParallelQuickSort::Worker, this));
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
//			std::cerr << "Controller Wait\n";
			this->Controller.wait(Lock);
		}
	};

}
#endif
