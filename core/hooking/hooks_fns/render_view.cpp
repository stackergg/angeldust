#include "../../../dependencies/common_includes.hpp"

void __stdcall hooks::render_view( view_setup_t &view, view_setup_t &hud_setup, int clear_flags, int what_to_draw ) {
	auto local = reinterpret_cast<entity_t*>(interfaces::entity_list->get_client_entity( interfaces::engine->get_local_player( ) ));
	static auto original_fn = reinterpret_cast<render_view_fn>(viewrender_hook->get_original( 6 ));

	if ( local && !local->scoped( ) ) {
		const float backup_dbg_fov = interfaces::console->get_convar( "fov_cs_debug" )->float_value,
			backup_viewmodel_fov = interfaces::console->get_convar( "viewmodel_fov" )->float_value;

		view.fov = elements::get( ).misc_fov_override == 0 ? backup_dbg_fov : elements::get( ).misc_fov_override;
		view.viewmodel_fov = elements::get( ).misc_viewmodel_fov_override == 0 ? backup_viewmodel_fov : elements::get( ).misc_viewmodel_fov_override;
	}

	original_fn( view_render_ptr, view, hud_setup, clear_flags, what_to_draw );
}