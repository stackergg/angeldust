#include "menu.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#define EMPTY_LABEL std::string(1, (char) 255).c_str()

IDirect3DStateBlock9 *state_block;
ImFont* Main;
ImFont* MainCaps;

namespace ImGui {
	bool vector_getter(void *vec_ptr, int index, const char **out_text) {
		auto &vector = *(std::vector<std::string>*) vec_ptr;

		if (index < 0 || index >= vector.size())
			return false;

		*out_text = vector[index].c_str();
		return true;
	}

	bool ColorPicker(const char *label, color *col) {
		ImGui::SameLine();
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - 32.0f - ImGui::GetStyle().FramePadding.x * 2.0f, ImGui::GetCursorPosY() + 1));

		float out[4] = { (float)col->r / 255.f, (float)col->g / 255.f , (float)col->b / 255.f, (float)col->a / 255.f };
		bool ret = ColorEdit4(label, out, ImGuiColorEditFlags_NoInputs);

		if (ret) {
			col->r = (int)(out[0] * 255.f);
			col->g = (int)(out[1] * 255.f);
			col->b = (int)(out[2] * 255.f);
			col->a = (int)(out[3] * 255.f);
		}

		return ret;
	}
	bool ListBox(const char *label, int *current_index, std::vector<std::string> &values, int height = -1) {
		if (values.empty())
			return false;

		return ListBox(label, current_index, vector_getter, (void *)(&values), values.size(), height);
	}
}

void legit_tab() {
	ImGui::Columns( 2, NULL, false );

	ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
	ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );

	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );

	ImGui::BeginChild( "main 1", ImVec2( 269, 201 ), true );
	{
		ImGui::Checkbox( "Active", &elements::get( ).lgt_enabled );



	}
	ImGui::EndChild( true );

	ImGui::PopStyleVar( );
	ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
	ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );
	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );

	ImGui::BeginChild( "main 2", ImVec2( 269, 152 ), true );
	{
	}
	ImGui::EndChild( true );

	ImGui::NextColumn( );

	ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );

	ImGui::BeginChild( "main 3", ImVec2( 289, 152 ), true );
	{
	}
	ImGui::EndChild( true );

	ImGui::Spacing( );

	ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );
	ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 7 );

	ImGui::BeginChild( "main 4", ImVec2( 289, 201 ), true );
	{
	}
	ImGui::EndChild( true );

	ImGui::PopStyleVar( );
	ImGui::Columns( );
}

void rage_tab() {
	ImGui::Columns( 2, NULL, false );

	ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
	ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );

	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );

	ImGui::BeginChild( "main 1", ImVec2( 269, 201 ), true );
	{
		ImGui::Checkbox( "Active", &elements::get( ).rge_enabled );



	}
	ImGui::EndChild( true );

	ImGui::PopStyleVar( );
	ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
	ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );
	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );

	ImGui::BeginChild( "main 2", ImVec2( 269, 152 ), true );
	{

		ImGui::Combo( "Pitch", &elements::get( ).rge_aa_pitch, "Off\0Down\0Up" );
		ImGui::Combo( "Yaw", &elements::get( ).rge_aa_yaw, "Off\0Back" );


	}
	ImGui::EndChild( true );

	ImGui::NextColumn( );

	ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );

	ImGui::BeginChild( "main 3", ImVec2( 289, 152 ), true );
	{
	}
	ImGui::EndChild( true );

	ImGui::Spacing( );

	ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );
	ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 7 );

	ImGui::BeginChild( "main 4", ImVec2( 289, 201 ), true );
	{
	}
	ImGui::EndChild( true );

	ImGui::PopStyleVar( );
	ImGui::Columns( );
}

