#ifndef _CTL_HASH_MAP_HPP_
#define _CTL_HASH_MAP_HPP_

#include "ctl_forwardlist.hpp"
#include "ctl_sort.hpp"
#include <utility>
#include <stdexcept>

namespace CTL
{
	template<typename K, typename V>
	class Entry
	{
	public:
		using KeyType = K;
		using ValueType = V;
		
	private:
		using Self = Entry<K,V>;
		KeyType Key;
		ValueType Value;
		
	public:
		Entry(KeyType k, ValueType v)
		: Key(k), Value(v)
		{
		}
		
		Entry(const Self& e)
		: Key(e.Key), Value(e.Value)
		{
		}
		
		Entry(Self&& e)
		: Key(std::move(e.Key)), Value(std::move(e.Value))
		{
		}
		
		~Entry()
		{
		}
		
		KeyType GetKey() const
		{
			return this->Key;
		}
		
		ValueType& GetValue()
		{
			return this->Value;
		}
		
		ValueType GetValue() const
		{
			return this->Value;
		}
		
		bool operator=(const Self& e)
		{
			return (this->Key==e.Key && this->Value==e.Value);
		}
	};
	
	template<typename Key, typename V, typename KeyEqual = std::equal_to<Key>, typename Hash = std::hash<Key> >
	class UnorderedMapPrototype
	{
	public:
		using KeyType = Key;
		using ValueType = V;
		using EntryType = Entry<Key,V>;
		using BucketType = CTL::ForwardList<EntryType>;
		using BucketSize = typename BucketType::SizeType;
		using SizeType = std::size_t;
		using Reference = ValueType&;
		using ConstReference = const ValueType&;
		
	private:
		using Self = UnorderedMapPrototype<Key,V,KeyEqual,Hash>;
		
		BucketType* Buckets = new BucketType[100];
		KeyEqual Equal;
		Hash Hasher;
		
	public:
		UnorderedMapPrototype(const KeyEqual& k = KeyEqual(), const Hash& h = Hash())
		: Equal(k), Hasher(h)
		{
		}
		
		UnorderedMapPrototype(Self&& u)
		: Buckets(u.Buckets), Equal(u.Equal), Hasher(u.Hasher)
		{
			u.Buckets = nullptr;
		}
		
		~UnorderedMapPrototype()
		{
			delete[] Buckets;
		}
		
		bool Insert(const KeyType& key, const ValueType& val)
		{
			auto hash = Hasher(key)%100;
			auto it = Buckets[hash].Begin();
			auto end = Buckets[hash].End();
			while(it!=end)
			{
				if(it->GetKey() == key)
				{
					return false;
				}
				++it;
			}
			Buckets[hash].PushBack(EntryType(key,val));
			return true;
		}
		
		Reference Find(const KeyType& key)
		{
			auto hash = Hasher(key)%100;
			auto it = Buckets[hash].Begin();
			auto end = Buckets[hash].End();
			while(it!=end)
			{
				if(it->GetKey() == key)
				{
					return it->GetValue();
				}
				++it;
			}
			throw std::out_of_range("Key not found");
		}
		
		ConstReference Find(const KeyType& key) const
		{
			auto hash = Hasher(key)%100;
			auto it = Buckets[hash].Begin();
			auto end = Buckets[hash].End();
			while(it!=end)
			{
				if(it->GetKey() == key)
				{
					return it->GetValue();
				}
				++it;
			}
			throw std::out_of_range("Key not found");
		}
		
		void Delete(const KeyType& key)
		{
			auto hash = Hasher(key)%100;
			auto it = Buckets[hash].Begin();
			auto end = Buckets[hash].End();
			BucketSize i = 0;
			while(it!=end)
			{
				if(it->GetKey() == key)
				{
					return Buckets[hash].Erase(i);
				}
				++i;
				++it;
			}
		}
		
		int getMaxListLen() const
		{
			SizeType max = 0;
			auto end = this->Buckets+100;
			for(auto it = this->Buckets; it != end ; ++it)
			{
				if( it->GetSize() > max)
				{
					max = it->GetSize();
				}
			}
			return int(max);
		}
		
		int getMedianListLen() const
		{
			SizeType* sizes = new SizeType[100];
			for(SizeType i = 0; i < 100; ++i)
			{
				sizes[i]=Buckets[i].GetSize();
			}
			CTL::HeapSort(sizes, sizes+100, std::less<SizeType>() );
			return int( (sizes[49]+sizes[50])/2 );
		}
		
	};
}

#endif