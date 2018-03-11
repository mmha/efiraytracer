---
author: Morris Hafner (@mmha)
title: UEFI Applications with C++
date: emBO++ 8th March 2018
---

## What is (U)EFI?
- "New" firmware interface for x86, ARM, RISC-V
- Many features compared to BIOS
   - Network, Secure Boot, NVRAM, EFI Bytecode, ...
- No assembly required for many applications (e. g. bootloaders)

## EFI executables
- COFF file format, subsystems 10-13
- MS calling convention
- UTF-16 strings
- On a FAT32 partition with partition type `0xEF00`
- Implementations usually default to executing `EFI/Boot/bootx64.efi`

# First Executable

## Toolchain
- TianoCore EDK II, gnu efilib
- We will use gnu efilib, it is easier to use
- Available compilers: MSVC, MinGW gcc
- No clang
	- Doesn't support freestanding COFF executables

## Standard Library
- no C or C++ standard library
  - A C95 stdlib implementation is part of EDK II
- Heap not easily accessible
- In practice, many headers can still be used
   - most of `<algorithm>`, `<array>`,`<tuple>`,`<type_traits>`
- See Ben Craig's talk and proposal (p0829r1) to fix this

## Compiler Invocation
```
x86_64-w64-mingw32-g++ \
  -mno-red-zone -DEFI_FUNCTION_WRAPPER \
  -ffreestanding -fshort-wchar \ 
  -nostdlib -e efi_main \
  -Wl,-dll -shared -Wl,--subsystem,10 \
  -c main.cpp
```
This can be wrapped in a CMake Toolchain

## The code
```c++
#include <efi.h>
#include <efilib.h>

extern "C" [[gnu::ms_abi]]
EFI_STATUS efi_main(
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
    SystemTable->ConOut->OutputString(
      SystemTable->ConOut,
      (CHAR16 *) L"Hello World\r\n");
}
```

## Running it
OVMF is an OSS implementation that can be used with QEMU
```bash
qemu-system-x86_64 \
-drive file=hdd.img,if=ide \
-bios OVMF.fd
```

# Accessing EFI Services

## The EFI Protocol Interface
- Firmware services can be queried using GUIDs
- Everything is loaded into structs
- OO style interface, but in C with function pointers
```c
EFI_STATUS
LocateDevicePath (
 IN     EFI_GUID                 *Protocol,
 IN OUT EFI_DEVICE_PATH_PROTOCOL **DevicePath,
 OUT    EFI_HANDLE               *Device
 );
```

## Hmmm...
Signature is usually

`EFI_STATUS(in..., inout *..., out *...)`

In C++, we want this:
```cpp
expected<tuple<out...>, EFI_STATUS>
  const res = func(in const..., inout &...);
if(res) {
   auto[out...] = res.value();
}
```

## What to do?
```c++
template <size_t in_count, size_t inout_count,
          size_t out_count = SIZE_MAX, typename Callable>
auto wrap(Callable c);
auto locate_device_path =
  wrap<1, 1, 1>(LocateDevicePath);
expected<tuple<EFI_HANDLE>, EFI_STATUS> result =
  locate_device_path(protocol, device_path);
```
1. Partition the argument list
2. Create a new function wrapping the EFI function
3. Wrap the error codes

## Getting the Argument List
→ Boost Callable Traits
```c++
EFI_STATUS
LocateDevicePath(EFI_GUID *,
	EFI_DEVICE_PATH_PROTOCOL **,
	EFI_HANDLE *);

#include <boost/callable_traits/args.hpp>
using args =
  boost::callable_traits::args_t<decltype(LocateDevicePath)>;
static_assert(is_same_v<args,
  tuple<EFI_GUID *,
        EFI_DEVICE_PATH_PROTOCOL **,
        EFI_HANDLE *>
);
```

## Partition the Argument List
```c++
template <size_t offset, typename T, size_t... I>
auto constexpr st_impl(T tuple, index_sequence<I...>) {
    return tuple{get<offset + I>(tuple)...};
}

template <size_t N, typename T>
auto constexpr split_tuple(T tuple) {
  auto constexpr sz = tuple_size_v<T>;
  return tuple{
    st_impl<0>(tuple, make_index_sequence<N>{}),
    st_impl<N>(tuple, make_index_sequence<sz - N>{}),
  };
}
```