void visuals_tab() {
	ImGui::Columns(2, NULL, false);

	ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
	ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

	ImGui::BeginChild("Player ESP", ImVec2(269, 201), true);
	{
		ImGui::Checkbox("Active", &elements::get().vis_enabled);

		ImGui::Checkbox("Bounding box", &elements::get().vis_box);
		ImGui::ColorPicker("##bounding_box", &elements::get().clr_box);

		ImGui::Checkbox("Health bar", &elements::get().vis_health);
		ImGui::ColorPicker("##health_bar", &elements::get().clr_health);

		ImGui::Checkbox("Name", &elements::get().vis_name);
		ImGui::ColorPicker("##name", &elements::get().clr_name);

		ImGui::Checkbox("Flags", &elements::get().vis_flags);
		ImGui::Combo("Weapons", &elements::get().vis_weapon, "Off\0Icon\0Text");
		ImGui::ColorPicker( "##clr_weapon", &elements::get( ).clr_weapon );

		ImGui::Checkbox("Ammo", &elements::get().vis_ammo);
		ImGui::ColorPicker("##ammo", &elements::get().clr_ammo);

		ImGui::Checkbox("Glow", &elements::get().vis_glow);
		ImGui::ColorPicker("##glow", &elements::get().clr_glow);

		ImGui::Checkbox("Out of FOV arrows", &elements::get().vis_arrows);
		ImGui::ColorPicker("#out_of_fov_arrows", &elements::get().clr_arrows);

		if (elements::get().vis_arrows) {
			ImGui::SliderInt("Arrow size", &elements::get().vis_arrows_px, 5, 20, "%.fpx");
			ImGui::SliderInt("Arrow distance", &elements::get().vis_arrows_dist, 0, 100, "%.f%%");
		}
	}
	ImGui::EndChild(true);

	ImGui::PopStyleVar();
	ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
	ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

	ImGui::BeginChild("Colored models", ImVec2(269, 152), true);
	{
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::CalcTextSize("Models type").y + 4.0f);
		ImGui::Combo("##models_type", &elements::get().vis_chams_type, "Default\0Solid\0Metallic");

		ImGui::Checkbox("Player", &elements::get().vis_chams_vis);
		ImGui::ColorPicker("##player", &elements::get().clr_chams_vis);

		ImGui::Checkbox("Player behind wall", &elements::get().vis_chams_invis);
		ImGui::ColorPicker("##player_behind_wall", &elements::get().clr_chams_invis);

		ImGui::Checkbox("Reflectivity", &elements::get().vis_chams_reflectivity);
		ImGui::ColorPicker("##reflectivity", &elements::get().clr_chams_reflectivity);

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::CalcTextSize("Reflectivity").y);
		ImGui::SliderInt("##reflectivity_value", &elements::get().vis_chams_reflectivity_val, 0, 100, EMPTY_LABEL);

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::CalcTextSize("Rim").y + 3.0f);
		ImGui::SliderInt("Rim", &elements::get().vis_chams_rim_val, 0, 100, EMPTY_LABEL);
	}
	ImGui::EndChild(true);

	ImGui::NextColumn();

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 9);

	ImGui::BeginChild("Other ESP", ImVec2(289, 152), true);
	{
		ImGui::Checkbox("Radar", &elements::get().vis_radar);

		float old_x = ImGui::GetCursorPosX();
		float old_y = ImGui::GetCursorPosY();

		ImGui::SetCursorPosX(old_x + 18.0f);
		ImGui::SetCursorPosY(old_y - 3.0f);
		ImGui::Text("Dropped weapons");

		ImGui::SetCursorPosX(old_x);
		ImGui::ColorPicker("##dropped_wpns_clr", &elements::get().clr_dropped_weapons);

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 16.0f);
		ImGui::Combo("##dropped_wpns", &elements::get().vis_dropped_weapons, "Off\0Text\0Glow\0Icon");

		ImGui::Checkbox("Grenades", &elements::get().vis_grenades);
		ImGui::ColorPicker("##grenades", &elements::get().clr_grenades);

		ImGui::Checkbox("Visualize inaccuracy", &elements::get().vis_visualize_inaccuracy);
		ImGui::ColorPicker("##visualize_inaccuracy", &elements::get().clr_inaccuracy);

		ImGui::Checkbox("Visualize recoil", &elements::get().vis_visualize_recoil);
		ImGui::Checkbox("Force crosshair", &elements::get().vis_force_crosshair);

		ImGui::Checkbox("Bomb", &elements::get().vis_bomb);
		ImGui::ColorPicker("##bomb", &elements::get().clr_bomb);

		ImGui::Checkbox("Grenade trajectory", &elements::get().vis_grenade_traj);
		ImGui::ColorPicker("##grenade_trajectory", &elements::get().clr_grenade_traj);

		ImGui::Checkbox("Spectators", &elements::get().vis_spectators);
	}
	ImGui::EndChild(true);

	ImGui::Spacing();

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 9);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 7);

	ImGui::BeginChild("Effects", ImVec2(289, 201), true);
	{
		ImGui::Checkbox("Remove flashbang effect", &elements::get().vis_flashbang_remove);
		ImGui::Checkbox("Remove smoke effect", &elements::get().vis_smoke_remove);
		ImGui::Checkbox("Remove fog", &elements::get().vis_fog_remove);

		ImGui::Combo("Visual recoil adjustment", &elements::get().vis_recoil_adj, "Off\0Remove shake\0Remove all");

		ImGui::SliderInt("Transparent walls", &elements::get().vis_walls_transparency, 0, 100, "%.f%%");
		ImGui::SliderInt("Transparent props", &elements::get().vis_props_transparency, 0, 100, "%.f%%");

		ImGui::Combo("Brightness adjustment", &elements::get().vis_brightness_adj, "Off\0Fullbright\0Night mode");

		ImGui::Checkbox("Remove scope overlay", &elements::get().vis_remove_scope);
		ImGui::Checkbox("Instant scope", &elements::get().vis_instant_scope);
		ImGui::Checkbox("Disable post processing", &elements::get().vis_disable_post_fx);
		ImGui::Checkbox("Force third person (alive)", &elements::get().vis_third_person_alive);
		ImGui::Checkbox("Force third person (dead)", &elements::get().vis_third_person_dead);
		ImGui::Checkbox("Disable rendering of teammates", &elements::get().vis_disable_teammates_render);

		ImGui::Checkbox("Bullet tracers", &elements::get().vis_bullet_tracers);
		ImGui::ColorPicker("##bullet_tracers", &elements::get().clr_bullet_tracers);

		ImGui::Checkbox("Bullet impacts", &elements::get().vis_bullet_impacts);
	}
	ImGui::EndChild(true);

	ImGui::PopStyleVar();
	ImGui::Columns();
}

