#include "lagcomp.h"

simulation_data *data;
tick_record *records;

void c_position_adjustment::store_tickrecord( ) {

}

void c_position_adjustment::restore_tickrecord( ) {

}

int c_lagcomp::start_lag_compensation( ) {

	return 0;
}

void c_lagcomp::lag_compensation( player_t *entity ) {

	data->m_pEntity = entity;
	entity->get_absolute_origin( );
	data->m_vecVelocity.z = records->m_vecVelocity.z;

	float original_origin = records->m_vecOrigin.z;

	data->m_vecOrigin.z = records->m_vecOrigin.z;
	data->m_fFlags = records->m_fFlags;
	data->m_vecVelocity.x = records->m_vecVelocity.x;
	data->m_vecOrigin.x = records->m_vecOrigin.x;

	INetChannelInfo *nci = interfaces::engine->GetNetChannelInfo( );
	
	float tick_interval = 1.f / interfaces::globals->interval_per_tick,

		inc_latency = nci->GetAvgLatency( 1 ),

		out_latency = nci->GetAvgLatency( 0 );

	float sim_time = records->m_flSimulationTime,

		deltaTime = (((((inc_latency + out_latency) * tick_interval) + .5f) + interfaces::globals->tick_count + 1)
			* interfaces::globals->interval_per_tick)
		- sim_time;

	if ( deltaTime > 1.f )
		deltaTime = 1.f;

	float simtime_delta = sim_time - entity->simulation_time( ),

		simulation_ticks = ((simtime_delta * tick_interval) + .5f);

	if ( simulation_ticks <= 15 ) {
		if ( simulation_ticks < 1 )
			simulation_ticks = 1;
	}
	else {
		simulation_ticks = 15;
	}

	float v31 = std::atan2( records->m_vecVelocity.y, records->m_vecVelocity.x ),

		v15 = (tick_interval * deltaTime) + .5f,

		velocity_degree = v31 * 57.295776f,

		v30 = std::atan2( entity->velocity( ).y, entity->velocity( ).x ),

		velocity_angle = velocity_degree - (v30 * 57.295776f);

	if ( velocity_angle <= 180.f ) {
		if ( velocity_angle < -180.f )
			velocity_angle = velocity_angle + 360.f;
	}
	else {
		velocity_angle = velocity_angle - 360.f;
	}

	int v18 = v15 - simulation_ticks;
	float v19 = velocity_angle / simtime_delta;

	if ( v18 < 0 ) {
		records->m_vecOrigin.z = original_origin;
	}
	else {
		do {
			if ( simulation_ticks > 0 ) {
				int v20 = simulation_ticks;
				do {
					float extrapolated_move = velocity_degree + (interfaces::globals->interval_per_tick * v19),
						v21 = (extrapolated_move * .017453292f);

					data->m_vecVelocity.x = v21 * data->m_vecVelocity.length_2d( );
					data->m_vecVelocity.y = v21 * data->m_vecVelocity.length_2d( );
					simulate_movement( entity );

					velocity_degree = extrapolated_move;
					--v20;
				} while ( v20 );
				v18 -= simulation_ticks;
			}
		} while ( v18 >= 0 );
		records->m_vecOrigin.z = data->m_vecOrigin.z;
	}
}

void c_lagcomp::simulate_movement( player_t *entity ) {

}