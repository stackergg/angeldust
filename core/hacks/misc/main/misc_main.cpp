#include "misc_main.h"

void c_misc_main::miscellaneous_main( player_t *local ) {

	if ( &elements::get( ).misc_fakelag_type > 0 ) {
	//	g_fakelag->fake_lag( local );
	}
	
	if ( &elements::get( ).misc_bhop ) {
		g_movement->bhop( local );
	}

	if ( &elements::get( ).misc_airstrafe ) {
		g_movement->airstrafe(  local );
	}

} c_misc_main *g_misc_main;