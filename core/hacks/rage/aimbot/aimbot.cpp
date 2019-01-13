#include "aimbot.h"

// simple aimbot for now to test. todo: mp, htscan, etc
void c_aimbot::aim( player_t *entity, c_usercmd *cmd, player_t *local ) {

	if ( !entity || entity->health( ) <= 0 
		|| entity->dormant( ) 
		|| entity->has_gun_game_immunity( ) )
		return;

}