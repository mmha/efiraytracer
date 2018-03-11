#pragma once

#include <lmi/lmi.h>
#include <type_traits>
#include <utility>

// Functions taken from http://iquilezles.org/www/articles/distfunctions/distfunctions.htm

namespace Primitive {
	namespace detail {
		template <typename BinaryOperator, typename T, typename U>
		constexpr decltype(auto) foldl(BinaryOperator &&op, T &&left, U &&right) {
			return std::forward<BinaryOperator>(op)(std::forward<T>(left), std::forward<U>(right));
		}

		template <typename BinaryOperator, typename T, typename... Rest>
		constexpr decltype(auto) foldl(BinaryOperator &&op, T left, Rest &&... right) {
			return std::forward<BinaryOperator>(op)(
				std::forward<T>(left), foldl(std::forward<BinaryOperator>(op), std::forward<Rest>(right)...));
		}
	}	// namespace detail

	template <typename... T>
	constexpr auto pUnion(T &&... distances) {
		return detail::foldl([](auto const &a, auto const &b) { return a < b ? a : b; }, std::forward<T>(distances)...);
	}

	template <typename... T>
	constexpr auto pSubtraction(T &&... distances) {
		return detail::foldl([](auto const &a, auto const &b) { return -a > b ? a : b; },
							 std::forward<T>(distances)...);
	}

	template <typename... T>
	constexpr auto pIntersection(T &&... distances) {
		return detail::foldl([](auto const &a, auto const &b) { return a > b ? a : b; }, std::forward<T>(distances)...);
	}

	float sphere(const lmi::vec3 &center, float radius);
}	// namespace Primitive
