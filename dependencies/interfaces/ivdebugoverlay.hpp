#pragma once
#include "../../source-sdk/math/vector3d.hpp"
#include "../../source-sdk/misc/color.hpp"

class iv_debug_overlay {
public:
	void draw_box(vec3_t &origin, vec3_t &min, vec3_t &max, vec3_t &rotation, color col, float duration) {
		using original_fn = void(__thiscall*)(void *, vec3_t &, vec3_t &, vec3_t &, vec3_t &, int, int, int, int, float);
		(*(original_fn**)this)[2](this, origin, min, max, rotation, col.r, col.g, col.b, col.a, duration);
	}

	bool world_to_screen( const vec3_t& in, vec3_t& out ) {
		using original_fn = int( __thiscall* )( iv_debug_overlay*, const vec3_t&, vec3_t& );
		int return_value = ( *( original_fn** )this )[ 13 ]( this, in, out );
		return static_cast< bool >( return_value != 1 );
	}

};