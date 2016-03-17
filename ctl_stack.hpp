#ifndef CTL_STACK_HPP
#define CTL_STACK_HPP
#include <utility>

namespace CTL
{
	template<typename Container>
	using BackEnabledCheck = typename std::enable_if< std::is_same<void, decltype
		(
			std::declval<Container>().back(),
			std::declval<Container>().pop_back(),
			std::declval<Container>().push_back(std::declval<typename Container::value_type>())
			)>::value>::type;

	template<typename Container, typename = void>
	struct IsBackEnabled : std::false_type {};

	template<typename Container>
	struct IsBackEnabled<Container, BackEnabledCheck<Container>> : std::true_type {};

	template<typename Container>
	class FrontStack
	{
	public:
		using value_type = typename Container::value_type;

	private:
		Container container;

	public:


		void push(const value_type& val)
		{
			container.push_front(val);
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

	private:
		Container container;

	public:

		void push(const value_type& val)
		{
			container.push_back(val);
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

	};
}

#endif
