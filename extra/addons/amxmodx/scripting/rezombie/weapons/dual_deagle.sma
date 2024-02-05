#include <amxmodx>
#include <hamsandwich>
#include <fakemeta>
#include <reapi>
#include <rezombie>

const DUAL_DEAGLE_MAX_SPEED = 250
const DUAL_DEAGLE_WEIGHT = 7
//const ELITE_WEIGHT = 5
const DUAL_DEAGLE_DAMAGE = 65
const Float:DUAL_DEAGLE_RANGE_MODIFER = 0.81
//const Float:ELITE_RANGE_MODIFER = 0.75
const Float:DUAL_DEAGLE_RELOAD_TIME = 4.6
//const Float:ELITE_RELOAD_TIME = 4.5
new const DUAL_DEAGLE_SHOOT_SOUND[] = "weapons/deagle-1.wav"
//new const ELITE_SHOOT_SOUND[] = "weapons/elite_fire.wav"

enum {
	DUAL_DEAGLE_IDLE,
	DUAL_DEAGLE_IDLE_LEFTEMPTY,
	DUAL_DEAGLE_SHOOTLEFT1,
	DUAL_DEAGLE_SHOOTLEFT2,
	DUAL_DEAGLE_SHOOTLEFT3,
	DUAL_DEAGLE_SHOOTLEFT4,
	DUAL_DEAGLE_SHOOTLEFT5,
	DUAL_DEAGLE_SHOOTLEFTLAST,
	DUAL_DEAGLE_SHOOTRIGHT1,
	DUAL_DEAGLE_SHOOTRIGHT2,
	DUAL_DEAGLE_SHOOTRIGHT3,
	DUAL_DEAGLE_SHOOTRIGHT4,
	DUAL_DEAGLE_SHOOTRIGHT5,
	DUAL_DEAGLE_SHOOTRIGHTLAST,
	DUAL_DEAGLE_RELOAD,
	DUAL_DEAGLE_DRAW,
}

public plugin_precache() {
	register_plugin("Dual Desert Eagle", "1.0.0", "fl0wer")
	new Weapon:weapon = create_weapon("cso_dual_deagle", weapon_type_secondary)
	set_weapon_var(weapon, "name", "CSO_DUAL_DEAGLE")
	set_weapon_var(weapon, "view_model", create_model("rezombie/weapons/ddeagle/v_ddeagle.mdl"))
	set_weapon_var(weapon, "player_model", create_model("rezombie/weapons/ddeagle/p_ddeagle.mdl"))
	set_weapon_var(weapon, "world_model", create_model("rezombie/weapons/ddeagle/w_ddeagle.mdl"))
	set_weapon_var(weapon, "hud", "weapon_elite")
	set_weapon_var(weapon, "max_clip", 28)
	set_weapon_var(weapon, "max_ammo", 70)
	set_weapon_var(weapon, "slot", PISTOL_SLOT)
	set_weapon_var(weapon, "weight", DUAL_DEAGLE_WEIGHT)
	set_weapon_var(weapon, "crosshair_size", CrosshairSize_Size8)
	set_weapon_var(weapon, "forward_deploy", "@DualDeagle_Deploy")
	set_weapon_var(weapon, "forward_max_speed", "@DualDeagle_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@DualDeagle_PrimaryAttack")
	set_weapon_var(weapon, "forward_reload", "@DualDeagle_Reload")
	set_weapon_var(weapon, "forward_idle", "@DualDeagle_Idle")
	precache_sound(DUAL_DEAGLE_SHOOT_SOUND)
	//add_translate("weapons/cso")
}

@DualDeagle_Deploy(weapon, player) {
	set_member(weapon, m_Weapon_flAccuracy, 0.88)
	if (!(get_member(weapon, m_Weapon_iClip) & 1)) {
		set_member(weapon, m_Weapon_iWeaponState, WPNSTATE_ELITE_LEFT)
	}
	return weapon_default_deploy(weapon, player, DUAL_DEAGLE_DRAW, "dualpistols")
}