## 3-way split
```c++
using in_split =
  decltype(split_tuple<in_count>(declval<args>()));
using In = // <- first
  tuple_element_t<0, in_split>;
using InOut_Out =
  tuple_element_t<1, in_split>;
using inout_out_split =
  decltype(split_tuple<inout_count>(declval<InOut_Out>()));
using InOut = // <- second
  tuple_element_t<0, inout_out_split>;
using Out = // <- third
  tuple_element_t<1, inout_out_split>;
```

## Wrap the Function
```c++
template <typename Func,
  typename... Is, typename... IOs, typename... Os>
auto constexpr make_out_param_adapter(
  Func func, tuple<Is...>, tuple<IOs...>, tuple<Os...>) {
    return [func](Is... is, IOs... ios) {
      tuple<remove_pointer_t<Os>...> res;
      auto const ptr = transform_tuple(res, [](auto &val) {
        return addressof(val);
      });
      apply(c, tuple_cat(
        tuple{get_ptr(is)...}, tuple{get_ptr(ios)...}, ptr));
      return res;
    };
}
```

## Overhead?
<iframe width="100%" height="500px" src="https://godbolt.org/e#g:!((g:!((g:!((h:codeEditor,i:(fontScale:2.6748301344767995,j:1,lang:c%2B%2B,source:'%23include+%3Ctuple%3E%0Aextern+%22C%22+void+f(int+*out)%3B%0Aint+c()+%7B%0A++++int+r%3B%0A++++f(%26r)%3B%0A++++return+r%3B%0A%7D%0Aauto+cpp()+%7B%0A++++std::tuple%3Cint%3E+r%3B%0A++++f(%26std::get%3C0%3E(r))%3B%0A++++return+r%3B%0A%7D'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:60,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((h:compiler,i:(compiler:clang600,filters:(b:'0',binary:'1',commentOnly:'0',demangle:'0',directives:'0',execute:'1',intel:'0',trim:'0',undefined:'1'),fontScale:2.2290251120639994,lang:c%2B%2B,libs:!(),options:'-std%3Dc%2B%2B17+-O3',source:1),l:'5',n:'0',o:'x86-64+clang+6.0.0+(Editor+%231,+Compiler+%231)+C%2B%2B',t:'0')),k:40,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',n:'0',o:'',t:'0')),version:4"></iframe>

## `aligned_storage`!
`std::tuple` causes value initialization

```c++
template <typename T> struct storage_for {
  storage_for() {}
  auto get() {
    return reinterpret_cast<T *>(&val);
  }
  aligned_storage_t<sizeof(T), alignof(T)> val;
};
```

```c++
tuple<storage_for<remove_pointer_t<Os>>...> res;
auto const ptr = transform_tuple(res, [](auto &val) {
  return val.get();
});
```

## Wrap the error codes
- `expected<T, E>` contains either a value or an error
- We use `tl::expected<T, E>` (thanks, Simon!)
```c++
if constexpr(
  is_same_v<invoke_result_t<Func, Is..., IOs..., Os...>,
            EFI_STATUS>) {
  using result_t =
    tl::expected<tuple<remove_pointer_t<Os>...>, EFI_STATUS>;
  if(auto result = apply(...) != EFI_SUCCESS)
    return result_t(tl::unexpect, result);
  return result_t{transform_tuple(...)};
} else {
  apply(...);
  return res;
}
```

## Wrap the pointers
We can modify the signature to accept references
```c++
EFI_STATUS func(int *inout);
int x{};
wrap<1, 0, 0>(func)(x);
```
Create a struct constructible with a reference and a pointer

