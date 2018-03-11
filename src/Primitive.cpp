#include "Primitive.h"

namespace Primitive {
	float sphere(const lmi::vec3 &center, float radius) {
		return length(center) - radius;
	}
}	// namespace Primitive
