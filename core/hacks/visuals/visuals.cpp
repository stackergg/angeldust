#include "visuals.hpp"
#include "../misc/dormancy/dormancy.hpp"

#define M_PI 3.1415926535897

inline void filled_circle( vec2_t position, int points, float radius, color col ) {
	std::vector<vertex_t> vertices;
	float step = ( float )M_PI * 2.f / ( float )points;

	for ( float a = 0.f; a < ( M_PI * 2.f ); a += step )
		vertices.push_back( vertex_t( vec2_t( radius * cosf( a ) + position.x, radius * sinf( a ) + position.y ) ) );

	render::get( ).add_textured_polygon( points, vertices.data( ), col.r, col.g, col.b, col.a );
}

void visuals::think( ) {
	auto local = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( interfaces::engine->get_local_player( ) ) );

	if ( !elements::get( ).vis_enabled )
		return;

	if ( !local )
		return;

	// hey guys. float division is slower than float multiplication. pls if u are dividing floats anywhere else change it. ~ chance
	// ALSO WHY THE FUCK ARE YOU DIVIDING HERE THE QUOTIENT WILL ALWAYS BE 1.11111111111 
//	const float arrow_fade = ( 1 / 0.9f ) * interfaces::globals->frame_time;
	// using float here for performance.
	const float arrow_fade = 1.11111111111f * interfaces::globals->frame_time;

	meme += ( meme_direction ? arrow_fade : -arrow_fade );

	if ( meme > 1.f ) {
		meme_direction = false;
		meme = 1.f;
	}
	else if ( meme <= 0.f ) {
		meme_direction = true;
		meme = 0.f;
	}

	for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
		auto player = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( i ) );

		if ( !player )
			continue;

		if ( player == local )
			continue;

		if ( player->health( ) <= 0 )
			continue;

		if ( player->team( ) == local->team( ) )
			continue;

		// another case of unnecessary float divison. gotta watch out for that guys! ~chance
//		const int fade = ( int )( ( ( 1 / 0.15f ) * interfaces::globals->frame_time ) * 255 );
		const int fade = ( int )( ( 6.66666666667f * interfaces::globals->frame_time ) * 255 );

		int new_alpha = alpha[ i ];
		new_alpha += player->dormant( ) ? -fade : fade;

		if ( new_alpha > ( player->has_gun_game_immunity( ) ? 130 : 210 ) )
			new_alpha = ( player->has_gun_game_immunity( ) ? 130 : 210 );
		if ( new_alpha < elements::get( ).vis_dormant ? 50 : 0 )
			new_alpha = elements::get( ).vis_dormant ? 50 : 0;

		alpha[ i ] = new_alpha;

		arrows( player );
		player_rendering( player );

		if ( elements::get( ).vis_radar )
			player->spotted( ) = true;

		last_dormant[ i ] = player->dormant( );
	}

}

void visuals::rotate_arrow( std::array< vec2_t, 3 >& points, float rotation )
{
	const auto points_center = ( points.at( 0 ) + points.at( 1 ) + points.at( 2 ) ) / 3;
	for ( auto& point : points ) {
		point -= points_center;

		const auto temp_x = point.x;
		const auto temp_y = point.y;

		const auto theta = deg_to_rad( rotation );
		const auto c = cos( theta );
		const auto s = sin( theta );

		point.x = temp_x * c - temp_y * s;
		point.y = temp_x * s + temp_y * c;

		point += points_center;
	}
}