## Overhead?
<iframe width="100%" height="500px" src="https://godbolt.org/e#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAM1QDsCBlZAQwBtMQBGAOgA4BWAOz8ATAAYAnCIBsgzhIUTBpAFZdSrZrVDIApCIBC%2Bg6QDOqAK7FkHAOT6AzHlrJWFrAGpdDgMIAjVFRTAgB6FlZNP3YAfQJiZkJTEOZiYFNuBAAHTO9sXTEAQUdnV3dML18qCxcCPDo2XPyikScXN09vHwQCAkzTEBCQ%2BIB3bmBCBAs/C1NMazoCTHpuNABbEIAVFIItABlNVeYQzAAPTMxkRfQQw%2BC5kIJWY7OLq4zsxsLitrKKnwILJl2J9mq1Sh1fAQAJ7nWLxRIgpozZzADzYABiAElogwNgUNgBVBgVAAiHmcBG8Ri%2BhUWqyBzEWf1MeAAXphYh5UFQqLMCKQPNDzrRmKtyhsBSz2bFuLKPJiQcwLEQPGhaHczsQPKYgYRYoCYnh6awIBsvDJ9NIAUDMJKCOgQCBnFgTtFZgBHCxLGydTGy7i5ACUHloqFONkyBAgofDmEjEGCDpA1uBgmpBQ8me19sdwEwlN83N5%2BfNBnluQTOZANGIwxS6E6GwrKcwgcD/t0ghJba8aaaWY8xHzVlo2aTLc7BkTufznSLfNL5Yc2ErSZrdeIDd8TeXEBbbY7XapTU7JJPtMwxsZ5U6Uo5BA8ADkBUKlqLxYrlahVXQNZktTqrB6i2prmtIlr7iGYYnBGUYxjBcZRtOyYGpgk79lmgHAah0RGkCnRiBWyHrvWjbNqhgZ2kmhwANYcs6pxupgnremhviPo0fZdpRGGZlhBD6jauHGp0HG7sRJAbluPg7iu%2B5UY6tH0bQLpMSxLhsT4yEtm6bIcgAbreVZYCwUKxGRy65B4AC0T6cUY3GkCe3HBuhNISEqKpqn%2BAGsqSY6Ojpd7RIZvjISZzBmQWMmNJZDjpvkEhDgCxCjtpqFuUUkiJfxgmGiJviEeJVYkZuFlyRRCkgEpuEqYxHpehpon2aePHuTluoCTpeGsM1xVrpJpHbuRNqUQF1XMHRtWqQ1rG3n5tliXkXHdk57VHvFzlnjSBR0gyTKdK%2BIpih4GyoS%2BMJvidAAKQ7oFVwUEP6S55IU3mLJqHied%2BcRaKYNarHl9HGqaqFgRBAq3Zg6DgzImRVQxrqzU1vh%2BrKQZQbG8bwbBq6BRlfaFAOEm1kNPhQ1uK6ZIGeMgHm0UKv1jqldJZ02s2bbtujLm9umA7JSO40TmmJNSZ0FMVtTtP076RElYNZXbhlu4EJzh5nptXxHjte2aAdkKXcd4rnYKhvvh4EuWW9v4ff%2BX1foK8TqgDQOgzasNWpDd0e9TmMIfGvF%2B7jv3OyQgPdSDotk2zwIq21GYDonSfJynmHy6Tivk3dkvx6nef58TVY1YjamNT6YVVkFekhUZSZDqsqD6RyQ5UHMrHmUr7NxXklkrZzvOBwLqWO39LsR0CtMs42ytyWNUeZ5bVO5wXK9J8hxd1UjzFl5p6VCcFoVaVW9eN83mCt0OGkdzJM%2BxctDndseWvbVlu2XvtN4G8K5uYrQF3fydX%2BAB5ZU/8rrlBAfyU2ADyg%2BDYJEWOr0CjBGIBYS4XJlTREyCkUU0RmDoGYJGOYT9X662vH8I65tZJNBQWgh8wQSDMDzNEGsA93LIm0NAz%2BZIdwJUkO9U4dsGHxGYTWCArlCZZQkKec8EgBGfW%2Bh4em4ig6ITYVIxKQ9RxDgpHMf8%2BZogsGCI2DwAAqCslp9JsEDCQxKMj3LyLtoo5RwZ3qqMjOouxkgtGDkwLo4g%2BiBJGOimaNx5jdyWOsbY7K2tX4Dn/HgKxiwQCB2QmwPAwBaDQzdEQER95bx6W5KaMa6TMlFI2DY5cHgrG9U1kUDafDSHvz1p/f4ZsxTPUxKYMBRtOlAO6Vw3pcp%2Bk9PNnAiIzAohsSQbQ9BlgBLYPiIDfBhDFjEFrvjLuPgukdmXFVCcqN%2Bm7OwPs5WPgjnoz2R4cZCDpmeP4TbQRWp5lYJwcsghRDiAQBuZM9gqp4G/NbO4yk7kBwgFVLTBuTcIDhFuT2TKdjYmyMcQBSuZzkInyblg1A/jr79NyMcrk5x4i5PERALpHtnp4AGZifplK5R1FMK4m2wL7nSMkHvWOR9GGiJIBskAmKOSZBxfQOYeLTB30JUOCVdSvEeQdm4yMWpvBkhDv9MOrtpUCl0PwIw/ASQQEUZE1gEi%2BHsvNT4nRoqAnJUMcwYxX9wHjQilFToJlWCvggDUyp2BAqXTMXLJM%2BD0DSvMFQL11jKlmsRY/WV2UOVVkIUCKEMKXw4RYEhNF7MRbpzFqjGVK5qVcwDF2U52apy5rJrSgtEBGXFtPAKJVbZokWuHMPNVY9UIQCVdq3VOqDVGpkN63mviUqjlMTUqkvZY3RqRQ4x5n1OW72PpeU%2B2LcXRXxcuQlqBiWMhIGSillpLRUppXS49MgqVBGDCEUxrLTEhFkQi%2BNEgl1GR5RyGs/LBXruteKyVlzsC%2BJldGyQijFVxH8h2jVIEtVeD7fqw1DtjWmqfS%2By1fjrWBLtQ6tpMDnUXEitfd1nrvW5D9ecANTMQDBtDUU71Ua0PSO4i2nKibsisBTcgNNQkM202FhWgaGdpJdIrEW9WZbUyCeZgrETW7C3Xokx4JtjH2reLbaOaDxBw5dp7fBvVA7kNDusSOnxE6GjxWnapjRc6E6ZgSUkjggcflTNVLYhp54mlXn1v8Zp5DDrtLuZQk6EpBnmwJCCFFHgAioFYOSUwtVghaBsP5KgbBZjRLft51pZCfPBaC4F06oyToRatgUKLMW4vUsSzsFGMkBQ7h8KVnuQGVWOy9Jl3LOXCvUMKLMh8SqW5sqi4N8%2B4jZFguU9EaMFgIhNrZfY1%2BiURtxBbrTWgs3WBKtiDY0FWZwWZGmxtubcRUPrRfrIrrFDCvNeRQuu2o3w0EjAr7HGiEJv7amxAS0vtn3MYu%2B5K7AX8O3fneqW2WpHsQGe6Y170FYIfczAd6bv3JExs88t%2B7zy925NOmYlRbi3seL%2B5ojTU3WO2cx%2BDp5ePTEuJ/OD1lJP1NjvJ3G/7nn4nEESdeFJRMsxmlh9EdzGtGlA8dUbU6kWsf2xVPTLBcRQIXqtMGInD4/t4CoAznytaEvOCS3Vx7VUXXX2ocuM7GiWeC3nqzDmYx8zjfZ6eDwmBWCzAW1b9tFOLu2fF3hp1vWCgcNREOBkNgFfKocGSZCap8C1HqKwa%2ByFqvCtxYfWSjbVvn2/aurFqe/0hLvg12KYu/N5cKy59gxXMCdIGflvpdfAvPXk00cDMuaovMWbglZnzvkAtc8r2FgK1p2dXmPteWauU7MA9r9XMgR/j8X4nN9hz0gz7cZaBfS/t8r/OWvgMVSN8yAt/zMnOqDDIH8jbzolfpkwv7%2BwGxiHQ%2BaHD0q30phZ9WXeqegUL/mBv5xC%2BjyZXpMrqIpy77CJMKfp8oVx1y55CoiprL/rdxSqYAgaBwDht6M5KpQZOzqraaaroG9oGZIYqgobgH5wYb%2BLYbBLA5OrhSEauq%2BAkaXQRomrkbJj%2BrhIrhpLoAhroFhrsHNp1IpytQkIpya6z5PK0zVamDvg1xwGOjOD6SoBTTSqzZdQ34P62jyj74CjVr%2BgCgXIH4nJohYg4h4iEgMAYyZQFzB7AaaGchtaPCOiCKvDQz8oHJHzwFQqIEbqdAgGAYCgYjYi4j4hEgl6YHJzYHBCOEepX7sbJqpqCjpqMj8YEwGDy5Kq671qlpCyZHZGK51pKYqYSH5ya4QAaEJFgBgBtZiAn5j4YamBOFRiuEgDVDuGXB/7oGaHWYFyLYFzNGtGThaY6Y2jdpxAkH9pkHfgUGTijqCzma1JljiEeb85JzO6u7u52H5xpIcZcY8YxB8ZvpphFFfLibcwkiSZoRnEGI5ElFXGZ7EAiF8xUFk7SrlGbGxIDgyKyq%2B5l6tKPTki0CGKWD0CSjVwPjOAvJoDVBQLAmwngnq5R4eAMCYgABa2A0QAAsgUAABrV7XI6GfheQy7DDxCZB94TID4WgyDICq7w5qJ/YOEsiZKMhWDcLRaBDBCOhD5TJwgJAED9A0apAJbRS35RHsIsicLOBuidT%2BSsHnAJidRAydBylwn0CBqOjuqTphQZIigpR36cwtoOG/yJHjg4Su6XhLBaGFQChyn8RSlSJmm0CQLRCQIWmbIxDWlij0DXycAOmglOlxRmoOEwmYIvL8SKkXAepsG5TeERlBLInakgC6kWbbJumYKQIVgmlxoukymojALKhekoRCS%2Bm2nXxiBBkvJRmdTOmJQOGeltZLrRAVn%2BnRSBkgm1mYIhk9xmo%2BId6YJd7vKrJzCTyybaE0mIIwpjS/zoT5HFmUgrTGHKgLkzpbSeZ%2B715yiB7hmjgtmT6aQbDHIkLblN67kgj7nzKlneEnmAZnmAnXYwLPR7mFkYIonR5HmNinn5mnBrIHkiAiA%2BD6AiAgkPjIBVAuC1oQngVmJBkPimLWaxEQ4eAUlJrQz%2BToU5C%2BBdl4UViQXVAMmZbFBUBYBa6PjYAADq0QAAEkAhsJRQwDYYUKoXgDDMgNkCorsZmBSF9OuQKH4OuV8bLt%2BG4tUZ%2BWhZSecOgIaoJf0VmJURJY0UnChfpnoPqv5KYhJSJVzhSOGqBS0cgDYKYJ/voPwA2EBQKMRaIVmIMVmNsTeJIinGpXMJHmSBJdwG5QegpUnAkvQAZUBWlngHFuZZZSIAKG5b5dOpuV8C0EsPgFQBji0JruRR4EArsCSNEAUI%2BFlQYESBsNgC/GxRxdxc5YpfQPxSudFgJW5rZZmIohJf5IRdBcwNqjIH4O1dIDZW8bxeGk1SqvUSpYnP5QQIFSIEZSZWZSIBZaBdZdFfZZmI5ZQX5dzgFd9kFQkKFTNeFT0S0R6gtUikivFSpJrrYJRKwCALYPwLYKQLQFdWILdagFdSBYYMYNqJYNYDeC0JwLdQQA9edZRDRCAAACwiDcAiASC8AOBiDiASAg0SCcDSCBmXW2Ag23X3W2CPWkDPW2C3X9DVn/VY3nWkBwCwBIBrCZAhVzDkCUCU3U3EB0yCAOCkBUAhVrL9AQB%2BAA2kB%2BDOApBQhXW/WkBrB%2BkEBAK0CcY81YCHDaDsDS14BDiXCJLoE82nAXDKh2DC26Ko3Y1xBGgA2UQ0D0BMBsAcDg1AUSCw38CcCcDiDSAOCKAg0aBaA6DGDGAaB4B%2BD9CwBGwgAxbBCkBNyM22BI3SCA042Rh1DqhXXWSJgqp6BvWGCcCCA2RAIOA2QkiUU0X0WMXYDMWZjWRUChjWTEA9B4A2SHDWAIAqqGkq02TF2oDWRYzx7qj42fU2BcAXVXU3V3U8240nC8DSDWTSAg1KLGUeCCDcAZ0QC4CEAkDmgOBdk%2BCoD0gM2L2cDBivVGCGB/WG1A0gBiDcCCAg0g2iBAWcAw2CAiCCDSA20aBXXo193E041XX42H2kBE2PWURk2IAoCr1U3sBkAUAwoAMM1M0s1s0epzCc3c0v180ijECC2h23Wi22kS1S0v0y2u3y1YOK2vAq39Av3q3ICa1C23U608362rCG2s0LCm3y0W2Q3W22322O0KDO2aDaAoDu2GCe3e3wDRjvj%2B08n8jB1C3SDh1tiR2t1ENx32gJ28MGAp1p0Z3WQkgZVZU5V5UFVFWF2N2l3l2V0pDIA11R511Nzt1WCd2b0P3XUY391XWD3D2j3j2X5T0z1z246ODL1gNAOL0iBb1KN73E3SPA1H38C8AI3w0OAOAg3Q0CDQ0s2o1P2Y3Y243v2E20O/0QBICJVUC03UD0PhAcA8ACDCBw0yByCKBKAaAID9CBnED1MgCBN2O91pNPVXUkia5a7OMj1j3AAT0ePVKf59OuODPuPT0hPf0H38DSDT3X2cC8CX1CAp0zW8B2OpOON41mAf1f0R2o0iAOMv0ZOf371B0wPR2g1AA%3D"></iframe>

