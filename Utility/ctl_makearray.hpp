#ifndef CTL_MAKEARRAY_HPP
#define CTL_MAKEARRAY_HPP
#include <memory>

namespace CTL
{
	template<typename Pointer, typename... Size>
	Pointer MakeArray(std::size_t size, Size... rest)
	{
		using Type = std::pointer_traits<Pointer>::element_type;
		Pointer tmp(new Type[size]);
		for (std::size_t i = 0; i < size; ++i)
		{
			tmp[i] = MakeArray<Type>(rest...);
		}
		return tmp;
	}

	template<typename Pointer>
	Pointer MakeArray(std::size_t size)
	{
		using Type = std::pointer_traits<Pointer>::element_type;
		return Pointer(new Type[size]);
	}
}

#endif // !CTL_MAKEARRAY_HPP
