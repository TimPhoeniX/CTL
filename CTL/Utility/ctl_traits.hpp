/**
 * Conceptual Template Library
 * SFINAE templates
 */

#ifndef CTL_TRAITS_HPP
#define CTL_TRAITS_HPP
#include <type_traits>

namespace CTL
{
	template<typename Container>
	using HasSizeCheck = typename std::enable_if< std::is_same<std::true_type, decltype(
		std::declval<Container>().size(),
		std::true_type())>::value>::type;

	template<typename Container, typename = void>
	struct IsSizeEnabled : std::false_type
	{};

	template<typename Container>
	struct IsSizeEnabled<Container, HasSizeCheck<Container>> : std::true_type
	{};

	template<typename Container>
	using BackEnabledCheck = typename std::enable_if< std::is_same<std::true_type, decltype(
		std::declval<Container>().back(),
		std::declval<Container>().pop_back(),
		std::declval<Container>().push_back(std::declval<typename Container::const_reference>()),
		std::true_type())>::value>::type;

	template<typename Container, typename = void>
	struct IsBackEnabled : std::false_type
	{};

	template<typename Container>
	struct IsBackEnabled<Container, BackEnabledCheck<Container>> : std::true_type
	{};

	//	Apparently, this does not work on MSVC.
	template<typename Container>
	using FrontEnabledCheck = typename std::enable_if< std::is_same<std::true_type, decltype(
		std::declval<Container>().front(),
		std::declval<Container>().pop_front(),
		std::declval<Container>().push_front(std::declval<typename Container::const_reference>()),
		std::true_type())>::value>::type;

	//	This works in C++14 by Intel
	//template<typename Container>
	//using FrontEnabledCheck = typename std::enable_if< decltype(
	//	std::declval<Container>().front(),
	//	std::declval<Container>().pop_front(),
	//	std::declval<Container>().push_front(std::declval<typename Container::const_reference>()),
	//	std::true_type())::value>::type;

	template<typename Container, typename = void>
	struct IsFrontEnabled : std::false_type
	{};

	template<typename Container>
	struct IsFrontEnabled<Container, FrontEnabledCheck<Container>> : std::true_type
	{};
}

#endif // !CTL_TRAITS_HPP
