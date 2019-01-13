#include "../../../dependencies/common_includes.hpp"

void __stdcall hooks::frame_stage_notify( int frame_stage ) {
	reinterpret_cast<frame_stage_notify_fn>(client_hook->get_original( 37 ))(interfaces::client, frame_stage);

	switch ( frame_stage ) {



	}
}

