#pragma once

#include <lmi/lmi.h>
#include <type_traits>
#include <utility>

// Functions taken from http://iquilezles.org/www/articles/distfunctions/distfunctions.htm

namespace Primitive
{
	namespace detail
	{
		template <typename BinaryOperator, typename T>
		auto foldl(BinaryOperator &&op, T left, T right)
		{
			return op(left, right);
		}

		template <typename BinaryOperator, typename T, typename... Rest>
		auto foldl(BinaryOperator &&op, T left, Rest... right)
		{
			return op(left, foldl(op, right...));
		}
	}

	template <typename... T>
	auto pUnion(T... distances)
	{
		return detail::foldl([](auto a, auto b) { return a < b ? a : b; }, std::forward<T>(distances)...);
	}

	template <typename... T>
	auto pSubtraction(T... distances)
	{
		return detail::foldl([](auto a, auto b) { return -a > b ? a : b; }, std::forward<T>(distances)...);
	}

	template <typename... T>
	auto pIntersection(T... distances)
	{
		return detail::foldl([](auto a, auto b) { return a > b ? a : b; }, std::forward<T>(distances)...);
	}

	float sphere(const lmi::vec3 &center, float radius);
}
