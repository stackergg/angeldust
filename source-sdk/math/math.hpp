#pragma once
#include <cmath>
#include "vector3d.hpp"
#include "../../dependencies/utilities/singleton.hpp"

#define M_PI		3.14159265358979323846
#define M_PI_2      (M_PI * 2.f)
#define M_PI_F		((float)(M_PI))
#define M_PHI		1.61803398874989484820

#ifndef rad_to_deg
#define rad_to_deg(x)  ((float)(x) * (float)(180.f / M_PI_F))
#endif

#ifndef deg_to_rad
#define deg_to_rad(x)  ((float)(x) * (float)(M_PI_F / 180.f))
#endif

class math {
public:
	void vec3_transform( const vec3_t& in1, const matrix_t &in2, vec3_t &out );
	vec3_t calculate_angle( const vec3_t & vecSource, const vec3_t & vecDestination );

	void vector_angles( const vec3_t & forward, vec3_t & angles );

	vec3_t nigger( vec3_t source, vec3_t destination );

private:
	float dot_product( const float* a, const float* b );
	void transform_out( const float *in1, const matrix_t& in2, float *out );
}; extern math* math_temp;