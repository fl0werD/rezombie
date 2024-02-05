#include <amxmodx>
#include <hamsandwich>
#include <fakemeta>
#include <reapi>
#include <rezombie>

/*

new const WEAPON_NAMES[WeaponIdType:MAX_WEAPONS][] = {
	"", "228 Compact", "", "SSG 08", "Осколочная граната", "XM1014", "Взрывчатка C4", "MAC-10", "AUG", "Дымовая граната",
	"Dual Berettas", "Five-SeveN", "UMP-45", "SG 550", "Galil", "FAMAS", "USP", "Glock-18", "AWP", "MP5",
	"M249", "M3", "M4A1", "TMP", "G3SG1", "Световая граната", "Desert Eagle", "SG 552", "AK-47", "Нож", "P90", "",
}

*/

new const GLOCK_SHOOT_SOUND[2][] = {
	"weapons/csgo/glock/glock-1.wav",
	"weapons/csgo/glock/glock-2.wav",
}
new const USP_SHOOT_SOUND[2][] = {
	"weapons/csgo/usp/usp_unsil-1.wav",
	"weapons/csgo/usp/usp-1.wav",
}
new const M4A1_SHOOT_SOUND[2][] = {
	"weapons/csgo/m4a1/m4a1_unsil-1.wav",
	"weapons/csgo/m4a1/m4a1-1.wav",
}
new const AK47_SHOOT_SOUND[] = "weapons/csgo/ak47/ak47-1.wav"
new const AWP_SHOOT_SOUND[] = "weapons/csgo/awp/awp-1.wav"
new const NEGEV_SHOOT_SOUND[] = "weapons/csgo/negev/negev-1.wav" // 2

new const ADDITIONAL_SOUNDS[][] = {
	"weapons/csgo/glock/glock_clipin.wav",
	"weapons/csgo/glock/glock_clipout.wav",
	"weapons/csgo/glock/glock_draw.wav",
	"weapons/csgo/glock/glock_slideback.wav",
	"weapons/csgo/glock/glock_sliderelease.wav",

	"weapons/csgo/usp/usp_silencer_screw_on_start.wav",
	"weapons/csgo/usp/usp_silencer_screw1.wav",
	"weapons/csgo/usp/usp_silencer_screw2.wav",
	"weapons/csgo/usp/usp_silencer_screw3.wav",
	"weapons/csgo/usp/usp_silencer_screw4.wav",
	"weapons/csgo/usp/usp_silencer_screw5.wav",

	"weapons/csgo/m4a1/m4a1_cliphit.wav",
	"weapons/csgo/m4a1/m4a1_clipin.wav",
	"weapons/csgo/m4a1/m4a1_clipout.wav",
	"weapons/csgo/m4a1/m4a1_draw.wav",
	"weapons/csgo/m4a1/m4a1_silencer_boltback.wav",
	"weapons/csgo/m4a1/m4a1_silencer_boltforward.wav",
	"weapons/csgo/m4a1/m4a1_silencer_screw_1.wav",
	"weapons/csgo/m4a1/m4a1_silencer_screw_2.wav",
	"weapons/csgo/m4a1/m4a1_silencer_screw_3.wav",
	"weapons/csgo/m4a1/m4a1_silencer_screw_4.wav",
	"weapons/csgo/m4a1/m4a1_silencer_screw_5.wav",
	"weapons/csgo/m4a1/m4a1_silencer_screw_off_end.wav",
	"weapons/csgo/m4a1/m4a1_silencer_screw_on_start.wav",

	"weapons/csgo/ak47/ak47_boltpull.wav",
	"weapons/csgo/ak47/ak47_clipin.wav",
	"weapons/csgo/ak47/ak47_clipout.wav",
	"weapons/csgo/ak47/ak47_draw.wav",

	"weapons/csgo/awp/awp_boltback.wav",
	"weapons/csgo/awp/awp_boltforward.wav",
	"weapons/csgo/awp/awp_cliphit.wav",
	"weapons/csgo/awp/awp_clipin.wav",
	"weapons/csgo/awp/awp_clipout.wav",
	"weapons/csgo/awp/awp_draw.wav",

	"weapons/csgo/negev/negev_boxin.wav",
	"weapons/csgo/negev/negev_boxout.wav",
	"weapons/csgo/negev/negev_chain.wav",
	"weapons/csgo/negev/negev_coverdown.wav",
	"weapons/csgo/negev/negev_coverup.wav",
	"weapons/csgo/negev/negev_draw.wav",
	"weapons/csgo/negev/negev_pump.wav",
}

