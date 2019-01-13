#include "../../../dependencies/common_includes.hpp"
#include "../../hacks/visuals/visuals.hpp"

void __stdcall hooks::paint_traverse( unsigned int panel, bool force_repaint, bool allow_force ) {
	std::string panel_name = interfaces::panel->get_panel_name( panel );

	reinterpret_cast<paint_traverse_fn>(panel_hook->get_original( 41 ))(interfaces::panel, panel, force_repaint, allow_force);


	static unsigned int _panel = 0;
	static bool once = false;

	if ( !once ) {
		PCHAR panel_char = (PCHAR)interfaces::panel->get_panel_name( panel );
		if ( strstr( panel_char, "MatSystemTopPanel" ) ) {
			_panel = panel;
			once = true;
		}
		render::get( ).setup_fonts( );
	}
	else if ( _panel == panel ) {
		visuals::get( ).think( );
		visuals::get( ).entities( );
		visuals::get( ).grenade_trajectory( );
		visuals::get( ).crosshair( );
		visuals::get( ).spectators( );
	}
}