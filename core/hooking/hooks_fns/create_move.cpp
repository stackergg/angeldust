#include "../../../dependencies/common_includes.hpp"
#include "../../hacks/assists/assists.h"
#include "../../hacks/misc/movement/movement.hpp"

bool __stdcall hooks::create_move( float frame_time, c_usercmd* user_cmd ) {
	static auto original_fn = reinterpret_cast<create_move_fn>(clientmode_hook->get_original( 24 ))(interfaces::clientmode, frame_time, user_cmd);
	if ( !user_cmd || !user_cmd->command_number )
		return original_fn;

	uintptr_t* pebp;
	__asm mov pebp, ebp;
	auto send_packet = ( bool* ) ( *pebp - 0x1C );

	globals::send_packet = true;

	// align our global vars so they don't return nullptr
	globals::original_cmd = user_cmd;
	*send_packet = globals::send_packet;

	player_t *local = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity( interfaces::engine->get_local_player( ) ));

	if ( local->life_state( ) || local->movetype( ) == movetype_custom ) {
		local->client_side_anim( ) = true;
	}
	else {

		const float old_cur = interfaces::globals->cur_time;
		const float old_frm = interfaces::globals->frame_time;
		const float interval_per_tick = interfaces::globals->interval_per_tick;
		const float tickbase = local->tick_base( );

		interfaces::globals->frame_time = interval_per_tick;
		interfaces::globals->cur_time = tickbase * interval_per_tick;

		g_assists->assists( local );

		interfaces::globals->frame_time = old_frm;
		interfaces::globals->cur_time = old_cur;

//		std::memcpy( &GlobalVars->m_nUsercmd, user_cmd, sizeof( GlobalVars->m_nUsercmd ) );
	}
	return false;
}