new const FLASHLIGHT_IMPULSE = 100

new Weapon:glock
new Weapon:usp
new Weapon:deagle
new Weapon:p90
// m4a1s
new Weapon:m4a1
new Weapon:ak47
new Weapon:awp
new Weapon:negev

public plugin_precache() {
	register_plugin("[RZ] CS:GO Weapons", "1.0.0", "fl0wer")

/*
	glock = create_weapon("csgo_glock", weapon_type_secondary)
	set_weapon_var(glock, weapon_view_model, "models/rz/glock/v_glock.mdl")
	set_weapon_var(glock, weapon_player_model, "models/rz/glock/p_glock.mdl")
	set_weapon_var(glock, weapon_world_model, "models/rz/glock/w_glock.mdl")
	precache_sound(GLOCK_SHOOT_SOUND[0])
	precache_sound(GLOCK_SHOOT_SOUND[1])

	usp = create_weapon("csgo_usp", weapon_type_secondary)
	set_weapon_var(usp, weapon_view_model, "models/rz/usp/v_usp.mdl")
	set_weapon_var(usp, weapon_player_model, "models/rz/usp/p_usp.mdl")
	set_weapon_var(usp, weapon_world_model, "models/rz/usp/w_usp.mdl")
	precache_sound(USP_SHOOT_SOUND[0])
	precache_sound(USP_SHOOT_SOUND[1])
*/

	createDeagle()

	createP90()

/*
	m4a1 = create_weapon("csgo_m4a1", weapon_type_primary)
	set_weapon_var(m4a1, weapon_view_model, "models/rz/m4a1/v_m4a1.mdl")
	set_weapon_var(m4a1, weapon_player_model, "models/rz/m4a1/p_m4a1.mdl")
	set_weapon_var(m4a1, weapon_world_model, "models/rz/m4a1/w_m4a1.mdl")
	precache_sound(M4A1_SHOOT_SOUND[0])
	precache_sound(M4A1_SHOOT_SOUND[1])

	ak47 = create_weapon("csgo_ak47", weapon_type_primary)
	set_weapon_var(ak47, weapon_view_model, "models/rz/ak47/v_ak47.mdl")
	set_weapon_var(ak47, weapon_player_model, "models/rz/ak47/p_ak47.mdl")
	set_weapon_var(ak47, weapon_world_model, "models/rz/ak47/w_ak47.mdl")
	precache_sound(AK47_SHOOT_SOUND)

	awp = create_weapon("csgo_awp", weapon_type_primary)
	set_weapon_var(awp, weapon_view_model, "models/rz/awp/v_awp.mdl")
	set_weapon_var(awp, weapon_player_model, "models/rz/awp/p_awp.mdl")
	set_weapon_var(awp, weapon_world_model, "models/rz/awp/w_awp.mdl")
	set_weapon_var(awp, weapon_forward_deploy, "@AWP_Deploy")
	set_weapon_var(awp, weapon_forward_primary_attack, "@AWP_PrimaryAttack")
	set_weapon_var(awp, weapon_forward_secondary_attack, "@AWP_SecondaryAttack")
	precache_sound(AWP_SHOOT_SOUND)

	negev = create_weapon("csgo_negev", weapon_type_primary)
	set_weapon_var(negev, weapon_view_model, "models/rz/negev/v_negev.mdl")
	set_weapon_var(negev, weapon_player_model, "models/rz/negev/p_negev.mdl")
	set_weapon_var(negev, weapon_world_model, "models/rz/negev/w_negev.mdl")
	precache_sound(NEGEV_SHOOT_SOUND)
*/
}

public plugin_init() {
	//RegisterHookChain(RG_CBasePlayer_PreThink, "@CBasePlayer_PreTnink_Post", true)
}

