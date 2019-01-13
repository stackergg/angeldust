#pragma once
#include "../../../dependencies/common_includes.hpp"

class visuals : public singleton< visuals > {
public:
	void think( );
	void glow();
	void chams();
	void radar();
	void entities();
	void inaccuracy();
	void crosshair();
	void grenade_trajectory();
	void spectators();

private:
	int alpha[ 65 ];
	int arrow_alpha[ 65 ];
	bool last_dormant[ 65 ];

	float meme = 0.f;
	bool meme_direction;

	struct box {
		int x, y, w, h;
		box( ) = default;
		box( int x, int y, int w, int h ) {
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
		}
	};

	bool get_playerbox( entity_t* ent, box& in ) {
		vec3_t origin, min, max, flb, brt, blb, frt, frb, brb, blt, flt;
		float left, top, right, bottom;

		origin = ent->get_absolute_origin( );
		min = ent->collideable( )->mins( ) + origin;
		max = ent->collideable( )->maxs( ) + origin;

		vec3_t points[ ] = {
			vec3_t( min.x, min.y, min.z ),
			vec3_t( min.x, max.y, min.z ),
			vec3_t( max.x, max.y, min.z ),
			vec3_t( max.x, min.y, min.z ),
			vec3_t( max.x, max.y, max.z ),
			vec3_t( min.x, max.y, max.z ),
			vec3_t( min.x, min.y, max.z ),
			vec3_t( max.x, min.y, max.z )
		};

		if ( !interfaces::debug_overlay->world_to_screen( points[ 3 ], flb ) || !interfaces::debug_overlay->world_to_screen( points[ 5 ], brt )
			|| !interfaces::debug_overlay->world_to_screen( points[ 0 ], blb ) || !interfaces::debug_overlay->world_to_screen( points[ 4 ], frt )
			|| !interfaces::debug_overlay->world_to_screen( points[ 2 ], frb ) || !interfaces::debug_overlay->world_to_screen( points[ 1 ], brb )
			|| !interfaces::debug_overlay->world_to_screen( points[ 6 ], blt ) || !interfaces::debug_overlay->world_to_screen( points[ 7 ], flt ) )
			return false;

		vec3_t arr[ ] = { flb, brt, blb, frt, frb, brb, blt, flt };

		left = flb.x;
		top = flb.y;
		right = flb.x;
		bottom = flb.y;

		for ( int i = 1; i < 8; i++ ) {
			if ( left > arr[ i ].x )
				left = arr[ i ].x;
			if ( bottom < arr[ i ].y )
				bottom = arr[ i ].y;
			if ( right < arr[ i ].x )
				right = arr[ i ].x;
			if ( top > arr[ i ].y )
				top = arr[ i ].y;
		}

		in.x = ( int )left;
		in.y = ( int )top;
		in.w = int( right - left );
		in.h = int( bottom - top );

		return true;
	}

	i_material *lit = interfaces::material_system->find_material( "angeldust_lit", "Model textures", true, nullptr ), *material = interfaces::material_system->find_material( "angeldust_material", "Model textures", true, nullptr );

	void rotate_arrow( std::array< vec2_t, 3 >& points, float rotation );
	void arrows( player_t* chuj );
	void player_rendering( player_t * player );

};