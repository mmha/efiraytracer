#pragma once
#include <boost/callable_traits/args.hpp>
#include <efi.h>
#include <functional>
#include <tl/expected.hpp>
#include <tuple>
#include <type_traits>

template <size_t offset, typename T, size_t... I>
auto constexpr split_tuple_impl(T &&tuple, std::index_sequence<I...>) noexcept(noexcept(std::tuple{
	std::get<offset + I>(std::forward<T>(tuple))...})) {
	return std::tuple{std::get<offset + I>(std::forward<T>(tuple))...};
}

template <size_t N, typename T>
auto constexpr split_tuple(T &&tuple) noexcept(noexcept(std::tuple{
	split_tuple_impl<0>(std::forward<T>(tuple), std::make_index_sequence<N>{}),
	split_tuple_impl<N>(std::forward<T>(tuple), std::make_index_sequence<std::tuple_size_v<std::decay_t<T>> - N>{}),
})) {
	auto constexpr sz = std::tuple_size_v<std::decay_t<T>>;
	return std::tuple{
		split_tuple_impl<0>(std::forward<T>(tuple), std::make_index_sequence<N>{}),
		split_tuple_impl<N>(std::forward<T>(tuple), std::make_index_sequence<sz - N>{}),
	};
}

template <typename Tuple, typename Pred, std::size_t... I>
constexpr auto transform_tuple_impl(Tuple &&t, Pred &&p, std::index_sequence<I...>) noexcept(noexcept(std::tuple{
	std::forward<Pred>(p)(std::get<I>(std::forward<Tuple>(t)))...})) {
	return std::tuple{std::forward<Pred>(p)(std::get<I>(std::forward<Tuple>(t)))...};
}

template <typename Tuple, typename Pred>
constexpr auto transform_tuple(Tuple &&t, Pred &&p) noexcept(
	noexcept(transform_tuple_impl(std::forward<Tuple>(t),
								  std::forward<Pred>(p),
								  std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{}))) {
	return transform_tuple_impl(std::forward<Tuple>(t), std::forward<Pred>(p),
								std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
}

template <typename In, typename InOut, typename Out, typename Callable>
struct out_param_adapter;

template <typename T>
struct storage_for {
	using type = T;
	constexpr storage_for() {
	}
	constexpr auto get() noexcept {
		return reinterpret_cast<T *>(&val);
	}
	constexpr auto get() const noexcept {
		return reinterpret_cast<T const *>(&val);
	}

	private:
	std::aligned_storage_t<sizeof(T), alignof(T)> val;
};


template <typename... Is, typename... IOs, typename... Os, typename Callable>
struct out_param_adapter<std::tuple<Is...>, std::tuple<IOs...>, std::tuple<Os...>, Callable> {
	constexpr out_param_adapter(Callable callable) noexcept
		: c(std::move(callable)) {
	}

	constexpr std::tuple<std::remove_pointer_t<Os>...> operator()(Is &&... is, IOs &&... ios) const noexcept {
		std::tuple<storage_for<std::remove_pointer_t<Os>>...> res;
		auto const ptr = transform_tuple(res, [](auto &val) {
			return reinterpret_cast<typename std::decay_t<decltype(val)>::type *>(std::addressof(val));
		});
		std::apply(c, tuple_cat(std::tuple{std::forward<Is>(is)...}, std::tuple{std::forward<IOs>(ios)...}, ptr));
		return transform_tuple(ptr, [](auto &val) { return *val; });
	}

	constexpr std::tuple<std::remove_pointer_t<Os>...> operator()(Is &&... is, IOs &&... ios) /* noexcept */
	{
		std::tuple<storage_for<std::remove_pointer_t<Os>>...> res;
		auto const ptr = transform_tuple(res, [](auto &val) {
			return reinterpret_cast<typename std::decay_t<decltype(val)>::type *>(std::addressof(val));
		});
		std::apply(c, tuple_cat(std::tuple{std::forward<Is>(is)...}, std::tuple{std::forward<IOs>(ios)...}, ptr));
		return transform_tuple(ptr, [](auto &val) { return *val; });
	}

	private:
	Callable c;
};


template <template <typename> typename T, typename U>
constexpr bool is_instance = false;

template <template <typename> typename T, typename U>
constexpr bool is_instance<T, T<U>> = true;

template <typename T>
struct ptrref {
	constexpr ptrref()
		: p_(nullptr) {
	}

	constexpr ptrref(std::nullptr_t)
		: p_(nullptr) {
	}

	template <typename U>
	constexpr ptrref(U &p) noexcept
		: p_(&p) {
	}

	template <typename U>
	constexpr ptrref(U *p) noexcept
		: p_(p) {
	}

	constexpr operator T *() const noexcept {
		return p_;
	}

	constexpr T *get() const noexcept {
		return p_;
	}

	private:
	T *p_;
};

template <typename T>
constexpr auto get_ptr(T &&t) noexcept {
	if constexpr(is_instance<ptrref, std::decay_t<T>>) {
		return std::forward<T>(t).get();
	} else {
		return t;
	}
}

template <typename T>
using replace_ptr = std::conditional_t<std::is_pointer_v<T>, ptrref<std::remove_pointer_t<T>>, T>;

template <typename Callable, typename... Is, typename... IOs, typename... Os>
auto constexpr make_out_param_adapter(Callable &&callable,
									  std::tuple<Is...> const &,
									  std::tuple<IOs...> const &,
									  std::tuple<Os...> const &) {
	return [c = std::forward<Callable>(callable)](replace_ptr<Is const> const... is, replace_ptr<IOs>... ios) {
		std::tuple<storage_for<std::remove_pointer_t<Os>>...> res;
		auto const ptr = transform_tuple(res, [](auto &val) {
			return reinterpret_cast<typename std::decay_t<decltype(val)>::type *>(std::addressof(val));
		});
		if constexpr(std::is_same_v<std::invoke_result_t<Callable, Is..., IOs..., Os...>, EFI_STATUS>) {
			using result_t = tl::expected<std::tuple<std::remove_pointer_t<Os>...>, EFI_STATUS>;
			auto const result = std::apply(c, tuple_cat(std::tuple{get_ptr(is)...}, std::tuple{get_ptr(ios)...}, ptr));
			if(false) {
				return result_t(tl::unexpect, result);
			}
			return result_t{transform_tuple(ptr, [](auto &val) { return *val; })};
		} else {
			std::apply(c, tuple_cat(std::tuple{get_ptr(is)...}, std::tuple{get_ptr(ios)...}, ptr));
			return res;
		}
	};
}

template <size_t in_count, size_t inout_count, size_t out_count = SIZE_MAX, typename Callable>
auto constexpr wrap(Callable &&c) noexcept {
	using signature = boost::callable_traits::args_t<Callable>;
	using in_split = decltype(split_tuple<in_count>(std::declval<signature>()));
	using In = std::tuple_element_t<0, in_split>;
	using InOut_Out = std::tuple_element_t<1, in_split>;
	using inout_out_split = decltype(split_tuple<inout_count>(std::declval<InOut_Out>()));

	using InOut = std::tuple_element_t<0, inout_out_split>;
	using Out = std::tuple_element_t<1, inout_out_split>;
	return make_out_param_adapter(std::forward<Callable>(c), In{}, InOut{}, Out{});
}

template <typename... T>
using in = std::tuple<T...>;
template <typename... T>
using inout = std::tuple<T...>;
template <typename... T>
using out = std::tuple<T...>;

CHAR16 *operator""_C16(char16_t const *str, size_t) {
	return (CHAR16 *)str;
}
