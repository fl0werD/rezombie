#include <amxmodx>
#include <hamsandwich>
#include <reapi>
#include <rezp>
#include <util_tempentities>

new const INFECTION_EXPLODE_SOUND[] = "zombie_plague/grenade_infect.wav"

new ModelIndex_LaserBeam
new ModelIndex_ShockWave

new Weapon:InfectGrenade

new Class:HumanClass
new Class:ZombieClass

public plugin_precache() {
	register_plugin("Grenade: Infection", "1.0.0", "fl0wer")

	HumanClass = checkClassExists("human")
	ZombieClass = checkClassExists("zombie")

	new Weapon:weapon = InfectGrenade = create_weapon("weapon_infectgrenade", "weapon_hegrenade")

	rz_weapon_set(weapon, RZ_WEAPON_NAME, "RZ_WPN_INFECT_GRENADE")
	rz_weapon_set(weapon, RZ_WEAPON_VIEW_MODEL, "models/zombie_plague/v_grenade_infect.mdl")

	precache_sound(INFECTION_EXPLODE_SOUND)

	ModelIndex_LaserBeam = precache_model("sprites/laserbeam.spr")
	ModelIndex_ShockWave = precache_model("sprites/shockwave.spr")
}

public rz_weapon_grenade_throw_post(id, entity, weapon) {
	if (weapon != InfectGrenade) {
		return
	}
	set_entity_render(entity, kRenderNormal, 16, "green", kRenderFxGlowShell)
	message_begin_f(MSG_ALL, SVC_TEMPENTITY)
	TE_BeamFollow(entity, ModelIndex_LaserBeam, 10, 10, { 0, 200, 0 }, 200)
}

public rz_weapon_grenade_explode_pre(id, grenade) {
	if (weapon != InfectGrenade) {
		return
	}
	new owner = get_entvar(grenade, var_owner)

	new Float:origin[3]
	new Float:targetOrigin[3]
	new Float:axis[3]

	get_entvar(grenade, var_origin, origin)

	axis = origin
	axis[2] += 555.0

	message_begin_f(MSG_PVS, SVC_TEMPENTITY, origin)
	TE_BeamCylinder(origin, axis, ModelIndex_ShockWave, 0, 0, 4, 60, 0, { 0, 200, 0 }, 200, 0)

	rh_emit_sound2(grenade, 0, CHAN_WEAPON, INFECTION_EXPLODE_SOUND)

	if (!is_user_connected(owner) && get_player_var(owner, "class") != ZombieClass) {
		owner = 0
	}
	for (new target = 1; target <= MaxClients; ++target) {
		if (!is_user_alive(target)) {
			continue
		}
		get_entvar(target, var_origin, targetOrigin)
		if (vector_distance(origin, targetOrigin) > 350.0) {
			continue
		}
		if (!ExecuteHamB(Ham_FVisible, target, grenade)) {
			continue
		}
		if (get_player_var(target, "class") != HumanClass) {
			continue
		}
		rz_class_player_change(target, owner, ZombieClass)
	}
}