## We made simplifications
- Assumptions that there are only builtins and PODs
  - All argument types need trivial destructors
- Cannot be overloaded
- Not too bad, C doesn't have overloads or destructors
- But may be wrapped by a future `std::overload`
- Future high level wrappers may implement overloads

# Example application

## Rendering to the Framebuffer
- We could write our own kernel in C++ now...
    - `ExitBootServices()`
- Let's render a couple of spheres onto the framebuffer instead

## Creating a Graphics Out Protocol Instance

First, some wrappers
```c++
// Look for GOP implementations
auto locate_handle_buffer =
  wrap<3, 1, 1>(bootServices->LocateHandleBuffer);
// Create a GOP Instance
auto handle_protocol =
  wrap<2, 0, 1>(bootServices->HandleProtocol);
```

## Yay, monads!
```c++
uint64_t handle_count = 0;
auto maybe_gop =
locate_handle_buffer(ByProtocol, graphicsOutProtocolGUID,
	                 nullptr, handle_count)
 .and_then([&](auto proto_impl) {
     return handle_protocol(get<0>(proto_impl)[0],
                            graphicsOutProtocolGUID);
 })
 .map([](auto opened_gop) {
     return reinterpret_cast<EFI_GRAPHICS_OUTPUT_PROTOCOL *>(
     	get<0>(opened_gop));
 });
if(!maybe_gop)
  conOut->OutputString(conOut, (CHAR16*) u"Fail\r\n");
```
FACT: This slide contains error handling

