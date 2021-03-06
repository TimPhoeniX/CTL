
/**
 * Conceptual Template Library by Piotr Grudzień
 * Simple ArrayList
 */
#ifndef CTL_ARRAYLIST_HPP
#define CTL_ARRAYLIST_HPP

#include <memory>
#include <iostream>
#include <stdexcept>

namespace CTL
{
	template<typename T, typename Allocator = std::allocator<T> >
	class ArrayList: private Allocator
	{
	private:
		using Alloc = std::allocator_traits<Allocator>;

	public:
		using value_type = T;
		using allocator_type = Allocator;
		using size_type = typename Alloc::size_type;
		using difference_type = typename Alloc::difference_type;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = typename Alloc::pointer;
		using const_pointer = typename Alloc::const_pointer;
		using iterator = value_type*;
		using const_iterator = const value_type*;

	private:
		size_type cSize = 0;
		size_type cMaxSize = 4;
		pointer storage = nullptr;

		void MoveBack(size_type begin)
		{
			if(this->cSize > 0)
			{
				Alloc::construct(*this, this->storage + this->cSize, std::move(this->storage[cSize - 1]));
				for(size_type i = this->cSize - 1; i > begin; --i)
				{
					this->storage[i] = std::move(this->storage[i - 1]);
				}
			}
			++(this->cSize);
		}

		void MoveFront(size_type begin)
		{
			for(size_type i = begin; i < cSize - 1; ++i)
			{
				this->storage[i] = std::move(this->storage[i + 1]);
			}
			Alloc::destroy(*this, this->storage + (--cSize));
		}

	public:
		ArrayList():
			ArrayList(Allocator())
		{}

		explicit ArrayList(const Allocator& alloc):
			Allocator(alloc),
			storage(Alloc::allocate(*this,this->cMaxSize))
		{}

		ArrayList(size_type count, const T& value, const Allocator& alloc = Allocator()):
			Allocator(alloc),
			cSize(count),
			cMaxSize(count),
			storage(Alloc::allocate(*this, count))
		{
			for(auto begin = this->storage, end = this->storage + count; begin != end; ++begin)
			{
				Alloc::construct(*this, begin, value);
			}
		}

		ArrayList(size_type count, const Allocator& alloc = Allocator()):
			Allocator(alloc),
			cMaxSize(count),
			storage(Alloc::allocate(*this, count))
		{}

		ArrayList(const ArrayList& other):
			Allocator(Alloc::select_on_container_copy_construction(other.get_allocator())),
			storage(Alloc::allocate(*this, other.cMaxSize)),
			cSize(other.cSize),
			cMaxSize(other.cMaxSize)
		{
			for(auto a = this->storage, b = other.storage, end = other.storage + other.cSize; b != end; ++a, ++b)
			{
				Alloc::construct(*this, a, *b);
			}
		}

		ArrayList(ArrayList<value_type>&& l):
			Allocator(std::move(l)),
			cSize(l.cSize),
			cMaxSize(l.cMaxSize),
			storage(l.storage)
		{
			l.storage = nullptr;
			l.cSize = 0;
			l.cMaxSize = 0;
		}

		~ArrayList()
		{
			this->clear();
			Alloc::deallocate(*this, this->storage, this->cMaxSize);
		}

		allocator_type get_allocator() const
		{
			return *this;
		}

		iterator begin()
		{
			return this->storage;
		}

		iterator end()
		{
			return this->storage + this->cSize;
		}

		const_iterator begin() const
		{
			return this->storage;
		}

		const_iterator end() const
		{
			return this->storage + this->cSize;
		}

		const_iterator cbegin()
		{
			return this->storage;
		}

		const_iterator cend()
		{
			return this->storage + this->cSize;
		}

		bool empty() const
		{
			return this->cSize == 0;
		}

		void reserve(const size_type newSize)
		{
			if(newSize <= this->cMaxSize) return;
			pointer to = Alloc::allocate(*this, newSize);
			pointer newStorage = to;
			pointer oldStorage = this->storage;
			for(size_type i = 0; i < this->cSize; ++i)
			{
				Alloc::construct(*this, newStorage++, std::move(*oldStorage));
				Alloc::destroy(*this, oldStorage++);
			}
			Alloc::deallocate(*this, this->storage, this->cMaxSize);
			this->storage = to;
			this->cMaxSize = newSize;
		}

