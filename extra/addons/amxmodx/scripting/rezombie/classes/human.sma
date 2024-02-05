#include <amxmodx>
#include <rezombie>

new Weapon:HumanKnife
new Flashlight:HumanFlashlight
new NightVision:ZombieNightVision

public plugin_precache() {
	register_plugin("Class: Human", "1.0.0", "fl0wer")

	new Class:class = create_class("human", TEAM_HUMAN)
	set_class_var(class, "name", "RZ_HUMAN")
	set_class_var(class, "hud_color", "blue")
	set_class_var(class, "forward_give_default_items", "@humanGiveDefaultItems")

	new Props:props = get_class_var(class, "props")
	set_props_var(props, "health", 1000)
	//set_props_var(props, "gravity", 0.5)

	new ModelsPack:models = get_class_var(class, "models")
	models_pack_add_model(models, create_model("rezombie/players/human/elite_crew/1.mdl"))
	models_pack_add_model(models, create_model("rezombie/players/human/phoenix/1.mdl"))
	models_pack_add_model(models, create_model("rezombie/players/human/separatist/1.mdl"))

	new Weapon:melee = get_class_var(class, "melee")
	set_weapon_var(melee, "view_model", create_model("rezombie/weapons/knife/v_knife.mdl"))
	set_weapon_var(melee, "player_model", create_model("rezombie/weapons/knife/p_knife.mdl"))
	set_weapon_var(melee, "world_model", create_model("rezombie/weapons/knife/w_knife.mdl"))

	add_translate("classes/human")

	HumanFlashlight = create_flashlight("yellow", 5, 120, 20)
	ZombieNightVision = create_night_vision("rgba(255, 0, 0, 50%)")//, create_fog("darkred", 20))
}

public plugin_init() {
	HumanKnife = checkWeaponExists("knife_strong")
}

@humanGiveDefaultItems(player) {
	give_weapon_by_id(player, HumanKnife)
	change_player_flashlight(player, HumanFlashlight)
	change_player_night_vision(player, ZombieNightVision)
	set_jumps_var(player, "maximum", -1)
	give_long_jump(player, .force = 560, .height = 300, .cooldown = 5)

	if (is_player_extra_exists(player, extra_int, "gender")) {
		server_print("gender var exists")
	} else {
		server_print("gender var not exists")
	}
	set_player_extra(player, extra_int, "gender", 1)
	server_print("gender setted")
	if (is_player_extra_exists(player, extra_int, "gender")) {
		server_print("gender var exists")
	} else {
		server_print("gender var not exists")
	}
	server_print("gender: %d", get_player_extra(player, extra_int, "gender"))

	if (is_player_extra_exists(player, extra_float, "gender")) {
		server_print("gender var exists")
	} else {
		server_print("gender var not exists")
	}
	set_player_extra(player, extra_float, "gender", 2.0)
	server_print("gender setted")
	if (is_player_extra_exists(player, extra_float, "gender")) {
		server_print("gender var exists")
	} else {
		server_print("gender var not exists")
	}
	server_print("gender: %f", get_player_extra(player, extra_float, "gender"))

	if (is_player_extra_exists(player, extra_string, "gender")) {
		server_print("gender var exists")
	} else {
		server_print("gender var not exists")
	}
	set_player_extra(player, extra_string, "gender", "male")
	server_print("gender setted")
	if (is_player_extra_exists(player, extra_string, "gender")) {
		server_print("gender var exists")
	} else {
		server_print("gender var not exists")
	}
	server_print("gender: %s", get_player_extra_string(player, "gender"))
}
