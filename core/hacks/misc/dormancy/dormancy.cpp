#include "dormancy.hpp"

#define range( x, a, b )    ( x >= a && x <= b )

void dormancy::position_correction( int stage ) {
	auto local = ( player_t* )interfaces::entity_list->get_client_entity( interfaces::engine->get_local_player( ) );

	for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
		auto entity = ( player_t* )interfaces::entity_list->get_client_entity( i );

		if ( entity == local )
			continue;

		if ( !viable( entity ) )
			continue;

		if ( !entity->dormant( ) && !players[ i ].set )
			players[ i ].oldpos = entity->origin( );

		if ( !entity->dormant( ) || players[ i ].set )
			continue;

		entity->set_position( players[ i ].oldpos );
	}
}

bool dormancy::set( const int index ) {
	if ( !range( index, 1, 64 ) )
		return false;

	return players[ index ].set;
}

bool dormancy::viable( player_t* entity ) {
	auto local = ( player_t* )interfaces::entity_list->get_client_entity( interfaces::engine->get_local_player( ) );

	if ( !entity )
		return false;

	if ( !local )
		return false;

	if ( !entity->health( ) > 0 || entity->flags( ) & ( 1 << 6 ) )
		return false;

	if ( entity->team( ) == local->team( ) )
		return false;

	if ( entity->clientclass( )->class_id != 38 )
		return false;

	if ( entity->has_gun_game_immunity( ) )
		return false;

	return true;
}

void dormancy::sound_dormancy( ) {
	std::vector<sound_info> sfdrd;
	sound_list.clear( );
	interfaces::engine_sound->get_active_sounds( sfdrd );

	if ( sfdrd.empty( ) )
		return;

	//sound_list = sfdrd;
	for ( int i = 0; i < sound_list.size( ); i++ ) {
		sound_info& sound = sound_list[ i ];
		if ( !range( sound.sound_source, 1, 64 ) )
			continue;

		if ( sound.sound_source == interfaces::engine->get_local_player( ) )
			continue;

		auto entity = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( sound.sound_source ) );

		if ( !entity || sound.origin == &vec3_t( 0, 0, 0 ) || !valid_sound( sound ) )
			continue;

		auto& player = players[ sound.sound_source ];
		player.entity = entity;


		/*vec3_t src_3d, dst_3d;
		trace_t tr;
		ray_t ray;
		trace_filter filter;

		filter.skip = entity;
		src_3d = ( *sound.origin ) + sdk::vec3( 0, 0, 1 );
		dst_3d = src_3d - sdk::vec3( 0, 0, 100 );
		ray.initialize( src_3d, dst_3d );

		interfaces::trace->TraceRay( ray, MASK_PLAYERSOLID, &filter, &tr );

		if ( tr.allsolid ) // if stuck
			player.sounddata.time = -1;

		*sound.origin = ( ( tr.flFraction < 0.97 ) ? tr.end : *sound.origin );
		player.flags |= ( tr.flFraction < 0.50f ? ( 1 << 1 ) : 0 ) | ( tr.flFraction != 1 ? ( 1 << 0 ) : 0 );
		player.flags &= ( tr.flFraction > 0.50f ? ~( 1 << 1 ) : 0 ) | ( tr.flFraction == 1 ? ~( 1 << 0 ) : 0 );*/

		player.flags = entity->flags( );
		player.oldpos = *sound.origin;
		player.sounddata.time = GetTickCount( );

		if ( !entity->dormant( ) )
			continue;

		adjust_player( sound.sound_source );
	}

	soundbuffer = sound_list;
}

void dormancy::adjust_player( const int index ) {
	if ( !elements::get().vis_dormant )
		return;

	auto& player = players[ index ];

	static int duration = 1000;
	bool expired = GetTickCount( ) - player.sounddata.time > duration;

	if ( expired )
		player.set = false;

	if ( !expired )
		player.set = true;

	// dormant check
	player.entity->flags( ) = player.flags;
	player.entity->origin( ) = player.oldpos;
	player.entity->set_position( player.oldpos );
}

void dormancy::restore_sound_dormancy( ) {
	for ( auto& player : players ) {
		if ( player.set )
			player.set = false;
	}
}

bool dormancy::valid_sound( sound_info & sound ) {
	if ( !sound.from_server )
		return false;

	for ( int i = 0; i < soundbuffer.size( ); i++ ) {
		sound_info& cached_sound = soundbuffer[ i ];
		if ( cached_sound.guid == sound.guid )
			return false;
	}
	 
	return true;
}