void misc_tab() {
	ImGui::Columns(2, NULL, false);

	ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
	ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

	ImGui::BeginChild("main 1", ImVec2(269, 361), true);
	{
		ImGui::Checkbox( "Active", &elements::get( ).msc_enabled );
	}
	ImGui::EndChild(true);
	ImGui::NextColumn();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 9);

	ImGui::BeginChild("main 2", ImVec2(289, 152), true);
	{
		ImGui::Checkbox( "Bunny Hop", &elements::get( ).misc_bhop );
		ImGui::Checkbox( "Airstrafe", &elements::get( ).misc_airstrafe );
	}
	ImGui::EndChild(true);

	ImGui::Spacing();

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 9);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 7);

	ImGui::BeginChild("main 3", ImVec2(289, 201), true);
	{
		ImGui::Combo( "Fakelag Type", &elements::get( ).misc_fakelag_type, "Off\0Factor\0Switch\0Adaptive\0Random" );
		// @chance, u dont need to do "%.i" for the simple fact its already a sliderint and uh it won't display a float value (cuz default format value
		// is %0.f anyways)
		ImGui::SliderInt( "Fakelag Value", &elements::get( ).misc_fakelag_value, 0, 14 );
	}
	ImGui::EndChild(true);

	ImGui::PopStyleVar();
	ImGui::Columns();
}

void config_tab() {

}

