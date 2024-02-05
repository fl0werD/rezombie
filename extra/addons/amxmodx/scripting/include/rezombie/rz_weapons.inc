#if defined _rz_weapons_included
	#endinput
#endif

#define _rz_weapons_included

// Maybe for Ham's hooks
stock const WEAPON_PLACEHOLDER[] = "weapon_ak47";

enum WeaponType
{
	weapon_type_primary,
	weapon_type_secondary,
	weapon_type_melee,
	weapon_type_grenade,
	weapon_type_extra,
};

native Weapon:create_weapon(const handle[], WeaponType:weaponType);

native any:get_weapon_var(Weapon:weapon, const var[], ...);
native set_weapon_var(Weapon:weapon, const var[], any:...);

const MAX_WEAPON_VAR_LENGTH = 32;

stock get_weapon_var_string(Weapon:weapon, const var[]) {
	new value[MAX_WEAPON_VAR_LENGTH];
	get_weapon_var(weapon, var, value, charsmax(value));
	return value;
}

native bool:is_weapon_extra_exists(Weapon:weapon, ExtraType:type, const key[]);

native any:get_weapon_extra(Weapon:weapon, ExtraType:type, const key[], any:...);
native set_weapon_extra(Weapon:weapon, ExtraType:type, const key[], any:...);

stock get_weapon_extra_string(Weapon:weapon, const key[]) {
	new value[MAX_EXTRA_VAR_LENGTH];
	get_weapon_extra(weapon, extra_string, key, value, charsmax(value));
	return value;
}

native weapons_count();
native Weapon:weapons_begin();
native Weapon:weapons_end();
native Weapon:find_weapon(const handle[]);

enum CrosshairSize {
	CrosshairSize_None,
	CrosshairSize_Size3,
	CrosshairSize_Size4,
	CrosshairSize_Size5,
	CrosshairSize_Size6,
	CrosshairSize_Size7,
	CrosshairSize_Size8,
	CrosshairSize_Size9,
};

enum RzGiveType {
	GT_Append,
	GT_Replace,
	GT_DropAndReplace
};

native Weapon:is_weapon(weapon);

// add bpammo last arg (def -1)
native give_weapon(player, const handle[], RzGiveType:giveType = GT_DropAndReplace);
native give_weapon_by_id(player, Weapon:weapon, RzGiveType:giveType = GT_DropAndReplace);

native bool:weapon_default_deploy(weapon, player, drawAnim, const playerAnim[]);

native bool:weapon_default_reload(weapon, player, reloadAnim, Float:reloadTime);

native bool:weapon_default_shotgun_reload(
	weapon,
	player,
	reloadAnim,
	reloadStartAnim,
	Float:reloadTime,
	Float:reloadStartTime,
	const reloadSound1[] = "",
	const reloadSound2[] = ""
);

native weapon_kick_back(
	weapon,
	player,
	Float:upBase,
	Float:lateralBase,
	Float:upModifier,
	Float:lateralModifier,
	Float:upMax,
	Float:lateralMax,
	directionChange
);

// return grenade
native weapon_throw_grenade(weapon, player, Float:origin[3], Float:velocity[3], Float:actionTime);

enum MeleeAttackResult {
	melee_attack_miss,
	melee_attack_hit,
	melee_attack_hit_wall,
};

native MeleeAttackResult:melee_default_attack(weapon, player, damage, distance, Float:backDamageMultiplier = 0.0);

enum MeleeAttackType {
	melee_attack_primary,
	melee_attack_secondary,
};

forward @melee_attack_pre(player, MeleeAttackType:type);
forward @melee_attack_post(player, MeleeAttackType:type, MeleeAttackResult:result);

enum MeleeSoundType {
	melee_sound_deploy,
	melee_sound_hit,
	melee_sound_slash,
	melee_sound_stab,
	melee_sound_hit_wall,
};

native melee_add_sound(Weapon:melee, MeleeSoundType:type, const path[]);

stock Weapon:checkWeaponExists(const handle[]) {
	new Weapon:weapon = find_weapon(handle);
	if (weapon == null) {
		log(Log_Warning, "Weapon '%s' not found", handle);
	}
	return weapon;
}

stock getAimingAngle(player, Float:aimingAngle[3]) {
	new Float:viewAngle[3]
	new Float:punchAngle[3]
	get_entvar(player, var_v_angle, viewAngle)
	get_entvar(player, var_punchangle, punchAngle)
	for (new i = 0; i < sizeof(viewAngle); ++i) {
		aimingAngle[i] = viewAngle[i] + punchAngle[i]
	}
}

stock getGunPositionAndAiming(player, Float:src[3], Float:aiming[3]) {
	new Float:viewAngle[3]
	getAimingAngle(player, viewAngle)
	engfunc(EngFunc_MakeVectors, viewAngle)
	ExecuteHamB(Ham_Player_GetGunPosition, player, src)
	global_get(glb_v_forward, aiming)
}
/*
stock playFireSound(player, const fireSound[]) {
	rh_emit_sound2(player, 0, CHAN_WEAPON, fireSound, VOL_NORM, ATTN_NORM, 0, 94 + random_num(0, 15))
}
*/
stock play_weapon_sound(player, const sound[]) {
	rh_emit_sound2(player, 0, CHAN_WEAPON, sound)
}

stock play_random_weapon_sound(player, const sounds[][], size = sizeof(sounds)) {
	rh_emit_sound2(player, 0, CHAN_WEAPON, sounds[random_num(0, size - 1)])
}

stock bool:isEmptyFire(weapon, clip, bool:isShotgun = false) {
	if (clip <= 0) {
		if (isShotgun) {
			if (!get_member(weapon, m_Weapon_fInSpecialReload)) {
				ExecuteHamB(Ham_Weapon_PlayEmptySound, weapon)
			}
			ExecuteHamB(Ham_Weapon_Reload, weapon)
		} else {
			if (get_member(weapon, m_Weapon_fFireOnEmpty)) {
				ExecuteHamB(Ham_Weapon_PlayEmptySound, weapon)
				set_member(weapon, m_Weapon_flNextPrimaryAttack, 0.2)
			}
		}
		return true
	}
	return false
}

stock Float:playerVelocityLength2d(player) {
	new Float:velocity[3]
	get_entvar(player, var_velocity, velocity)
	velocity[2] = 0.0
	return vector_length(velocity)
}

stock modifyVelocityByScalar(entity, Float:scalar) {
	new Float:velocity[3]
	get_entvar(entity, var_velocity, velocity)
	for (new i = 0; i < sizeof(velocity); ++i) {
		velocity[i] *= scalar
	}
	set_entvar(entity, var_velocity, velocity)
}