const DEAGLE_MAX_SPEED = 250
const DEAGLE_WEIGHT = 7
const DEAGLE_DAMAGE = 54
const Float:DEAGLE_RANGE_MODIFER = 0.81
const Float:DEAGLE_RELOAD_TIME = 2.2
new const DEAGLE_SHOOT_SOUND[] = "weapons/csgo/deagle/deagle-1.wav"
new const DEAGLE_ADDITIONAL_SOUNDS[][] = {
	"weapons/csgo/deagle/de_clipin.wav",
	"weapons/csgo/deagle/de_clipout.wav",
	"weapons/csgo/deagle/de_draw.wav",
	"weapons/csgo/deagle/de_slideback.wav",
	"weapons/csgo/deagle/de_slideforward.wav",
	"weapons/csgo/deagle/deagle_special_lookat_f009.wav",
	"weapons/csgo/deagle/deagle_special_lookat_f036.wav",
	"weapons/csgo/deagle/deagle_special_lookat_f057.wav",
	"weapons/csgo/deagle/deagle_special_lookat_f081.wav",
	"weapons/csgo/deagle/deagle_special_lookat_f111.wav",
	"weapons/csgo/deagle/deagle_special_lookat_f133.wav",
	"weapons/csgo/deagle/deagle_special_lookat_f166.wav",
	"weapons/csgo/deagle/deagle_special_lookat_f193.wav",
	"weapons/csgo/deagle/deagle_special_lookat_f228.wav",
}

enum {
	DEAGLE_IDLE,
	DEAGLE_SHOOT1,
	DEAGLE_SHOOT2,
	DEAGLE_SHOOT_EMPTY,
	DEAGLE_RELOAD,
	DEAGLE_DRAW,
}

createDeagle() {
	new Weapon:weapon = create_weapon("csgo_deagle", weapon_type_secondary)
	set_weapon_var(weapon, "name", "DEFAULT_DEAGLE")
	set_weapon_var(weapon, "view_model", create_model("rezombie/weapons/deagle/v_deagle.mdl"))
	set_weapon_var(weapon, "player_model", create_model("rezombie/weapons/deagle/p_deagle.mdl"))
	set_weapon_var(weapon, "world_model", create_model("rezombie/weapons/deagle/w_deagle.mdl"))
	set_weapon_var(weapon, "hud", "weapon_deagle")
	set_weapon_var(weapon, "max_clip", 7)
	set_weapon_var(weapon, "max_ammo", 35)
	set_weapon_var(weapon, "slot", PISTOL_SLOT)
	set_weapon_var(weapon, "weight", DEAGLE_WEIGHT)
	set_weapon_var(weapon, "crosshair_size", CrosshairSize_Size8)
	set_weapon_var(weapon, "forward_deploy", "@Deagle_Deploy")
	set_weapon_var(weapon, "forward_max_speed", "@Deagle_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@Deagle_PrimaryAttack")
	set_weapon_var(weapon, "forward_reload", "@Deagle_Reload")
	set_weapon_var(weapon, "forward_idle", "@Deagle_Idle")
	precache_sound(DEAGLE_SHOOT_SOUND)
	precache_sounds(DEAGLE_ADDITIONAL_SOUNDS)
}

@Deagle_Deploy(weapon, player) {
	set_member(weapon, m_Weapon_flAccuracy, 0.9)
	return weapon_default_deploy(weapon, player, DEAGLE_DRAW, "onehanded")
}

@Deagle_MaxSpeed(weapon, player) {
	return DEAGLE_MAX_SPEED
}

@Deagle_PrimaryAttack(weapon, player, clip, ammo) {
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
		player, // if it's inflictor, maybe weapon?
		player,
		src,
		aiming,
		spread,
		4096.0,
		2,
		BULLET_PLAYER_50AE,
		DEAGLE_DAMAGE,
		DEAGLE_RANGE_MODIFER,
		true,
		get_member(player, random_seed)
	)
	send_weapon_anim(player, clip ? random_num(DEAGLE_SHOOT1, DEAGLE_SHOOT2) : DEAGLE_SHOOT_EMPTY)
	playFireSound(player, DEAGLE_SHOOT_SOUND)
	new Float:nextAttack = 0.3 - 0.075
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 1.8)
	new Float:punchAngle[3]
	get_entvar(player, var_punchangle, punchAngle)
	punchAngle[0] -= 2.0
	set_entvar(player, var_punchangle, punchAngle)
}

@Deagle_Reload(weapon, player) {
	if (!weapon_default_reload(weapon, player, DEAGLE_RELOAD, DEAGLE_RELOAD_TIME)) {
		return
	}
	rg_set_animation(player, PLAYER_RELOAD)
	set_member(weapon, m_Weapon_flAccuracy, 0.9)
}