void menu::think() {
	ImGui::GetIO().MouseDrawCursor = opened;

	static int page = 0;

	if (opened) {

		ImGui::GetStyle().Colors[ImGuiCol_CheckMark] = ImVec4(elements::get().clr_menu.r / 255.f, elements::get().clr_menu.g / 255.f, elements::get().clr_menu.b / 255.f, 1.f);
		ImGui::GetStyle().Colors[ImGuiCol_SliderGrab] = ImVec4(elements::get().clr_menu.r / 255.f, elements::get().clr_menu.g / 255.f, elements::get().clr_menu.b / 255.f, 1.f);
		ImGui::GetStyle().Colors[ImGuiCol_SliderGrabActive] = ImVec4(elements::get().clr_menu.r / 255.f, elements::get().clr_menu.g / 255.f, elements::get().clr_menu.b / 255.f, 1.f);

		ImGui::SetNextWindowSize(ImVec2(600, 455), ImGuiSetCond_FirstUseEver);

		ImGui::Begin("angeldust", &opened, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar); {
			ImVec2 p = ImGui::GetCursorScreenPos();
			ImColor c = ImColor(elements::get().clr_menu.r, elements::get().clr_menu.g, elements::get().clr_menu.b);

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y + 3), ImVec2(p.x + ImGui::GetWindowWidth(), p.y + -3), ImColor(elements::get().clr_menu.r, elements::get().clr_menu.g, elements::get().clr_menu.b));
			ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(p.x, p.y + 3), ImVec2(p.x + ImGui::GetWindowWidth() / 2, p.y + -3), ImColor(0, 0, 0, 125), ImColor(0, 0, 0, 15), ImColor(0, 0, 0, 15), ImColor(0, 0, 0, 125));
			ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(p.x + ImGui::GetWindowWidth() / 2, p.y + 3), ImVec2(p.x + ImGui::GetWindowWidth(), p.y + -3), ImColor(0, 0, 0, 15), ImColor(0, 0, 0, 125), ImColor(0, 0, 0, 125), ImColor(0, 0, 0, 15));
			ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x, p.y + 2), ImVec2(p.x + ImGui::GetWindowWidth(), p.y + 2), ImColor(0, 0, 0, 150));

			ImGui::PushFont(MainCaps);
			ImGui::Dummy(ImVec2(250, 0)); ImGui::SameLine();
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
			ImGui::TextColored(ImVec4(elements::get().clr_menu.r / 255.f, elements::get().clr_menu.g / 255.f, elements::get().clr_menu.b / 255.f, 1.f), "angeldust");
			ImGui::PopFont();

			ImGui::PushFont(Main);
			ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
			ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();

			ImGui::BeginChild("tabs", ImVec2(568, 30), true, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
			{
				if (ImGui::ButtonT("legit", ImVec2(568 / 5, -1), page, 0, c, false)) page = 0; ImGui::SameLine(0, 0);
				if (ImGui::ButtonT("rage", ImVec2(568 / 5, -1), page, 1, c, false)) page = 1; ImGui::SameLine(0, 0);
				if (ImGui::ButtonT("visual", ImVec2(568 / 5, -1), page, 2, c, false)) page = 2; ImGui::SameLine(0, 0);
				if (ImGui::ButtonT("other", ImVec2(568 / 5, -1), page, 3, c, false)) page = 3; ImGui::SameLine(0, 0);
				if (ImGui::ButtonT("config", ImVec2(568 / 5, -1), page, 4, c, false)) page = 4; ImGui::SameLine(0, 0);
			}
			ImGui::EndChild();
			ImGui::PopFont();

			ImGui::PushFont(Menu);

			switch ( page ) {
			case 0: legit_tab( );
				break;
			case 1: rage_tab( );
				break;
			case 2: visuals_tab( );
				break; 
			case 3: misc_tab( );
				break;
			case 4: config_tab( );
				break;
			}

			ImGui::PopFont();
		}
		ImGui::End();
	}
}
