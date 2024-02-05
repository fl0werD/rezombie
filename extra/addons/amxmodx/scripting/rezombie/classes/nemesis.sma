#include <amxmodx>
#include <reapi>
#include <rezombie>

new Class:NemesisClass

// kRenderFxHologram top

public plugin_precache() {
	register_plugin("Class: Nemesis", "1.0.0", "fl0wer")

	new Class:class = NemesisClass = create_class("nemesis", TEAM_ZOMBIE)
	set_class_var(class, "name", "RZ_NEMESIS")
	set_class_var(class, "forward_give_default_items", "@nemesisGiveDefaultItems")

	new Props:props = get_class_var(class, "props")
	set_props_var(props, "health", 2000)
	set_props_var(props, "gravity", 0.5)
	set_props_var(props, "weapons_interaction", false)
	set_props_var(props, "render_fx", kRenderFxGlowShell)
	set_props_var(props, "render_amount", 64)
	set_props_var(props, "render_color", "black")

	new ModelsPack:models = get_class_var(class, "models")
	models_pack_add_model(models, create_model("rezombie/players/z4hide/z4hide.mdl"))

	new Weapon:melee = get_class_var(class, "melee")
	set_weapon_var(melee, "view_model", create_model("rezombie/players/z4hide/v_z4hide.mdl"))
	set_weapon_var(melee, "always_damage", 250)
	set_weapon_var(melee, "gibs", true)
	
	add_translate("classes/nemesis")
}

public plugin_init() {
	RegisterHookChain(RG_CBasePlayer_SetAnimation, "@Player_SetAnimation_Pre", false)
}

@nemesisGiveDefaultItems(player) {
	set_jumps_var(player, "maximum", 1)
	give_long_jump(player, .force = 560, .height = 300, .cooldown = 5)
}

@Player_SetAnimation_Pre(player, PLAYER_ANIM:anim) {
	if (get_player_var(player, "class") != NemesisClass) {
		return HC_CONTINUE
	}
	if (anim != PLAYER_FLINCH && anim != PLAYER_LARGE_FLINCH) {
		return HC_CONTINUE
	}
	return HC_SUPERCEDE
}
