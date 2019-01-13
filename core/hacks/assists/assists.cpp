#include "assists.h"

void c_assists::assists( player_t *local ) {

	if ( local && local->health( ) > 0 && local->active_weapon( ) ) {

		// some placeholder shit, don't remove this for now.
		if ( &elements::get( ).rge_enabled && &elements::get( ).lgt_enabled ) {
			elements::get( ).rge_enabled = false;
		}

		if ( &elements::get( ).rge_enabled ) {
			g_ragebot_main->ragebot_main( );
		}
		else if ( &elements::get( ).lgt_enabled ) {
			g_legitbot_main->legitbot_main( );
		}

		if ( &elements::get( ).knv_enabled ) {
			g_legitbot_main->legitbot_main( );
		}

		if ( &elements::get( ).msc_enabled ) {
			g_misc_main->miscellaneous_main( local );
		}
	}
} c_assists *g_assists;