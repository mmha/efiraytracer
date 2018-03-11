/*
https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAM1QDsCBlZAQwBtMQBGAOgA4BWAOz8ATAAYAnCIBsgzhIUTBpAFZdSrZrVDIApCIBC%2Bg6QDOqAK7FkHAOT6AzHlrJWFrAGpdDgMIAjVFRTAgB6FlZNP3YAfQJiZkJTEOZiYFNuBAAHTO9sXTEAQUdnV3dML18qCxcCPDo2XPyikScXN09vHwQCAkzTEBCQ%2BIB3bmBCBAs/C1NMazoCTHpuNABbEIAVFIItABlNVeYQzAAPTMxkRfQQw%2BC5kIJWY7OLq4zsxsLitrKKnwILJl2J9mq1Sh1fAQAJ7nWLxRIgpozZzADzYABiAElogwNgUNgBVBgVAAiHmcBG8Ri%2BhUWqyBzEWf1MeAAXphYh5UFQqLMCKQPNDzrRmKtyhsBSz2bFuLKPJiQcwLEQPGhaHczsQPKYgYRYoCYnh6awIBsvDJ9NIAUDMJKCOgQCBnFgTtFZgBHCxLGydTGy7i5ACUHloqFONkyBAgofDmEjEGCDpA1uBgmpBQ8me19sdwEwlN83N5%2BfNBnluQTOZANGIwxS6E6GwrKcwgcD/t0ghJba8aaaWY8xHzVlo2aTLc7BkTufznSLfNL5Yc2ErSZrdeIDd8TeXEBbbY7XapTU7JJPtMwxsZ5U6Uo5BA8ADkBUKlqLxYrlahVXQNZktTqrB6i2prmtIlr7iGYYnBGUYxjBcZRtOyYGpgk79lmgHAah0RGkCnRiBWyHrvWjbNqhgZ2kmhwANYcs6pxupgnremhviPo0fZdpRGGZlhBD6jauHGp0HG7sRJAbluPg7iu%2B5UY6tH0bQLpMSxLhsT4yEtm6bIcgAbreVZYCwUKxGRy65B4AC0T6cUY3GkCe3HBuhNISEqKpqn%2BAGsqSY6Ojpd7RIZvjISZzBmQWMmNJZDjpvkEhDgCxCjtpqFuUUkiJfxgmGiJviEeJVYkZuFlyRRCkgEpuEqYxHpehpon2aePHuTluoCTpeGsM1xVrpJpHbuRNqUQF1XMHRtWqQ1rG3n5tliXkXHdk57VHvFzlnjSBR0gyTKdK%2BIpih4GyoS%2BMJvidAAKQ7oFVwUEP6S55IU3mLJqHied%2BcRaKYNarHl9HGqaqFgRBAq3Zg6DgzImRVQxrqzU1vh%2BrKQZQbG8bwbBq6BRlfaFAOEm1kNPhQ1uK6ZIGeMgHm0UKv1jqldJZ02s2bbtujLm9umA7JSO40TmmJNSZ0FMVtTtP076RElYNZXbhlu4EJzh5nptXxHjte2aAdkKXcd4rnYKhvvh4EuWW9v4ff%2BX1foK8TqgDQOgzasNWpDd0e9TmMIfGvF%2B7jv3OyQgPdSDotk2zwIq21GYDonSfJynmHy6Tivk3dkvx6nef58TVY1YjamNT6YVVkFekhUZSZDqsqD6RyQ5UHMrHmUr7NxXklkrZzvOBwLqWO39LsR0CtMs42ytyWNUeZ5bVO5wXK9J8hxd1UjzFl5p6VCcFoVaVW9eN83mCt0OGkdzJM%2BxctDndseWvbVlu2XvtN4G8K5uYrQF3fydX%2BAB5ZU/8rrlBAfyU2ADyg%2BDYJEWOr0CjBGIBYS4XJlTREyCkUU0RmDoGYJGOYT9X662vH8I65tZJNBQWgh8wQSDMDzNEGsA93LIm0NAz%2BZIdwJUkO9U4dsGHxGYTWCArlCZZQkKec8EgBGfW%2Bh4em4ig6ITYVIxKQ9RxDgpHMf8%2BZogsGCI2DwAAqCslp9JsEDCQxKMj3LyLtoo5RwZ3qqMjOouxkgtGDkwLo4g%2BiBJGOimaNx5jdyWOsbY7K2tX4Dn/HgKxiwQCB2QmwPAwBaDQzdEQER95bx6W5KaMa6TMlFI2DY5cHgrG9U1kUDafDSHvz1p/f4ZsxTPUxKYMBRtOlAO6Vw3pcp%2Bk9PNnAiIzAohsSQbQ9BlgBLYPiIDfBhDFjEFrvjLuPgukdmXFVCcqN%2Bm7OwPs5WPgjnoz2R4cZCDpmeP4TbQRWp5lYJwcsghRDiAQBuZM9gqp4G/NbO4yk7kBwgFVLTBuTcIDhFuT2TKdjYmyMcQBSuZzkInyblg1A/jr79NyMcrk5x4i5PERALpHtnp4AGZifplK5R1FMK4m2wL7nSMkHvWOR9GGiJIBskAmKOSZBxfQOYeLTB30JUOCVdSvEeQdm4yMWpvBkhDv9MOrtpUCl0PwIw/ASQQEUZE1gEi%2BHsvNT4nRoqAnJUMcwYxX9wHjQilFToJlWCvggDUyp2BAqXTMXLJM%2BD0DSvMFQL11jKlmsRY/WV2UOVVkIUCKEMKXw4RYEhNF7MRbpzFqjGVK5qVcwDF2U52apy5rJrSgtEBGXFtPAKJVbZokWuHMPNVY9UIQCVdq3VOqDVGpkN63mviUqjlMTUqkvZY3RqRQ4x5n1OW72PpeU%2B2LcXRXxcuQlqBiWMhIGSillpLRUppXS49MgqVBGDCEUxrLTEhFkQi%2BNEgl1GR5RyGs/LBXruteKyVlzsC%2BJldGyQijFVxH8h2jVIEtVeD7fqw1DtjWmqfS%2By1fjrWBLtQ6tpMDnUXEitfd1nrvW5D9ecANTMQDBtDUU71Ua0PSO4i2nKibsisBTcgNNQkM202FhWgaGdpJdIrEW9WZbUyCeZgrETW7C3Xokx4JtjH2reLbaOaDxBw5dp7fBvVA7kNDusSOnxE6GjxWnapjRc6E6ZgSUkjggcflTNVLYhp54mlXn1v8Zp5DDrtLuZQk6EpBnmwJCCFFHgAioFYOSUwtVghaBsP5KgbBZjRLft51pZCfPBaC4F06oyToRatgUKLMW4vUsSzsFGMkBQ7h8KVnuQGVWOy9Jl3LOXCvUMKLMh8SqW5sqi4N8%2B4jZFguU9EaMFgIhNrZfY1%2BiURtxBbrTWgs3WBKtiDY0FWZwWZGmxtubcRUPrRfrIrrFDCvNeRQuu2o3w0EjAr7HGiEJv7amxAS0vtn3MYu%2B5K7AX8O3fneqW2WpHsQGe6Y170FYIfczAd6bv3JExs88t%2B7zy925NOmYlRbi3seL%2B5ojTU3WO2cx%2BDp5ePTEuJ/OD1lJP1NjvJ3G/7nn4nEESdeFJRMsxmlh9EdzGtGlA8dUbU6kWsf2xVPTLBcRQIXqtMGInD4/t4CoAznytaEvOCS3Vx7VUXXX2ocuM7GiWeC3nqzDmYx8zjfZ6eDwmBWCzAW1b9tFOLu2fF3hp1vWCgcNREOBkNgFfKocGSZCap8C1HqKwa%2ByFqvCtxYfWSjbVvn2/aurFqe/0hLvg12KYu/N5cKy59gxXMCdIGflvpdfAvPXk00cDMuaovMWbglZnzvkAtc8r2FgK1p2dXmPteWauU7MA9r9XMgR/j8X4nN9hz0gz7cZaBfS/t8r/OWvgMVSN8yAt/zMnOqDDIH8jbzolfpkwv7%2BwGxiHQ%2BaHD0q30phZ9WXeqegUL/mBv5xC%2BjyZXpMrqIpy77CJMKfp8oVx1y55CoiprL/rdxSqYAgaBwDht6M5KpQZOzqraaaroG9oGZIYqgobgH5wYb%2BLYbBLA5OrhSEauq%2BAkaXQRomrkbJj%2BrhIrhpLoAhroFhrsHNp1IpytQkIpya6z5PK0zVamDvg1xwGOjOD6SoBTTSqzZdQ34P62jyj74CjVr%2BgCgXIH4nJohYg4h4iEgMAYyZQFzB7AaaGchtaPCOiCKvDQz8oHJHzwFQqIEbqdAgGAYCgYjYi4j4hEgl6YHJzYHBCOEepX7sbJqpqCjpqMj8YEwGDy5Kq671qlpCyZHZGK51pKYqYSH5ya4QAaEJFgBgBtZiAn5j4YamBOFRiuEgDVDuGXB/7oGaHWYFyLYFzNGtGThaY6Y2jdpxAkH9pkHfgUGTijqCzma1JljiEeb85JzO6u7u52H5xpIcZcY8YxB8ZvpphFFfLibcwkiSZoRnEGI5ElFXGZ7EAiF8xUFk7SrlGbGxIDgyKyq%2B5l6tKPTki0CGKWD0CSjVwPjOAvJoDVBQLAmwngnq5R4eAMCYgABa2A0QAAsgUAABrV7XI6GfheQy7DDxCZB94TID4WgyDICq7w5qJ/YOEsiZKMhWDcLRaBDBCOhD5TJwgJAED9A0apAJbRS35RHsIsicLOBuidT%2BSsHnAJidRAydBylwn0CBqOjuqTphQZIigpR36cwtoOG/yJHjg4Su6XhLBaGFQChyn8RSlSJmm0CQLRCQIWmbIxDWlij0DXycAOmglOlxRmoOEwmYIvL8SKkXAepsG5TeERlBLInakgC6kWbbJumYKQIVgmlxoukymojALKhekoRCS%2Bm2nXxiBBkvJRmdTOmJQOGeltZLrRAVn%2BnRSBkgm1mYIhk9xmo%2BId6YJd7vKrJzCTyybaE0mIIwpjS/zoT5HFmUgrTGHKgLkzpbSeZ%2B715yiB7hmjgtmT6aQbDHIkLblN67kgj7nzKlneEnmAZnmAnXYwLPR7mFkYIonR5HmNinn5mnBrIHkiAiA%2BD6AiAgkPjIBVAuC1oQngVmJBkPimLWaxEQ4eAUlJrQz%2BToU5C%2BBdl4UViQXVAMmZbFBUBYBa6PjYAADq0QAAEkAhsJRQwDYYUKoXgDDMgNkCorsZmBSF9OuQKH4OuV8bLt%2BG4tUZ%2BWhZSecOgIaoJf0VmJURJY0UnChfpnoPqv5KYhJSJVzhSOGqBS0cgDYKYJ/voPwA2EBQKMRaIVmIMVmNsTeJIinGpXMJHmSBJdwG5QegpUnAkvQAZUBWlngHFuZZZSIAKG5b5dOpuV8C0EsPgFQBji0JruRR4EArsCSNEAUI%2BFlQYESBsNgC/GxRxdxc5YpfQPxSudFgJW5rZZmIohJf5IRdBcwNqjIH4O1dIDZW8bxeGk1SqvUSpYnP5QQIFSIEZSZWZSIBZaBdZdFfZZmI5ZQX5dzgFd9kFQkKFTNeFT0S0R6gtUikivFSpJrrYJRKwCALYPwLYKQLQFdWILdagFdSBYYMYNqJYNYDeC0JwLdQQA9edZRDRCAAACwiDcAiASC8AOBiDiASAg0SCcDSCBmXW2Ag23X3W2CPWkDPW2C3X9DVn/VY3nWkBwCwBIBrCZAhVzDkCUCU3U3EB0yCAOCkBUAhVrL9AQB%2BAA2kB%2BDOApBQhXW/WkBrB%2BkEBAK0CcY81YCHDaDsDS14BDiXCJLoE82nAXDKh2DC26Ko3Y1xBGgA2UQ0D0BMBsAcDg1AUSCw38CcCcDiDSAOCKAg0aBaA6DGDGAaB4B%2BD9CwBGwgAxbBCkBNyM22BI3SCA042Rh1DqhXXWSJgqp6BvWGCcCCA2RAIOA2QkiUU0X0WMXYDMWZjWRUChjWTEA9B4A2SHDWAIAqqGkq02TF2oDWRYzx7qj42fU2BcAXVXU3V3U8240nC8DSDWTSAg1KLGUeCCDcAZ0QC4CEAkDmgOBdk%2BCoD0gM2L2cDBivVGCGB/WG1A0gBiDcCCAg0g2iBAWcAw2CAiCCDSA20aBXXo193E041XX42H2kBE2PWURk2IAoCr1U3sBkAUAwoAMM1M0s1s0epzCc3c0v180ijECC2h23Wi22kS1S0v0y2u3y1YOK2vAq39Av3q3ICa1C23U608362rCG2s0LCm3y0W2Q3W22322O0KDO2aDaAoDu2GCe3e3wDRjvj%2B08n8jB1C3SDh1tiR2t1ENx32gJ28MGAp1p0Z3WQkgZVZU5V5UFVFWF2N2l3l2V0pDIA11R511Nzt1WCd2b0P3XUY391XWD3D2j3j2X5T0z1z246ODL1gNAOL0iBb1KN73E3SPA1H38C8AI3w0OAOAg3Q0CDQ0s2o1P2Y3Y243v2E20O/0QBICJVUC03UD0PhAcA8ACDCBw0yByCKBKAaAID9CBnED1MgCBN2O91pNPVXUkia5a7OMj1j3AAT0ePVKf59OuODPuPT0hPf0H38DSDT3X2cC8CX1CAp0zW8B2OpOON41mAf1f0R2o0iAOMv0ZOf371B0wPR2g1AA%3D
*/

// Invocation line:
// g++ -std=c++17 -O3 -DOLD_AND_BUSTED
// or
// g++ -std=c++17 -O3 -DNEW_HOTNESS

#include <boost/callable_traits/args.hpp>
#include <functional>
#include <https://raw.githubusercontent.com/TartanLlama/expected/master/tl/expected.hpp>
#include <tuple>
#include <type_traits>

using EFI_STATUS = int;

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
            if(result != 0) {
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

extern "C" int cfunc(int, int *, int *);
auto constexpr wrapped = wrap<1, 1, 1>(cfunc);

#ifdef NEW_HOTNESS
void cpp() {
    int a{}, b{};
    auto const result = wrapped(a, b);
    if(result) {
        auto const [c] = *result;
        printf("success %d", c);
    }
    else {
        auto const err = result.error();
        printf("fail %d", err);
    }
}
#endif

#ifdef OLD_AND_BUSTED
void c() {
    int a{}, b{}, c;
    auto result = cfunc(a, &b, &c);
    if(result == 0) {
        printf("success %d", c);
    }
    else {
        printf("fail %d", result);
    }
}
#endif