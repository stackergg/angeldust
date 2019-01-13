#include "antiaim.h"

// processing function for anti aim system
void c_antiaim::on_think( player_t *local ) {

	// disable if the local is shooting and they can actually fire
	if ( globals::original_cmd->buttons & in_attack && !local->active_weapon()->is_c4( )
		// if were knifing and can attack
		|| globals::original_cmd->buttons & in_attack2 && local->active_weapon( )->is_knife( ) ) return;

	// disable when were in use when we arent planting and we arent defusing
	if ( globals::original_cmd->buttons & in_use && !local->active_weapon( )->is_c4( ) && !local->is_defusing( ) ) return;

	// disable when were on a ladder or no clipping and moving
	if ( (local->movetype( ) == movetype_ladder || local->movetype( ) == movetype_noclip)
		&& globals::original_cmd->buttons & (in_forward | in_back | in_moveleft | in_moveright) ) return;

	if ( &elements::get( ).rge_aa_pitch > 0 ) {
		pitch( );
	}

	if ( &elements::get( ).rge_aa_yaw > 0 ) {
		yaw( );
	}
}

void c_antiaim::pitch( ) {

	switch ( elements::get( ).rge_aa_pitch ) {
	case 0: // off
		break;

	case 1: // down
		globals::original_cmd->view_angles.x = 89.f;
		break;

	case 2: // up
		globals::original_cmd->view_angles.x = -89.f;
		break;
	}
}

void c_antiaim::yaw( ) {

	switch ( elements::get( ).rge_aa_yaw ) {
	case 0: // off
		break;
	
	case 1: // 180
		globals::original_cmd->view_angles.y += 180.f;
		break;
	}
}