#pragma once
#include "../../../../dependencies/common_includes.hpp"

class c_antiaim {
	// sub calls
private:
	void pitch( );
	void yaw( );

	// main calls
public:
	void on_think( player_t *local );
};