## A. Sagar: p0931r0

Structured bindings with polymorphic lambdas

To be discussed in Jacksonville
```c++
std::find_if(range, [](auto const& [key, value]) {
  return examine(key, value);
});

locate_handle_buffer(ByProtocol, graphicsOutProtocolGUID,
	                 nullptr, handleCount)
 .and_then([&](auto [proto_impl]) {
     return handle_protocol(proto_impl[0],
                            graphicsOutProtocolGUID);
 });
```

## Create the Framebuffer
```c++
gop = maybe_gop.value();
auto query_mode = wrap<2, 0, 2>(gop->QueryMode);
for(int i = 0;; ++i) {
  auto mode_info = query_mode(gop, i)
  .map([](auto r)->decltype(auto){return *std::get<1>(r);});
  if(!mode_info) break;
  if(mode_info->HorizontalResolution == width &&
     mode_info->VerticalResolution == height &&
     mode_info->PixelFormat ==
     PixelBlueGreenRedReserved8BitPerColor) {
       gop->SetMode(gop, i);
       break;
  }
}
```

## Use the Framebuffer

With emulated double buffering

```c++
void swap_to_screen() const {
  auto const pixel_ptr =
    reinterpret_cast<uint32_t *>(gop->Mode->FrameBufferBase);
  for(auto const& row : rows) {
    std::copy(row.begin(), row.end(), pixel_ptr);
    pixel_ptr += gop->Mode->Info->PixelsPerScanLine;
  }
}
void clear() {
  for(auto &row : rows) {
    std::fill(row.begin(), row.end(), T{});
  }
}
```

## Implicit Surfaces
Remember: Avoided heap

Implicit surfaces using signed distance fields offer a functional representation of a scene

Unfortunately no time for details

```c++
auto scene = [](vec3 const& rayT) {
  return pUnion(sphere(vec3( 10.0, 0.0, 50), 20, rayT),
                sphere(vec3(-10.0, 5.0, 50), 20, rayT));
};
```

## References
- [github.com/mmha/efiraytracer](https://github.com/mmha/efiraytracer)
- [github.com/TartanLlama/expected](https://github.com/TartanLlama/expected/)
- [OSDev Wiki: UEFI Bare Bones](https://wiki.osdev.org/UEFI_Bare_Bones)
- [P0829r1: Freestanding Proposal](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0829r1.html)
- [P0931r0: Structured bindings with polymorphic lambdas](http://open-std.org/JTC1/SC22/WG21/docs/papers/2018/p0931r0.pdf)

# Live Demo
