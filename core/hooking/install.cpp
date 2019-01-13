#pragma once
#include "../../dependencies/common_includes.hpp"
#include <algorithm>

std::unique_ptr<vmt_hook> hooks::client_hook;
std::unique_ptr<vmt_hook> hooks::clientmode_hook;
std::unique_ptr<vmt_hook> hooks::panel_hook;
std::unique_ptr<vmt_hook> hooks::renderview_hook;
std::unique_ptr<vmt_hook> hooks::viewrender_hook;
std::unique_ptr<vmt_hook> hooks::surface_hook;

void *view_render_ptr;
WNDPROC hooks::wndproc_original = NULL;
HWND hooks::window;
uint8_t* present_address;
hooks::present_fn original_present;
uint8_t* reset_address;
hooks::reset_fn original_reset;

// find a new home for this function
void setup_material( ) {
	std::ofstream( "csgo/materials/angeldust_material.vmt" ) << R"#("VertexLitGeneric" {
            "$basetexture" "vgui/white_additive"
            "$ignorez"      "0"
            "$envmap"       ""
            "$nofog"        "1"
            "$model"        "1"
            "$nocull"       "0"
            "$selfillum"    "1"
            "$halflambert"  "1"
            "$znearer"      "0"
            "$flat"         "0"
			"$phong"		"1"
			"$rimlight"		"1"
        })#";
	std::ofstream( "csgo/materials/angeldust_lit.vmt" ) << R"#("UnlitGeneric" {
			"$basetexture" "vgui/white_additive"
			"$ignorez"      "0"
			"$envmap"       ""
			"$nofog"        "1"
			"$model"        "1"
			"$nocull"       "0"
			"$selfillum"    "1"
			"$halflambert"  "1"
			"$znearer"      "0"
			"$flat"         "1"
			"$phong"		"1"
			"$rimlight"		"1"
		})#"
		;
}

void hooks::initialize( ) {

	setup_material( );
	render::get( ).setup_fonts( );

	client_hook = std::make_unique<vmt_hook>( );
	clientmode_hook = std::make_unique<vmt_hook>( );
	panel_hook = std::make_unique<vmt_hook>( );
	renderview_hook = std::make_unique<vmt_hook>( );
	viewrender_hook = std::make_unique<vmt_hook>( );
	surface_hook = std::make_unique<vmt_hook>( );

	surface_hook->setup( interfaces::surface );
	surface_hook->hook_index( 67, reinterpret_cast< void* >( lock_cursor ) );

	client_hook->setup( interfaces::client );
	client_hook->hook_index( 37, reinterpret_cast< void* >( frame_stage_notify ) );

	clientmode_hook->setup( interfaces::clientmode );
	clientmode_hook->hook_index( 24, reinterpret_cast< void* >( create_move ) );
	clientmode_hook->hook_index(44, reinterpret_cast<void *>(post_screen_fx));

	panel_hook->setup( interfaces::panel );
	panel_hook->hook_index( 41, reinterpret_cast< void* >( paint_traverse ) );

	renderview_hook->setup( interfaces::render_view );
	renderview_hook->hook_index( 9, reinterpret_cast< void* >( scene_end ) );

	view_render_ptr = *(void **)(utilities::pattern_scan(GetModuleHandleA("client_panorama.dll"), "A1 ? ? ? ? B9 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? FF 10") + 1);

	viewrender_hook->setup(view_render_ptr);
	viewrender_hook->hook_index(6, reinterpret_cast<void*>(render_view));

	present_address = utilities::pattern_scan( GetModuleHandleW( L"gameoverlayrenderer.dll" ), "FF 15 ? ? ? ? 8B F8 85 DB" ) + 0x2;
	reset_address = utilities::pattern_scan( GetModuleHandleW( L"gameoverlayrenderer.dll" ), "FF 15 ? ? ? ? 8B F8 85 FF 78 18" ) + 0x2;

	original_present = **reinterpret_cast< present_fn** >( present_address );
	original_reset = **reinterpret_cast< reset_fn** >( reset_address );

	**reinterpret_cast< void*** >( present_address ) = reinterpret_cast< void* >( &present );
	**reinterpret_cast< void*** >( reset_address ) = reinterpret_cast< void* >( &reset );

	// Valve001 = all window class name for valve games, it's ok to use this if you're a nigger, but otherwise don't do this. ~chance
	window = FindWindow( "Valve001", NULL );

	wndproc_original = ( WNDPROC )SetWindowLongPtrA( window, GWL_WNDPROC, ( LONG )wndproc );

	interfaces::console->get_convar( "mat_queue_mode" )->set_value( 0 );
	interfaces::console->get_convar( "viewmodel_fov" )->callbacks.SetSize( 0 );
	interfaces::console->get_convar( "mat_postprocess_enable" )->set_value( 0 );
	interfaces::console->get_convar( "crosshair" )->set_value( 1 );
}

void hooks::shutdown( ) {
	clientmode_hook->release( );
	client_hook->release( );
	panel_hook->release( );
	renderview_hook->release( );
	viewrender_hook->release();

	**reinterpret_cast< void*** >( present_address ) = reinterpret_cast< void* >( original_present );
	**reinterpret_cast< void*** >( reset_address ) = reinterpret_cast< void* >( original_reset );

	SetWindowLongPtrA( window, GWL_WNDPROC, ( LONG )wndproc_original );
} 