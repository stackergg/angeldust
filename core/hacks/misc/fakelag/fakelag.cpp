#include "fakelag.h"

void c_fakelag::fake_lag( player_t *local ) {

	int wish_ticks = 0,
		adaptive_ticks = 2;

	int wish_ticks_1 = (elements::get( ).misc_fakelag_value + .5f) - 1;

	float extrapolated_speed = local->velocity( ).length_2d( ) * interfaces::globals->interval_per_tick;

	std::random_device random;
	std::mt19937 generator( random( ) );
	std::uniform_int_distribution random_lag( wish_ticks, wish_ticks_1 );

	switch ( elements::get( ).misc_fakelag_type ) {
	case 0: // off
		break;

	case 1: // factor
		wish_ticks = wish_ticks_1;
		break;

	case 2: // switch
		if ( globals::original_cmd->tick_count % 30 < 15 )
			wish_ticks = wish_ticks_1;
		break;

	case 3: // adaptive
		while ( (wish_ticks * extrapolated_speed) <= 68.f ) {
			if ( ((adaptive_ticks - 1) * extrapolated_speed) > 68.f ) {
				++wish_ticks;
				break;
			}
			if ( (adaptive_ticks * extrapolated_speed) > 68.f ) {
				wish_ticks += 2;
				break;
			}
			if ( ((adaptive_ticks + 1) * extrapolated_speed) > 68.f ) {
				wish_ticks += 3;
				break;
			}
			if ( ((adaptive_ticks + 2) * extrapolated_speed) > 68.f ) {
				wish_ticks += 4;
				break;
			}

			adaptive_ticks += 5;
			wish_ticks += 5;

			if ( adaptive_ticks > 16 )
				break;
		}
		break;

	case 4: // random
		wish_ticks = random_lag( generator );
		break;

	}
} c_fakelag *g_fakelag;