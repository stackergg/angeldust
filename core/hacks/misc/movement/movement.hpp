#pragma once
#include "../../../../dependencies/common_includes.hpp"
#include "../../../../source-sdk/classes/c_usercmd.hpp"

class movement {

	// sub calls
private:
	void z_strafe( player_t *local );
	void c_strafe( player_t *local );

	// main calls
public:
	void bhop( player_t *local );
	void airstrafe( player_t *local );
}; extern movement *g_movement;