void visuals::arrows( player_t* player ) {
	auto local = reinterpret_cast< entity_t* >( interfaces::entity_list->get_client_entity( interfaces::engine->get_local_player( ) ) );

	// another case of unnecessary float divison. gotta watch out for that guys! ~chance
//	const int fade = ( int )( ( ( 1 / 0.25f ) * interfaces::globals->frame_time ) * 255 );
	const int fade = ( int )( ( 4 * interfaces::globals->frame_time ) * 255 );

	int new_alpha = arrow_alpha[ player->index( ) ];
	new_alpha += player->dormant( ) ? -fade : fade;
	new_alpha = utilities::math::clamp_value< int >( new_alpha, 0, 200 );
	arrow_alpha[ player->index( ) ] = new_alpha;

	if ( !elements::get( ).vis_arrows )
		return;

	if ( !local )
		return;

	if ( player->dormant( ) && new_alpha == 0 )
		return;

	vec3_t position;
	if ( render::get( ).on_screen( player->get_hitbox( 2 ), position ) )
		return;

	vec3_t viewangles;
	int width, height;

	viewangles = interfaces::engine->get_view_angles( );
	interfaces::engine->get_screen_size( width, height );

	// more float division, i'm just gonna stop putting comments next to it at this point xD. ~chance
//	const auto screen_center = vec2_t( width / 2.f, height / 2.f );
	const auto screen_center = vec2_t( width * .5f, height * .5f );

	const auto angle_yaw_rad = deg_to_rad( viewangles.y - math_temp->calculate_angle( local->eye_position( ), player->get_hitbox( 2 ) ).y - 90 );

	int radius = max( 10, elements::get( ).vis_arrows_dist );
	int size = max( 5, elements::get( ).vis_arrows_px );

	const auto new_point_x = screen_center.x + ( ( ( ( width - ( size * 3 ) ) * .5f ) * ( radius / 100.0f ) ) * cos( angle_yaw_rad ) ) + ( int )( 6.0f * ( ( ( float )size - 4.f ) / 16.0f ) );
	const auto new_point_y = screen_center.y + ( ( ( ( height - ( size * 3 ) ) * .5f ) * ( radius / 100.0f ) ) * sin( angle_yaw_rad ) );

	color clr( elements::get( ).clr_arrows.r, elements::get( ).clr_arrows.g, elements::get( ).clr_arrows.b, ( float )new_alpha * meme );

	std::array< vec2_t, 3 >points{ vec2_t( new_point_x - size, new_point_y - size ),
		vec2_t( new_point_x + size, new_point_y ),
		vec2_t( new_point_x - size, new_point_y + size ) };

	rotate_arrow( points, viewangles.y - math_temp->calculate_angle( local->eye_position( ), player->get_hitbox( 2 ) ).y - 90 );
	render::get( ).draw_filled_triangle( points, clr );
}

