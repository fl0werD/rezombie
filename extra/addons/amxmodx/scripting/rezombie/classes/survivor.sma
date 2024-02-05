#include <amxmodx>
#include <rezombie>

new Weapon:SurvivorWeapon
new Weapon:DualDeagleWeapon

public plugin_precache() {
	register_plugin("Class: Survivor", "1.0.0", "fl0wer")
	
	SurvivorWeapon = checkWeaponExists("default_m249")
	DualDeagleWeapon = checkWeaponExists("cso_dual_deagle")

	new Class:class = create_class("survivor", TEAM_HUMAN)
	set_class_var(class, "name", "RZ_SURVIVOR")
	set_class_var(class, "melee", null)
	set_class_var(class, "forward_give_default_items", "@survivorGiveDefaultItems")

	new Props:props = get_class_var(class, "props")
	set_props_var(props, "health", 111)
	set_props_var(props, "gravity", 0.5)
	set_props_var(props, "weapons_interaction", false)

	add_translate("classes/survivor")
}

@survivorGiveDefaultItems(player) {
	give_weapon_by_id(player, SurvivorWeapon)
	give_weapon_by_id(player, DualDeagleWeapon)
}
