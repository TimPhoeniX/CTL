#ifndef CTL_STACK_HPP
#define CTL_STACK_HPP
#include <utility>
#include <algorithm>
#include "../Utility/ctl_traits.hpp"
#include <algorithm>

namespace CTL
{
	template<typename Container>
	class FrontStack
	{
	public:
		using value_type = typename Container::value_type;

	protected:
		Container container;

	public:


		void push(const value_type& val)
		{
			container.push_front(val);
		}

		void push(value_type&& val)
		{
			container.push_front(std::move(val));
		}
		
		template<typename... Args>
		void emplace(Args&&... args)
		{
			container.emplace_front(std::forward<Args>(args)...);
		}

		void pop()
		{
			container.pop_front();
		}

		value_type& top()
		{
			return container.front();
		}
	};

	template<typename Container>
	class BackStack
	{
	public:
		using value_type = typename Container::value_type;

	protected:
		Container container;

	public:

		void push(const value_type& val)
		{
			container.push_back(val);
		}

		void push(value_type&& val)
		{
			container.push_back(std::move(val));
		}

		template<typename... Args>
		void emplace(Args&&... args)
		{
			container.emplace_back(std::forward<Args>(args)...);
		}

		void pop()
		{
			container.pop_back();
		}

		value_type& top()
		{
			return container.back();
		}
	};

	template<typename C>
	using StackType = typename std::conditional<IsBackEnabled<C>::value, BackStack<C>, FrontStack<C>>::type;

	template<typename T, typename Container>
	class Stack : public StackType<Container>
	{
	public:
		using value_type = typename StackType<Container>::value_type;
		using size_type = typename Container::size_type;

	private:

		template<typename TestSize>
		typename std::enable_if<IsSizeEnabled<TestSize>::value, size_type>::type
			getSize() const
		{
			return this->container.size();
		}

		template<typename TestSize>
		typename std::enable_if<!IsSizeEnabled<TestSize>::value, size_type>::type
			getSize() const
		{
			return std::distance(std::begin(this->container), std::end(this->container));
		}

//		static_assert(std::is_same<T, Container::value_type>::value, "Container does not store type T");
		

	public:
		size_type size() const
		{
			return this->template getSize<Container>();
		}

		bool empty() const
		{
			return this->container.empty();
		}

		void swap( Stack<T,Container>& other)
		{
			using std::swap;
			swap(this->container, other.container);
		}
	};
}

#endif
