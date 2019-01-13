#include "../../../../source-sdk/math/math.hpp"
#include "movement.hpp"

void movement::bhop( player_t *local ) {
	if ( !elements::get( ).misc_bhop )
		return;
	{
		globals::original_cmd->buttons &= globals::original_cmd->buttons & in_jump && !(local->flags( ) & fl_onground) ? ~in_jump : UINT_MAX;
	}
}

void movement::airstrafe( player_t *local ) {
	if ( !elements::get( ).misc_airstrafe )
		return;

	static float circle_yaw = 0.f,
		old_yaw = 0.f;

	if ( !(local->flags( ) & fl_onground && !(globals::original_cmd->buttons & in_jump)) ) {
		float yaw_delta = utilities::math::normalize_angle( globals::original_cmd->view_angles.y - old_yaw );

		circle_yaw = old_yaw = globals::original_cmd->view_angles.y;

		if ( yaw_delta > 0.f )
			globals::original_cmd->side_move = -450.f;
		else if ( yaw_delta < 0.f )
			globals::original_cmd->side_move = 450.f;
	}
}

void movement::c_strafe( player_t *local ) {

}

void movement::z_strafe( player_t *local ) {

}

movement *g_movement; 