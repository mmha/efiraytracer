#include <cstddef>

extern "C" [[noreturn]] void _assert(const char *, const char *, unsigned int) {
	while(true)
		;
}

extern "C" int atexit(void (*)()) {
	return 0;
}

extern "C" void *memmove(void *dest, const void *src, size_t n) {
	auto pd = reinterpret_cast<unsigned char *>(dest) + n;
	auto ps = reinterpret_cast<const unsigned char *>(src) + n;

	while(n--) {
		*--pd = *--ps;
	}

	return dest;
}