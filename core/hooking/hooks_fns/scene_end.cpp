#include "../../../dependencies/common_includes.hpp"
#include "../../hacks/visuals/visuals.hpp"

void __stdcall hooks::scene_end( ) {
	reinterpret_cast<scene_end_fn>(renderview_hook->get_original( 9 ))(interfaces::render_view);
	visuals::get( ).chams( );
}