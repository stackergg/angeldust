#include "../../../dependencies/common_includes.hpp"
#include "../../hacks/visuals/visuals.hpp"

int __stdcall hooks::post_screen_fx( int unk0 ) {
	static auto original_fn = reinterpret_cast<post_screen_fx_fn>(clientmode_hook->get_original( 44 ));

	visuals::get( ).glow( );

	return original_fn( interfaces::clientmode, unk0 );
}