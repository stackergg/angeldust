#pragma once
#include "../../../../dependencies/common_includes.hpp"
#include "../../../../dependencies/interfaces/engine_sounds.hpp"
struct dormancy_data
{
	player_t* entity;
	bool	prevstate;
	bool	set;
	int		flags;
	vec3_t	oldpos = vec3_t( 0.f, 0.f, 0.f );
	struct {
		int time;
	} sounddata;
};

namespace dormancy  {
	void position_correction( int stage );
	bool set( const int index );

	bool viable( player_t* entity );

	void sound_dormancy( );
	void adjust_player( const int index );
	void restore_sound_dormancy( );
	bool valid_sound( sound_info& sound );

	inline std::vector<sound_info> soundbuffer;
	inline std::vector<sound_info> sound_list;

	inline dormancy_data players[ 65 ];
};