#include <amxmodx>
#include <reapi>
#include <rezombie>

new Class:SniperClass
new Weapon:SniperWeapon

public plugin_precache() {
	register_plugin("Class: Sniper", "1.0.0", "fl0wer")

	SniperWeapon = checkWeaponExists("default_awp")

	new Class:class = SniperClass = create_class("sniper", TEAM_HUMAN)
	set_class_var(class, "name", "RZ_SNIPER")
	set_class_var(class, "melee", null)
	set_class_var(class, "forward_give_default_items", "@sniperGiveDefaultItems")

	new Props:props = get_class_var(class, "props")
	set_props_var(props, "health", 250)
	set_props_var(props, "armor", 100)
	set_props_var(props, "gravity", 0.5)
	set_props_var(props, "weapons_interaction", false)

	add_translate("classes/sniper")
}

public plugin_init() {
	RegisterHookChain(RG_CBasePlayer_TakeDamage, "@Player_TakeDamage_Pre", false)
	RegisterHookChain(RG_CBasePlayer_Killed, "@Player_Killed_Pre", false)
}

@sniperGiveDefaultItems(player) {
	give_weapon_by_id(player, SniperWeapon)
}

// always damage
@Player_TakeDamage_Pre(player, inflictor, attacker, Float:damage, damageType) {
	if (player == attacker || !rg_is_player_can_takedamage(player, attacker)) {
		return
	}
	if (get_player_var(attacker, "class") != SniperClass) {
		return
	}
	// inflictor impulse == ZombieMelee
	if (get_player_var(attacker, "active_weapon") != SniperWeapon) {
		return
	}
	SetHookChainArg(4, ATYPE_FLOAT, 1000.0)
}

@Player_Killed_Pre(player, attacker, gib) {
	if (player == attacker || !is_user_connected(attacker)) {
		return
	}
	if (!rg_is_player_can_takedamage(player, attacker)) {
		return
	}
	// inflictor
	if (get_player_var(attacker, "class") != SniperClass) {
		return
	}
	if (get_player_var(attacker, "active_weapon") != SniperWeapon) {
		return
	}
	SetHookChainArg(3, ATYPE_INTEGER, GIB_ALWAYS)
	new Float:origin[3]
	get_entvar(player, var_origin, origin)
	message_begin_f(MSG_PVS, SVC_TEMPENTITY, origin)
	TE_LavaSplash(origin)
}

TE_LavaSplash(Float:position[3]) {
	write_byte(TE_LAVASPLASH)
	write_coord_f(position[0])
	write_coord_f(position[1])
	write_coord_f(position[2])
	message_end()
}
