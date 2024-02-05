#include <amxmodx>
#include <rezombie>

public plugin_precache() {
	register_plugin("Class: Assassin", "1.0.0", "fl0wer")

	new Class:class = create_class("assassin", TEAM_ZOMBIE)

	new Props:props = get_class_var(class, "props")
	set_class_var(class, "name", "RZ_ASSASSIN")
	set_props_var(props, "health", 2000)
	set_props_var(props, "gravity", 0.5)
	set_props_var(props, "weapons_interaction", false)
	set_props_var(props, "render_fx", kRenderFxDistort)

	new ModelsPack:models = get_class_var(class, "models")
	models_pack_add_model(models, create_model("rezombie/players/husk/husk.mdl"))

	new Weapon:melee = get_class_var(class, "melee")
	set_weapon_var(melee, "view_model", create_model("rezombie/players/husk/v_husk.mdl"))
	set_weapon_var(melee, "always_damage", 1000)
	set_weapon_var(melee, "gibs", true)
	
	add_translate("classes/assassin")
}
