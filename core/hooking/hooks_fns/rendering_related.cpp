#include "../../../dependencies/common_includes.hpp"
#include "../../menu/menu.hpp"

struct VERTEX {
	float x, y, z;
	float rhw;
	DWORD color;
};

inline void clircle_filled_dual_colour( float x, float y, float rad, float rotate, int type, int resolution, DWORD color, DWORD color2, IDirect3DDevice9* m_device ) {
	LPDIRECT3DVERTEXBUFFER9 g_pVB2;

	std::vector<VERTEX> circle( resolution + 2 );

	float angle = rotate * D3DX_PI / 180;

	circle[0].x = x;
	circle[0].y = y;
	circle[0].z = 0;
	circle[0].rhw = 1;
	circle[0].color = color2;

	for ( int i = 1; i < resolution + 2; i++ )
	{
		circle[i].x = (float)(x - rad * cos( D3DX_PI*((i - 1) / (resolution / 2.0f)) ));
		circle[i].y = (float)(y - rad * sin( D3DX_PI*((i - 1) / (resolution / 2.0f)) ));
		circle[i].z = 0;
		circle[i].rhw = 1;
		circle[i].color = color;
	}

	int _res = resolution + 2;
	for ( int i = 0; i < _res; i++ )
	{
		circle[i].x = x + cos( angle )*(circle[i].x - x) - sin( angle )*(circle[i].y - y);
		circle[i].y = y + sin( angle )*(circle[i].x - x) + cos( angle )*(circle[i].y - y);
	}

	m_device->CreateVertexBuffer( (resolution + 2) * sizeof( VERTEX ), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB2, NULL );

	VOID* pVertices;
	g_pVB2->Lock( 0, (resolution + 2) * sizeof( VERTEX ), (void**)&pVertices, 0 );
	memcpy( pVertices, &circle[0], (resolution + 2) * sizeof( VERTEX ) );
	g_pVB2->Unlock( );

	m_device->SetTexture( 0, NULL );
	m_device->SetPixelShader( NULL );
	m_device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	m_device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	m_device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	m_device->SetStreamSource( 0, g_pVB2, 0, sizeof( VERTEX ) );
	m_device->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	m_device->DrawPrimitive( D3DPT_TRIANGLEFAN, 0, resolution );

	if ( g_pVB2 != NULL )
		g_pVB2->Release( );
}

static bool initialized = false;

long __stdcall hooks::present( IDirect3DDevice9* device, RECT* source_rect, RECT* dest_rect, HWND dest_window_override, RGNDATA* dirty_region ) {
	if ( !initialized ) {
		menu::get( ).apply_fonts( );
		menu::get( ).setup_resent( device );
		initialized = true;
	}
	if ( initialized ) {
		menu::get( ).pre_render( device );
		menu::get( ).post_render( );

		if ( interfaces::engine->in_game( ) && interfaces::engine->connected( ) ) {
			auto local = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity( interfaces::engine->get_local_player( ) ));

			if ( local && local->health( ) > 0 ) {
				if ( elements::get( ).vis_enabled && elements::get( ).vis_visualize_inaccuracy ) {
					auto weapon = local->active_weapon( );

					int width, height;
					interfaces::engine->get_screen_size( width, height );

					double spread_distance = ((weapon->get_innacuracy( ) + weapon->get_spread( )) * 320) / std::tan( deg_to_rad( elements::get( ).misc_fov_override ) * 0.5 );
					double spread_size = (height * 0.002083) * spread_distance;

					clircle_filled_dual_colour( width / 2, height / 2, spread_size, 0.0f, 1, 100, D3DCOLOR_ARGB( elements::get( ).clr_inaccuracy.a, elements::get( ).clr_inaccuracy.r, elements::get( ).clr_inaccuracy.g, elements::get( ).clr_inaccuracy.b ), D3DCOLOR_ARGB( 10, elements::get( ).clr_inaccuracy.r, elements::get( ).clr_inaccuracy.g, elements::get( ).clr_inaccuracy.b ), device );
				}
			}
		}

		menu::get( ).think( );
		menu::get( ).end_present( device );
	}

	return original_present( device, source_rect, dest_rect, dest_window_override, dirty_region );
}

long __stdcall hooks::reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters ) {
	if ( !initialized )
		original_reset( device, present_parameters );

	menu::get( ).invalidate_objects( );
	long hr = original_reset( device, present_parameters );
	menu::get( ).create_objects( device );

	return hr;
}

void __stdcall hooks::lock_cursor( ) {
	if ( menu::get( ).opened ) {
		interfaces::surface->unlock_cursor( );
		return;
	}

	reinterpret_cast<lock_cursor_fn>(surface_hook->get_original( 67 ))(interfaces::surface);
}

extern LRESULT ImGui_ImplDX9_WndProcHandler( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );

LRESULT __stdcall hooks::wndproc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam ) {
	static bool pressed = false;

	if ( !pressed && GetAsyncKeyState( VK_INSERT ) ) {
		pressed = true;
	}
	else if ( pressed && !GetAsyncKeyState( VK_INSERT ) ) {
		pressed = false;

		menu::get( ).opened = !menu::get( ).opened;
	}

	if ( menu::get( ).opened && ImGui_ImplDX9_WndProcHandler( hwnd, message, wparam, lparam ) )
		return true;

	return CallWindowProcA( wndproc_original, hwnd, message, wparam, lparam );
}