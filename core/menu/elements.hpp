#pragma once

#include "../../dependencies/utilities/singleton.hpp"
#include "../../source-sdk/misc/color.hpp"

struct skin_info_t {
	bool enabled;
	bool stat_trak;

	int quality;
	int seed;
	int skin;
};

class elements : public singleton< elements > {
public:
	bool lgt_enabled;
	bool rge_enabled;
	bool knv_enabled;
	bool msc_enabled;

	bool vis_enabled;
	bool vis_dormant;
	bool vis_box;
	bool vis_name;
	bool vis_health;
	bool vis_flags;
	int vis_weapon;
	bool vis_ammo;
	bool vis_distance;
	bool vis_glow;
	int vis_chams_type;
	bool vis_chams_vis;
	bool vis_chams_invis;
	bool vis_chams_reflectivity;
	int vis_chams_reflectivity_val = 70;
	int vis_chams_rim_val = 0;
	bool vis_arrows;
	int vis_arrows_px = 12;
	int vis_arrows_dist = 50;
	bool vis_radar;
	int vis_dropped_weapons;
	bool vis_grenades;
	bool vis_visualize_inaccuracy;
	bool vis_visualize_recoil;
	bool vis_force_crosshair;
	bool vis_bomb;
	bool vis_grenade_traj;
	bool vis_spectators;
	bool vis_flashbang_remove;
	bool vis_smoke_remove;
	bool vis_fog_remove;
	int vis_recoil_adj = 0;
	int vis_walls_transparency = 100;
	int vis_props_transparency = 100;
	int vis_brightness_adj = 0;
	bool vis_remove_scope;
	bool vis_instant_scope;
	bool vis_disable_post_fx;
	bool vis_third_person_alive;
	bool vis_third_person_dead;
	bool vis_disable_teammates_render;
	bool vis_bullet_tracers;
	bool vis_bullet_impacts;

	int rge_aa_pitch;
	int rge_aa_yaw;

	float rge_aa_spin_speed;

	int misc_fov_override = 90;
	int misc_viewmodel_fov_override = 68;
	bool misc_bhop;
	bool misc_airstrafe;
	int misc_fakelag_type;
	int misc_fakelag_value;
	std::unordered_map<int, skin_info_t> misc_skins;

	color clr_arrows = color(255, 255, 255);
	color clr_box = color( 255, 255, 255 );
	color clr_health = color(0, 255, 0);
	color clr_name = color( 255, 255, 255 );
	color clr_weapon = color( 255, 255, 255 );
	color clr_ammo = color( 80, 160, 200 );
	color clr_menu = color(32, 114, 247); // color(255, 150, 40);
	color clr_glow = color(255, 255, 255);
	color clr_chams_vis = color(255, 255, 255);
	color clr_chams_invis = color(255, 255, 255);
	color clr_dropped_weapons = color(255, 255, 255);
	color clr_grenades = color(255, 255, 255);
	color clr_bomb = color(255, 255, 255);
	color clr_grenade_traj = color(255, 255, 255);
	color clr_inaccuracy = color(220, 30, 30);
	color clr_chams_reflectivity = color(255, 178, 0);
	color clr_bullet_tracers = color(255, 0, 40);
};