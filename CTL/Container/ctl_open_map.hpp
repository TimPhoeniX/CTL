#ifndef _CTL_OPEN_MAP_HPP_
#define _CTL_OPEN_MAP_HPP_

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
		
		bool operator==(const Self& e)
		{
			return (this->Key==e.Key && this->Value==e.Value);
		}
	};
	
	template<typename K, typename V>
	class OpenEntry
	{
	public:
		using KeyType = K;
		using ValueType = V;
		
	private:
		using Self = OpenEntry<K,V>;
		using EntryType = Entry<K,V>;
		bool Used = false;
		EntryType* Storage = nullptr;
		
	public:
		OpenEntry()
		{
		}
		
		OpenEntry(const Self& e)
		: Used(e.Used), Storage(new EntryType(*e.Storage))
		{
		}
		
		OpenEntry(Self&& e)
		: Used(e.Used), Storage(e.Storage)
		{
			
			e.Storage=nullptr;
		}
		
		~OpenEntry()
		{
			delete Storage;
		}
		
		KeyType GetKey() const
		{
			return this->Storage->GetKey();
		}
		
		ValueType& GetValue()
		{
			return this->Storage->GetValue();
		}
		
		ValueType GetValue() const
		{
			return this->Storage->GetValue();
		}
		
		void Set(const KeyType& key, const ValueType& val)
		{
			delete this->Storage;
			this->Storage=new EntryType(key,val);
			this->Used=true;
		}
		
		bool Available()
		{
			return this->Storage==nullptr;
		}
		
		bool operator==(const Self& e)
		{
			return (this->Used==e.Used && *this->Storage==*e.Storage);
		}
	};
	
	template<typename Key, typename V, typename Hash, typename KeyEqual = std::equal_to<Key>>
	class OpenMap
	{
	public:
		using KeyType = Key;
		using ValueType = V;
		using EntryType = OpenEntry<Key,V>;
		using SizeType = std::size_t;
		using Reference = ValueType&;
		using ConstReference = const ValueType&;
		
	private:
		using Self = OpenMap<Key,V,KeyEqual,Hash>;
		
		EntryType* Entries = nullptr;
		SizeType Size = 0;
		KeyEqual Equal;
		Hash Hasher;
		
	public:
		OpenMap(const SizeType size, const Hash& h = Hash(), const KeyEqual& k = KeyEqual())
		: Entries(new EntryType[size]), Size(size),  Equal(k), Hasher(h)
		{
		}
		
		OpenMap(const SizeType size, Hash&& h = Hash(), const KeyEqual& k = KeyEqual())
		: Entries(new EntryType[size]), Size(size),  Equal(k), Hasher(std::move(h))
		{
		}
		
		OpenMap(Self&& m)
		: Entries(m.Entries), Size(m.size), Equal(std::move(m.Equal)), Hasher(std::move(m.Hasher))
		{
			m.Entries = nullptr;
		}
		
		~OpenMap()
		{
			delete[] Entries;
		}
		
		int Insert(const KeyType& key, const ValueType& val)
		{
			int count = 0;
			for(SizeType i = 0; i < this->Size; ++i)
			{
				auto hash = this->Hasher(key,i)%this->Size;
				if( this->Entries[hash].Available() )
				{
					this->Entries[hash].Set(key,val);
					return count;
				}
				++count;
			}
//			std::cerr << "Unable to find entry\n";
			return count;
		}
	};
}

#endif