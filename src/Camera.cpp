#include "Camera.h"

using namespace lmi;

Camera::Camera(float fovy, vec2i resolution, const vec3 &position, const Quat &orientation) {
	resolution_ = resolution;
	viewMat_ = translate(position.x, position.y, position.z) * static_cast<mat4>(orientation);
	invViewMat_ = inverse(viewMat_);

	auto aspect = resolution.x / static_cast<float>(resolution.y);
	projMat_ = perspective(fovy, aspect, zNear, zFar);
	invProjMat_ = inverse(projMat_);
}

Ray Camera::rayFromPixel(vec2i pixel) {
	// Search for point on the far plane
	vec4 tmpDest(invProjMat_ * vec4(2.0f * pixel.x / resolution_.x - 1, 2.0f * pixel.y / resolution_.y - 1, -1, 1));
	tmpDest /= tmpDest.w;

	vec3 origin(invViewMat_[3].x, invViewMat_[3].y, invViewMat_[3].z);
	vec3 direction(tmpDest.x, tmpDest.y, tmpDest.z);
	direction = normalize(direction);
	float minT = zNear;
	float maxT = zFar;

	return {origin, minT, direction, maxT};
}

vec3 Camera::getPosition() const {
	vec4 v;
	auto pos = viewMat_ * v * -1;
	return vec3(pos.x, pos.y, pos.z);
}


const float Camera::zNear = 0.1f;
const float Camera::zFar = 100;
