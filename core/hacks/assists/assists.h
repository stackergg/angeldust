#pragma once

#include "../../../dependencies/common_includes.hpp"
#include "../rage/main/ragebot_main.h"
#include "../legit/main/legit_main.h"
#include "../misc/main/misc_main.h"

class c_assists {
	// sub calls
private:

	// main calls
public:
	void assists( player_t *local );
}; extern c_assists *g_assists;