		void shrink_to_fit()
		{
			if(this-> cSize == this->cMaxSize) return;
			pointer to = Alloc::allocate(*this, this->cSize);
			pointer newStorage = to;
			pointer oldStorage = this->storage;
			for(size_type i = 0; i < this->cSize; ++i)
			{
				Alloc::construct(*this, newStorage++, std::move(*oldStorage));
				Alloc::destroy(*this, oldStorage++);
			}
			Alloc::deallocate(*this, this->storage, this->cMaxSize);
			this->storage = to;
			this->cMaxSize = this->cSize;
		}

		size_type size() const
		{
			return this->cSize;
		}

		size_type max_size() const
		{
			return this->cMaxSize;
		}

		value_type& back()
		{
			return this->storage[this->cSize - 1];
		}

		value_type& front()
		{
			return *this->storage;
		}

		void push_front(const value_type& e)
		{
			if(this->cSize == this->cMaxSize)
			{
				this->reserve(2 * this->cMaxSize);
			}
			this->MoveBack(0);
			*(this->storage) = e;
		}

		void push_front(value_type&& e)
		{
			if(this->cSize == this->cMaxSize)
			{
				this->reserve(2 * this->cMaxSize);
			}
			this->MoveBack(0);
			*(this->storage) = std::move(e);
		}

		void push_back(const value_type& e)
		{
			if(this->cSize == this->cMaxSize)
			{
				this->reserve(2 * this->cMaxSize);
			}
			Alloc::construct(*this, this->storage + (cSize++), e);
		}

		void push_back(value_type&& e)
		{
			if(this->cSize == this->cMaxSize)
			{
				this->reserve(2 * this->cMaxSize);
			}
			Alloc::construct(*this, this->storage + (cSize++), std::forward<T>(e));
		}

		//Standarize later;
		value_type pop_back()
		{
			if(this->cSize == 0) throw std::out_of_range("PopBack called on empty list");
			value_type tmp = std::move(this->storage[--this->cSize]);
			std::allocator_traits<allocator_type>::destroy(*this, this->storage + this->cSize);
			return tmp;
		}

		value_type pop_front()
		{
			if(this->cSize == 0) throw std::out_of_range("PopFront called on empty list");
			value_type tmp = std::move(*(this->storage));
			this->MoveFront(0);
			return tmp;
		}

		reference get(const size_type i)
		{
			if(this->cSize <= i) throw std::out_of_range("Get called with i >= cSize");
			return this->storage[i];
		}

		const_reference get(const size_type i) const
		{
			if(this->cSize <= i) throw std::out_of_range("Get called with i >= cSize");
			return this->storage[i];
		}

		void insert(const size_type i, const value_type& e)
		{
			if(this->cSize < this->cMaxSize)
			{
				this->MoveBack(i);
				this->storage[i] = e;
			}
		}

		void insert(const size_type i, value_type&& e)
		{
			if(this->cSize < this->cMaxSize)
			{
				this->MoveBack(i);
				this->storage[i] = std::move(e);
			}
		}

		void insert(const_iterator i, const value_type& e)
		{
			this->insert(i - this->storage, e);
		}

		void insert(const_iterator i, value_type&& e)
		{
			this->insert(i - this->storage, std::move(e));
		}

		iterator find(const value_type& e)
		{
			for(size_type i = 0; i < this->cSize; ++i)
			{
				if(this->storage[i] == e) return this->storage + i;
			}
			return this->end();
		}

		void Erase(const size_type i)
		{
			if(i < this->cSize) this->MoveFront(i);
		}

		void clear()
		{
			for(auto ptr = this->begin(), end = this->end(); ptr != end; ++ptr)
			{
				Alloc::destroy(*this, ptr);
			}
			this->cSize = 0;
		}

		void Print(std::ostream& out = std::cout) const
		{
			for(size_type i = 0; i < this->cSize; ++i)
			{
				out << this->storage[i] << ' ';
			}
		}

		reference operator[](const size_type i)
		{
			return this->storage[i];
		}

		const_reference operator[](const size_type i) const
		{
			return this->storage[i];
		}

		friend std::ostream& operator<<(std::ostream& out, const CTL::ArrayList<value_type>& l)
		{
			l.Print(out);
			return out;
		}
	};
}

#endif