void visuals::player_rendering( player_t* player ) {
	if ( ( player->dormant( ) && alpha[ player->index( ) ] == 0 ) and !elements::get( ).vis_dormant )
		return;

	player_info_t info;
	interfaces::engine->get_player_info( player->index( ), &info );

	box bbox;
	if ( !get_playerbox( player, bbox ) )
		return;

	if ( elements::get( ).vis_box ) {
		render::get( ).draw_outline( bbox.x, bbox.y, bbox.w, bbox.h, color( 0, 0, 0, 25 + alpha[ player->index( ) ] ) );
		render::get( ).draw_rect( bbox.x, bbox.y, bbox.w, bbox.h, color(
			elements::get( ).clr_box.r,
			elements::get( ).clr_box.g,
			elements::get( ).clr_box.b,
			alpha[ player->index( ) ] ) );
	}
	if ( elements::get( ).vis_health ) {
		box temp( bbox.x - 5, bbox.y + ( bbox.h - bbox.h * ( utilities::math::clamp_value<int>( player->health( ), 0, 100.f ) / 100.f ) ), 1, bbox.h * ( utilities::math::clamp_value<int>( player->health( ), 0, 100 ) / 100.f ) - ( player->health( ) >= 100 ? 0 : -1 ) );
		box temp_bg( bbox.x - 5, bbox.y, 1, bbox.h );

		render::get( ).draw_filled_rect( temp_bg.x - 1, temp_bg.y - 1, temp_bg.w + 2, temp_bg.h + 2, color( 0, 0, 0, 25 + alpha[ player->index( ) ] ) );
		render::get( ).draw_filled_rect( temp.x, temp.y, temp.w, temp.h, color( elements::get( ).clr_health.r, elements::get( ).clr_health.g, elements::get( ).clr_health.b, alpha[ player->index( ) ] ) );
	}
	if ( elements::get( ).vis_name ) {

		std::string print( info.fakeplayer ? std::string( "bot " ).append( info.name ).c_str( ) : info.name );
		std::transform( print.begin( ), print.end( ), print.begin( ), ::tolower );

		render::get( ).draw_text( bbox.x + ( bbox.w / 2 ), bbox.y - 13, render::get( ).name_font, print, true, color(
			elements::get( ).clr_name.r,
			elements::get( ).clr_name.g,
			elements::get( ).clr_name.b,
			alpha[ player->index( ) ] ) );
	}
	if ( elements::get( ).vis_flags ) {
		std::vector<std::pair<std::string, color>> flags;

		if ( player->has_helmet( ) && player->armor( ) > 0 )
			flags.push_back( std::pair<std::string, color>( "hk", color( 255, 255, 255, alpha[ player->index( ) ] ) ) );
		else if ( !player->has_helmet( ) && player->armor( ) > 0 )
			flags.push_back( std::pair<std::string, color>( "k", color( 255, 255, 255, alpha[ player->index( ) ] ) ) );

		if ( player->money( ) )
			flags.push_back( std::pair<std::string, color>( std::string( "$" ).append( std::to_string( player->money( ) ) ), color( 120, 180, 10, alpha[ player->index( ) ] ) ) );

		if ( player->is_scoped( ) )
			flags.push_back( std::pair<std::string, color>( std::string( "zoom" ), color( 80, 160, 200, alpha[ player->index( ) ] ) ) );

		int pos = 0;
		for ( auto text : flags ) {
			render::get( ).draw_text( bbox.x + bbox.w + 3, bbox.y + pos - 2, render::get( ).main_font, text.first, false, text.second );
			pos += 10;
		}
	}
	if ( elements::get( ).vis_ammo ) {
		weapon_t* weapon = ( weapon_t* )player->active_weapon( );

		if ( weapon != nullptr ) {

			int clip1 = weapon->clip1_count( );
			int max_clip = weapon->get_weapon_data( )->max_clip;

			box temp( bbox.x, bbox.y + bbox.h + 3, ( ( bbox.w * clip1 ) / max_clip ), 1 );
			box temp_bg( bbox.x - 1, bbox.y + bbox.h + 2, bbox.w + 2, 3 );

			render::get( ).draw_filled_rect( temp_bg.x, temp_bg.y, temp_bg.w, temp_bg.h, color( 0, 0, 0, 25 + alpha[ player->index( ) ] ) );
			render::get( ).draw_filled_rect( temp.x, temp.y, temp.w, temp.h, color(
				elements::get( ).clr_ammo.r,
				elements::get( ).clr_ammo.g,
				elements::get( ).clr_ammo.b,
				alpha[ player->index( ) ] ) );
		}

	}
	if ( elements::get( ).vis_weapon ) {
		weapon_t* weapon = ( weapon_t* )player->active_weapon( );

		const char *name = weapon->get_weapon_data( )->hud_name;
		wchar_t *localised_name = interfaces::localize->find( name );

		std::wstring ret( localised_name );
		std::string print( ret.begin( ), ret.end( ) );
		std::transform( print.begin( ), print.end( ), print.begin( ), ::tolower );

		if ( weapon != nullptr ) {
			render::get( ).draw_text( bbox.x + ( bbox.w / 2 ), bbox.h + bbox.y + ( elements::get( ).vis_ammo ? 6 : 2 ), render::get( ).main_font, print, true, color(
				elements::get( ).clr_weapon.r,
				elements::get( ).clr_weapon.g,
				elements::get( ).clr_weapon.b,
				alpha[ player->index( ) ] ) );
		}
	}
}