@DualDeagle_MaxSpeed(weapon, player) {
	return DUAL_DEAGLE_MAX_SPEED
}

@DualDeagle_PrimaryAttack(weapon, player, clip, ammo) {
	new Float:accuracy = get_member(weapon, m_Weapon_flAccuracy)
	new Float:spread
	new playerFlags = get_entvar(player, var_flags)
	if (!(playerFlags & FL_ONGROUND)) {
		spread = 1.3 * (1.0 - accuracy)
	} else if (playerVelocityLength2d(player) > 0.0) {
		spread = 0.175 * (1.0 - accuracy)
	} else if (playerFlags & FL_DUCKING) {
		spread = 0.08 * (1.0 - accuracy)
	} else {
		spread = 0.1 * (1.0 - accuracy)
	}
	new shotsFired = get_member(weapon, m_Weapon_iShotsFired)
	set_member(weapon, m_Weapon_iShotsFired, ++shotsFired)
	if (shotsFired > 1) {
		return
	}
	new Float:lastFire = Float:get_member(weapon, m_Weapon_flLastFire)
	new Float:time = get_gametime()
	if (lastFire != 0.0) {
		accuracy -= (0.325 - (time - lastFire)) * 0.275
		if (accuracy > 0.88) {
			accuracy = 0.88
		} else if (accuracy < 0.55) {
			accuracy = 0.55
		}
		set_member(weapon, m_Weapon_flAccuracy, accuracy)
	}
	set_member(weapon, m_Weapon_flLastFire, time)
	if (isEmptyFire(weapon, clip)) {
		return
	}
	set_member(weapon, m_Weapon_iClip, --clip)
	set_member(player, m_iWeaponVolume, BIG_EXPLOSION_VOLUME)
	set_member(player, m_iWeaponFlash, DIM_GUN_FLASH)
	set_entvar(player, var_effects, get_entvar(player, var_effects) | EF_MUZZLEFLASH)
	new WeaponState:weaponState = get_member(weapon, m_Weapon_iWeaponState)
	new Float:aiming[3]
	new Float:src[3]
	new Float:viewRight[3]
	getGunPositionAndAiming(player, src, aiming)
	global_get(glb_v_right, viewRight)
	if (weaponState & WPNSTATE_ELITE_LEFT) {
		rg_set_animation(player, PLAYER_ATTACK1)
		set_member(weapon, m_Weapon_iWeaponState, weaponState & ~WPNSTATE_ELITE_LEFT)
		for (new i = 0; i < sizeof(src); ++i) {
			src[i] -= viewRight[i] * 5.0
		}
		send_weapon_anim(player, random_num(DUAL_DEAGLE_SHOOTLEFT1, DUAL_DEAGLE_SHOOTLEFT4))
	} else {
		rg_set_animation(player, PLAYER_ATTACK2)
		set_member(weapon, m_Weapon_iWeaponState, weaponState | WPNSTATE_ELITE_LEFT)
		for (new i = 0; i < sizeof(src); ++i) {
			src[i] += viewRight[i] * 5.0
		}
		send_weapon_anim(player, random_num(DUAL_DEAGLE_SHOOTRIGHT1, DUAL_DEAGLE_SHOOTRIGHT4))
	}
	rg_fire_bullets3(
		weapon,
		player,
		src,
		aiming,
		spread,
		8192.0,
		1,
		BULLET_PLAYER_9MM,
		DUAL_DEAGLE_DAMAGE,
		DUAL_DEAGLE_RANGE_MODIFER,
		true,
		get_member(player, random_seed)
	)
	playFireSound(player, DUAL_DEAGLE_SHOOT_SOUND)
	new Float:nextAttack = 0.2 - 0.078
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 2.0)
	new Float:punchAngle[3]
	get_entvar(player, var_punchangle, punchAngle)
	punchAngle[0] -= 2.0
	set_entvar(player, var_punchangle, punchAngle)
}

