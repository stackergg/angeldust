#include "math.hpp"

float math::dot_product( const float* a, const float* b ) {
	return ( a[ 0 ] * b[ 0 ] + a[ 1 ] * b[ 1 ] + a[ 2 ] * b[ 2 ] );
}

void math::transform_out( const float *in1, const matrix_t& in2, float *out ) {
	out[ 0 ] = dot_product( in1, in2[ 0 ] ) + in2[ 0 ][ 3 ];
	out[ 1 ] = dot_product( in1, in2[ 1 ] ) + in2[ 1 ][ 3 ];
	out[ 2 ] = dot_product( in1, in2[ 2 ] ) + in2[ 2 ][ 3 ];
}

void math::vec3_transform( const vec3_t& in1, const matrix_t &in2, vec3_t &out ) {
	transform_out( &in1.x, in2, &out.x );
}

vec3_t math::calculate_angle( const vec3_t& vecSource, const vec3_t& vecDestination )
{
	vec3_t qAngles;
	vec3_t delta = vec3_t( ( vecSource[ 0 ] - vecDestination[ 0 ] ), ( vecSource[ 1 ] - vecDestination[ 1 ] ), ( vecSource[ 2 ] - vecDestination[ 2 ] ) );
	double hyp = std::sqrtf( delta[ 0 ] * delta[ 0 ] + delta[ 1 ] * delta[ 1 ] );
	qAngles[ 0 ] = ( float ) (std::atan( delta[ 2 ] / hyp ) * ( 180.0 / M_PI ) );
	qAngles[ 1 ] = ( float ) (std::atan( delta[ 1 ] / delta[ 0 ] ) * ( 180.0 / M_PI ) );
	qAngles[ 2 ] = 0.f;
	if ( delta[ 0 ] >= 0.f )
		qAngles[ 1 ] += 180.f;

	return qAngles;
}

void math::vector_angles( const vec3_t& forward, vec3_t &angles ) {
	float tmp, yaw, pitch;

	if ( forward[ 1 ] == 0 && forward[ 0 ] == 0 ) {
		yaw = 0;
		if ( forward[ 2 ] > 0 )
			pitch = 270;
		else
			pitch = 90;
	}
	else {
		yaw = (std::atan2( forward[ 1 ], forward[ 0 ] ) * 180 / M_PI );
		if ( yaw < 0 )
			yaw += 360;

		tmp = std::sqrt( forward[ 0 ] * forward[ 0 ] + forward[ 1 ] * forward[ 1 ] );
		pitch = (std::atan2( -forward[ 2 ], tmp ) * 180 / M_PI );
		if ( pitch < 0 )
			pitch += 360;
	}

	angles[ 0 ] = pitch;
	angles[ 1 ] = yaw;
	angles[ 2 ] = 0;
}


vec3_t math::nigger( vec3_t source, vec3_t destination ) {
	vec3_t ret;
	vector_angles( destination - source, ret );
	return ret;
}
