#pragma once
#include "../../../../dependencies/common_includes.hpp"

class c_lagcomp {

	// sub calls
private:
	void simulate_movement( player_t *entity );
	void lag_compensation( player_t *entity );
	// main calls
public:
	int start_lag_compensation( );
};

class c_position_adjustment {

	// sub calls
private:
	// main calls
public:
	void store_tickrecord( );
	void restore_tickrecord( );
};

struct simulation_data {
	player_t *m_pEntity;
	vec3_t m_vecVelocity;
	vec3_t m_vecOrigin;
	int m_fFlags;
};

struct tick_record {
	vec3_t m_vecVelocity;
	vec3_t m_vecOrigin;
	int m_fFlags;

	float m_flSimulationTime;

};