@DualDeagle_Reload(weapon, player) {
	if (!weapon_default_reload(weapon, player, DUAL_DEAGLE_RELOAD, DUAL_DEAGLE_RELOAD_TIME)) {
		return
	}
	rg_set_animation(player, PLAYER_RELOAD)
	set_member(weapon, m_Weapon_flAccuracy, 0.88)
}

@DualDeagle_Idle(weapon, player, Float:idleTime) {
	// need rework after grenades
	ExecuteHamB(Ham_Weapon_ResetEmptySound, weapon)
	if (idleTime > 0.0) {
		return
	}
	new clip = get_member(weapon, m_Weapon_iClip)
	if (!clip) {
		return
	}
	new anim = (clip == 1) ? DUAL_DEAGLE_IDLE_LEFTEMPTY : DUAL_DEAGLE_IDLE
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 60.0)
	send_weapon_anim(player, anim)
}

/*
@DualDeagle_PrimaryAttack(weapon, player, clip, ammo) {
	new Float:accuracy = get_member(weapon, m_Weapon_flAccuracy)
	new Float:spread
	new playerFlags = get_entvar(player, var_flags)
	if (!(playerFlags & FL_ONGROUND)) {
		spread = 1.5 * (1.0 - accuracy)
	} else if (playerVelocityLength2d(player) > 0.0) {
		spread = 0.25 * (1.0 - accuracy)
	} else if (playerFlags & FL_DUCKING) {
		spread = 0.115 * (1.0 - accuracy)
	} else {
		spread = 0.13 * (1.0 - accuracy)
	}
	new shotsFired = get_member(weapon, m_Weapon_iShotsFired)
	set_member(weapon, m_Weapon_iShotsFired, ++shotsFired)
	if (shotsFired > 1) {
		return
	}
	new Float:lastFire = Float:get_member(weapon, m_Weapon_flLastFire)
	new Float:time = get_gametime()
	if (lastFire != 0.0) {
		accuracy -= (0.4 - (time - lastFire)) * 0.35
		if (accuracy > 0.9) {
			accuracy = 0.9
		} else if (accuracy < 0.55) {
			accuracy = 0.55
		}
		set_member(weapon, m_Weapon_flAccuracy, accuracy)
	}
	set_member(weapon, m_Weapon_flLastFire, time)
	if (isEmptyFire(weapon, clip)) {
		return
	}
	set_member(weapon, m_Weapon_iClip, --clip)
	set_member(player, m_iWeaponVolume, BIG_EXPLOSION_VOLUME)
	set_member(player, m_iWeaponFlash, NORMAL_GUN_FLASH)
	set_entvar(player, var_effects, get_entvar(player, var_effects) | EF_MUZZLEFLASH)
	new Float:aiming[3]
	new Float:src[3]
	getGunPositionAndAiming(player, src, aiming)
	rg_set_animation(player, PLAYER_ATTACK1)
	rg_fire_bullets3(
		weapon,
		player,
		src,
		aiming,
		spread,
		4096.0,
		2,
		BULLET_PLAYER_50AE,
		DUAL_DEAGLE_DAMAGE,
		DUAL_DEAGLE_RANGE_MODIFER,
		true,
		get_member(player, random_seed)
	)
	send_weapon_anim(player, clip ? random_num(DEAGLE_SHOOT1, DEAGLE_SHOOT2) : DEAGLE_SHOOT_EMPTY)
	playFireSound(player, DEAGLE_SHOOT_SOUND[random(sizeof(DEAGLE_SHOOT_SOUND))])
	new Float:nextAttack = 0.3 - 0.075
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 1.8)
	new Float:punchAngle[3]
	get_entvar(player, var_punchangle, punchAngle)
	punchAngle[0] -= 2.0
	set_entvar(player, var_punchangle, punchAngle)
}
*/
