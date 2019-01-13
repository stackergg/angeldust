#pragma once
#include <cstdint>
#include "../../source-sdk/math/vector3d.hpp"

#define M_PI 3.14159265358979323846

namespace utilities {
	namespace math {
		template <typename t> t clamp_value( t value, t min, t max ) {
			if ( value > max ) {
				return max;
			}
			if ( value < min ) {
				return min;
			}
			return value;
		}

		__forceinline static double rad2deg( int x ) {
			return (float)x * (float)(180.f / (float)M_PI);
		}

		__forceinline static void vector_angles( vec3_t& forward, vec3_t& angles )
		{
			if ( !forward[1] && !forward[0] ) {
				angles[0] = (forward[2] > 0.f) ? 270.0f : 90.0f; // Pitch (up/down)
				angles[1] = 0.0f; //yaw left/right
			}

			else {
				angles[0] = atan2( -forward[2], forward.length_2d( ) ) * -180.f / M_PI;
				angles[1] = atan2( forward[1], forward[0] ) * 180.f / M_PI;
				if ( angles[1] > 90.f ) angles[1] -= 180.f;
				else if ( angles[1] < 90.f ) angles[1] += 180.f;
				else if ( angles[1] == 90.f ) angles[1] = 0.f;
			}

			angles[2] = 0.f;
		}

		__forceinline static float normalize_angle( float angle ) {
			if ( angle > 180.f || angle < -180.f ) {
				auto revolutions = round( abs( angle / 360.f ) );

				if ( angle < 0.f )
					angle = angle + 360.f * revolutions;
				else
					angle = angle - 360.f * revolutions;
			}
			return angle;
		}

		__forceinline static float normalize_float( float arg1 ) {
			if ( arg1 > 180.0 )
				return arg1 - 360.0;
			if ( arg1 < -180.0 )
				arg1 = arg1 + 360.0;
			return arg1;
		}

	}
	namespace game {
		void* capture_interface( const char* mod, const char* iface );
	}

	std::uint8_t* pattern_scan( void* module, const char* signature );
}