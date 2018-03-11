#pragma once

#include "Ray.h"
#include <lmi/lmi.h>

class Camera {
	public:
	/**
	 * Construct a pinhole camera suitable for offline raytracing
	 * @param fovy Field of View on the Y-Axis
	 * @param resolution the framebuffers resolution
	 * @param position The position of the Camera
	 * @param orientation The orientation of the Camera
	 */
	Camera(float fovy,
		   lmi::vec2i resolution,
		   const lmi::vec3 &position,
		   const lmi::Quat &orientation);	// Lets assume square pixels

	/**
	 * Calculate the ray using the pixels coordinate
	 * @param pixel The Pixels coordinate in absolute device coordinates
	 * @returns The ray which belongs to the pixel
	 */
	Ray rayFromPixel(lmi::vec2i pixel);

	lmi::vec3 getPosition() const;

	// TODO
	/*
	void setFovy ( float fovy );
	void setResolution ( const lmi::vec2i resolution );
	void setPosition ( const lmi::vec3 &position );
	void setOrientation ( const lmi::Quat &orientation );
	*/
	private:
	const static float zNear;
	const static float zFar;
	lmi::mat4 viewMat_, projMat_, invViewMat_, invProjMat_;
	lmi::vec2i resolution_;
};