@Deagle_Idle(weapon, player, Float:idleTime) {
	// need rework after grenades
	ExecuteHamB(Ham_Weapon_ResetEmptySound, weapon)
	if (idleTime > 0.0) {
		return
	}
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 20.0)
	send_weapon_anim(player, DEAGLE_IDLE)
}

const P90_MAX_SPEED = 230
const P90_WEIGHT = 26
const P90_DAMAGE = 25
const Float:P90_RANGE_MODIFER = 0.86
const Float:P90_RELOAD_TIME = 3.4
const Float:P90_BASE_ACCURACY = 0.2
const Float:P90_ACCURACY_DIVISOR = 175.0
const Float:P90_INSPECT_TIME = 5.37
new const P90_SHOOT_SOUND[] = "weapons/csgo/p90/p90-1.wav"
new const P90_ADDITIONAL_SOUNDS[][] = {
	"weapons/csgo/p90/p90_boltback.wav",
	"weapons/csgo/p90/p90_boltforward.wav",
	"weapons/csgo/p90/p90_cliphit.wav",
	"weapons/csgo/p90/p90_clipin.wav",
	"weapons/csgo/p90/p90_clipout.wav",
	"weapons/csgo/p90/p90_cliprelease.wav",
	"weapons/csgo/p90/p90_draw.wav",
}

enum {
	P90_IDLE,
	P90_RELOAD,
	P90_DRAW,
	P90_SHOOT1,
	P90_SHOOT2,
	P90_SHOOT3,
	P90_INSPECT,
}

createP90() {
	new Weapon:weapon = create_weapon("csgo_p90", weapon_type_primary)
	set_weapon_var(weapon, "name", "DEFAULT_P90")
	set_weapon_var(weapon, "view_model", create_model("rezombie/weapons/p90/v_p90.mdl"))
	set_weapon_var(weapon, "player_model", create_model("rezombie/weapons/p90/p_p90.mdl"))
	set_weapon_var(weapon, "world_model", create_model("rezombie/weapons/p90/w_p90.mdl"))
	set_weapon_var(weapon, "hud", "weapon_p90")
	set_weapon_var(weapon, "max_clip", 50)
	set_weapon_var(weapon, "max_ammo", 100)
	set_weapon_var(weapon, "slot", PRIMARY_WEAPON_SLOT)
	set_weapon_var(weapon, "weight", P90_WEIGHT)
	set_weapon_var(weapon, "base_accuracy", P90_BASE_ACCURACY)
	set_weapon_var(weapon, "crosshair_size", CrosshairSize_Size7)
	set_weapon_var(weapon, "forward_deploy", "@P90_Deploy")
	set_weapon_var(weapon, "forward_max_speed", "@P90_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@P90_PrimaryAttack")
	set_weapon_var(weapon, "forward_reload", "@P90_Reload")
	set_weapon_var(weapon, "forward_idle", "@P90_Idle")
	precache_sound(P90_SHOOT_SOUND)
	precache_sounds(P90_ADDITIONAL_SOUNDS)
}

@P90_Deploy(weapon, player) {
	set_member(weapon, m_Weapon_iShotsFired, 0)
	set_member(weapon, m_Weapon_bDelayFire, false)
	set_member(weapon, m_Weapon_flAccuracy, P90_BASE_ACCURACY)
	return weapon_default_deploy(weapon, player, P90_DRAW, "carbine")
}

@P90_MaxSpeed(weapon, player) {
	return P90_MAX_SPEED
}