void visuals::glow( ) {
	if ( !elements::get( ).vis_enabled || !elements::get( ).vis_glow )
		return;

	auto local = reinterpret_cast< entity_t* >( interfaces::entity_list->get_client_entity( interfaces::engine->get_local_player( ) ) );

	for ( size_t i = 0; i < interfaces::glow_manager->size; i++ ) {
		auto &object = interfaces::glow_manager->objects[ i ];

		if ( object.unused( ) )
			continue;

		auto *ent = ( entity_t* )object.entity;

		if ( !ent || ent->dormant( ) || ent->clientclass( )->class_id != class_ids::ccsteam )
			continue;
		if ( local && ent->team( ) == local->team( ) )
			continue;

		object.set( elements::get( ).clr_glow.r / 255.0f, elements::get( ).clr_glow.g / 255.0f, elements::get( ).clr_glow.b / 255.0f, elements::get( ).clr_glow.a / 255.f );
	}
}

void visuals::chams( ) {

	if ( !elements::get( ).vis_enabled || ( !elements::get( ).vis_chams_vis && !elements::get( ).vis_chams_invis ) )
		return;

	auto local = reinterpret_cast< entity_t* >( interfaces::entity_list->get_client_entity( interfaces::engine->get_local_player( ) ) );

	for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
		auto player = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( i ) );

		if ( !player )
			continue;
		if ( local && player == local )
			continue;
		if ( player->health( ) <= 0 )
			continue;
		if ( player->team( ) == local->team( ) )
			continue;

		auto *mat = elements::get( ).vis_chams_type == 0 ? material : lit;

		if ( elements::get( ).vis_chams_invis ) {
			float col[ 3 ] = {
				elements::get( ).clr_chams_invis.r / 255.0f,
				elements::get( ).clr_chams_invis.g / 255.0f,
				elements::get( ).clr_chams_invis.b / 255.0f
			};

			interfaces::render_view->modulate_color( col );
			mat->set_flag( material_var_ignorez, true );

			interfaces::model_render->override_material( mat );
			player->draw_model( 1, 255 );
		}
		if ( elements::get( ).vis_chams_vis ) {
			float col[ 3 ] = {
				elements::get( ).clr_chams_vis.r / 255.0f,
				elements::get( ).clr_chams_vis.g / 255.0f,
				elements::get( ).clr_chams_vis.b / 255.0f
			};

			interfaces::render_view->modulate_color( col );
			mat->set_flag( material_var_ignorez, false );

			interfaces::model_render->override_material( mat );
			player->draw_model( 1, 255 );
		}

		interfaces::model_render->override_material( nullptr );
	}
}

void visuals::entities( ) {

}

void visuals::inaccuracy( ) {
	if ( !elements::get( ).vis_enabled || ( !elements::get( ).vis_visualize_inaccuracy && !elements::get( ).vis_visualize_recoil ) )
		return;

	auto local = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( interfaces::engine->get_local_player( ) ) );

	if ( !local )
		return;

	auto weapon = local->active_weapon( );

	if ( !weapon )
		return;

	if ( elements::get( ).vis_visualize_inaccuracy ) {
		auto weapon = local->active_weapon( );

		int width, height;
		interfaces::engine->get_screen_size( width, height );

		double spread_distance = ( ( weapon->get_innacuracy( ) + weapon->get_spread( ) ) * 320 ) / std::tan( deg_to_rad( elements::get( ).misc_fov_override ) * 0.5 );
		double spread_size = ( height * 0.002083 ) * spread_distance;

		filled_circle( vec2_t( width / 2, height / 2 ), 128, spread_size, color( 255, 0, 0, 255 ) );
	}
}

void visuals::crosshair( ) {
	if ( !elements::get( ).vis_enabled || !elements::get( ).vis_force_crosshair )
		return;

	auto local = reinterpret_cast< entity_t* >( interfaces::entity_list->get_client_entity( interfaces::engine->get_local_player( ) ) );
}

void visuals::grenade_trajectory( ) {
	if ( !elements::get( ).vis_enabled || !elements::get( ).vis_grenade_traj )
		return;

	auto local = reinterpret_cast< entity_t* >( interfaces::entity_list->get_client_entity( interfaces::engine->get_local_player( ) ) );
}

void visuals::spectators( ) {
	if ( !elements::get( ).vis_enabled || !elements::get( ).vis_spectators )
		return;

	auto local = reinterpret_cast< entity_t* >( interfaces::entity_list->get_client_entity( interfaces::engine->get_local_player( ) ) );
}
