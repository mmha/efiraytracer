#include <cstdint>

// TODO Use SSE
// http://www.myreckonings.com/Dead_Reckoning/Online/Materials/Fast_Approximation_of_Elementary_Functions.pdf
extern "C" float tanf(float x)
{
	float tan;
	__asm__("FPTAN;" : "=t"(tan) : "0"(x));
	return tan;
}

extern "C" double sqrt(double x)
{
	__asm__("sqrtsd %1, %0" : "+x"(x) : "x"(x), "x"(x));
	return x;
}

// http://martin.ankerl.com/2012/01/25/optimized-approximative-pow-in-c-and-cpp/
extern "C" double pow(double a, double b)
{
	union {
		double d;
		int32_t x[2];
	} u = {a};
	u.x[1] = static_cast<int32_t>(b * (u.x[1] - 1072632447) + 1072632447);
	u.x[0] = 0;
	return u.d;
}