@P90_PrimaryAttack(weapon, player, clip, ammo) {
	new Float:accuracy = get_member(weapon, m_Weapon_flAccuracy)
	new Float:spread
	new playerFlags = get_entvar(player, var_flags)
	new Float:playerVelocityLength = playerVelocityLength2d(player)
	if (!(playerFlags & FL_ONGROUND)) {
		spread = 0.3 * accuracy
	} else if (playerVelocityLength > 170.0) {
		spread = 0.115 * accuracy
	} else {
		spread = 0.045 * accuracy
	}
	set_member(weapon, m_Weapon_bDelayFire, true)
	new shotsFired = get_member(weapon, m_Weapon_iShotsFired) + 1
	set_member(weapon, m_Weapon_iShotsFired, shotsFired)
	accuracy = (shotsFired * shotsFired / P90_ACCURACY_DIVISOR) + 0.45
	if (accuracy > 1.0) {
		accuracy = 1.0
	}
	set_member(weapon, m_Weapon_flAccuracy, accuracy)
	if (isEmptyFire(weapon, clip)) {
		return
	}
	set_member(weapon, m_Weapon_iClip, --clip)
	set_member(player, m_iWeaponVolume, NORMAL_GUN_VOLUME)
	set_member(player, m_iWeaponFlash, DIM_GUN_FLASH)
	set_entvar(player, var_effects, get_entvar(player, var_effects) | EF_MUZZLEFLASH)
	new Float:aiming[3]
	new Float:src[3]
	getGunPositionAndAiming(player, src, aiming)
	rg_set_animation(player, PLAYER_ATTACK1)
	rg_fire_bullets3(
		player, // if it's inflictor, maybe weapon?
		player,
		src,
		aiming,
		spread,
		8192.0,
		2,
		BULLET_PLAYER_57MM,
		P90_DAMAGE,
		P90_RANGE_MODIFER,
		false,
		get_member(player, random_seed)
	)
	send_weapon_anim(player, random_num(P90_SHOOT1, P90_SHOOT3))
	playFireSound(player, P90_SHOOT_SOUND)
	new Float:nextAttack = 0.066
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 2.0)
	if (!(playerFlags & FL_ONGROUND)) {
		weapon_kick_back(weapon, player, 0.9, 0.45, 0.35, 0.04, 5.25, 3.5, 4)
	} else if (playerVelocityLength > 0.0) {
		weapon_kick_back(weapon, player, 0.45, 0.3, 0.2, 0.0275, 4.0, 2.25, 7)
	} else if (playerFlags & FL_DUCKING) {
		weapon_kick_back(weapon, player, 0.275, 0.2, 0.125, 0.02, 3.0, 1.0, 9)
	} else {
		weapon_kick_back(weapon, player, 0.3, 0.225, 0.125, 0.02, 3.25, 1.25, 8)
	}
}

@P90_Reload(weapon, player) {
	if (!weapon_default_reload(weapon, player, P90_RELOAD, P90_RELOAD_TIME)) {
		return
	}
	rg_set_animation(player, PLAYER_RELOAD)
	set_member(weapon, m_Weapon_iShotsFired, 0)
	set_member(weapon, m_Weapon_flAccuracy, P90_BASE_ACCURACY)
}

@P90_Idle(weapon, player, Float:idleTime) {
	// need rework after grenades
	ExecuteHamB(Ham_Weapon_ResetEmptySound, weapon)
	if (idleTime > 0.0) {
		return
	}
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 20.0)
	send_weapon_anim(player, P90_IDLE)
}

@CBasePlayer_PreTnink_Post(player) {
	if (!is_user_alive(player)) {
		return
	}
	if (get_entvar(player, var_impulse) != FLASHLIGHT_IMPULSE) {
		return
	}
	set_entvar(player, var_impulse, 0)
	if (get_member(player, m_iFOV) != DEFAULT_NO_ZOOM) {
		return
	}
	new activeItem = get_member(player, m_pActiveItem)
	if (is_nullent(activeItem)) {
		return
	}
	if (get_member(activeItem, m_Weapon_fInReload) || get_member(activeItem, m_Weapon_fInSpecialReload)) {
		return
	}
	new Weapon:impulse = get_entvar(activeItem, var_impulse)
	new animNumber
	new Float:animTime
	if (impulse == glock) {
		animNumber = 13
		animTime = 6.47
	} else if (impulse == usp) {
		animNumber = get_member(activeItem, m_Weapon_iWeaponState) ? 16 : 17
		animTime = 6.45
	} else if (impulse == deagle) {
		animNumber = 6
		animTime = 5.7 // two
	} else if (impulse == p90) {
		animNumber = P90_INSPECT
		animTime = P90_INSPECT_TIME
	} else if (impulse == m4a1) {
		animNumber = get_member(activeItem, m_Weapon_iWeaponState) ? 14 : 15
		animTime = 5.34
	} else if (impulse == ak47) {
		animNumber = 6
		animTime = 4.6
	} else if (impulse == awp) {
		animNumber = 6
		animTime = 5.0
	} else if (impulse == negev) {
		animNumber = 5
		animTime = 6.9
	} else {
		return
	}
	if (get_entvar(player, var_weaponanim) != animNumber) {
		set_member(activeItem, m_Weapon_flTimeWeaponIdle, animTime)
		send_weapon_anim(player, animNumber)
	}
}
