#include <amxmodx>
#include <hamsandwich>
#include <fakemeta>
#include <reapi>
#include <rezombie>

// rework fire anim with empty
// muzzeflash
// decals
// eject brass

public plugin_precache() {
	register_plugin("Default Weapons Pack", "1.0.0", "fl0wer")
	add_translate("weapons/default")

	createGlock18()
	createUSP()
	createP228()
	createFiveSeven()
	createDeagle()
	createElite()

	createM3()
	createXM1014()

	createMAC10()
	createTMP()
	createMP5()
	createUMP45()
	createP90()

	createGalil()
	createFamas()
	createAK47()
	createM4A1()
	createSG552()
	createAUG()

	createScout()
	createAWP()
	createG3SG1()
	createSG550()

	createM249()

	createHeGrenade()
}

const GLOCK18_MAX_SPEED = 250
const GLOCK18_WEIGHT = 5
const GLOCK18_DAMAGE = 25
const GLOCK18_DAMAGE_BURST = 18
const Float:GLOCK18_RANGE_MODIFER = 0.75
const Float:GLOCK18_RANGE_MODIFER_BURST = 0.9
const Float:GLOCK18_RELOAD_TIME = 2.2
new const GLOCK18_SHOOT_SOUND[2][] = {
	"weapons/glock18-1.wav",
	"weapons/glock18-2.wav",
}

enum {
	GLOCK18_IDLE1,
	GLOCK18_IDLE2,
	GLOCK18_IDLE3,
	GLOCK18_SHOOT1,
	GLOCK18_SHOOT2,
	GLOCK18_SHOOT3,
	GLOCK18_SHOOT_EMPTY,
	GLOCK18_RELOAD,
	GLOCK18_DRAW,
	GLOCK18_HOLSTER,
	GLOCK18_ADD_SILENCER,
	GLOCK18_DRAW2,
	GLOCK18_RELOAD2,
}

createGlock18() {
	new Weapon:weapon = create_weapon("default_glock18", weapon_type_secondary)
	set_weapon_var(weapon, "name", "DEFAULT_GLOCK18")
	set_weapon_var(weapon, "view_model", create_model("models/v_glock18.mdl"))
	set_weapon_var(weapon, "player_model", create_model("models/p_glock18.mdl"))
	set_weapon_var(weapon, "world_model", create_model("models/w_glock18.mdl"))
	set_weapon_var(weapon, "hud", "weapon_glock18")
	set_weapon_var(weapon, "max_clip", 20)
	set_weapon_var(weapon, "max_ammo", 120)
	set_weapon_var(weapon, "slot", PRIMARY_WEAPON_SLOT)
	set_weapon_var(weapon, "weight", GLOCK18_WEIGHT)
	set_weapon_var(weapon, "crosshair_size", CrosshairSize_Size8)
	set_weapon_var(weapon, "forward_deploy", "@Glock18_Deploy")
	set_weapon_var(weapon, "forward_max_speed", "@Glock18_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@Glock18_PrimaryAttack")
	set_weapon_var(weapon, "forward_fire_remaining", "@Glock18_FireRemaining")
	set_weapon_var(weapon, "forward_secondary_attack", "@Glock18_SecondaryAttack")
	set_weapon_var(weapon, "forward_reload", "@Glock18_Reload")
	set_weapon_var(weapon, "forward_idle", "@Glock18_Idle")
	precache_sounds(GLOCK18_SHOOT_SOUND)
}

@Glock18_Deploy(weapon, player) {
	set_member(weapon, m_Weapon_flAccuracy, 0.9)
	set_member(weapon, m_Weapon_iRemainingShotsFired, 0)
	set_member(weapon, m_Weapon_flNextRemainingShoot, 0.0)
	new anim = random_num(0, 1) ? GLOCK18_DRAW : GLOCK18_DRAW2
	return weapon_default_deploy(weapon, player, anim, "onehanded")
}

@Glock18_MaxSpeed(weapon, player) {
	return GLOCK18_MAX_SPEED
}

@Glock18_PrimaryAttack(weapon, player, clip, ammo) {
	new bool:isBurstMode = (get_member(weapon, m_Weapon_iWeaponState) & WPNSTATE_GLOCK18_BURST_MODE) ? true : false
	new Float:nextAttack = isBurstMode ? 0.5 : (0.2 - 0.05)
	new Float:accuracy = get_member(weapon, m_Weapon_flAccuracy)
	new Float:spread
	new playerFlags = get_entvar(player, var_flags)
	if (isBurstMode) {
		if (!(playerFlags & FL_ONGROUND)) {
			spread = 1.2 * (1.0 - accuracy)
		} else if (playerVelocityLength2d(player) > 0.0) {
			spread = 0.185 * (1.0 - accuracy)
		} else if (playerFlags & FL_DUCKING) {
			spread = 0.095 * (1.0 - accuracy)
		} else {
			spread = 0.3 * (1.0 - accuracy)
		}
	} else {
		if (!(playerFlags & FL_ONGROUND)) {
			spread = 1.0 * (1.0 - accuracy)
		} else if (playerVelocityLength2d(player) > 0.0) {
			spread = 0.165 * (1.0 - accuracy)
		} else if (playerFlags & FL_DUCKING) {
			spread = 0.075 * (1.0 - accuracy)
		} else {
			spread = 0.1 * (1.0 - accuracy)
		}
		new shotsFired = get_member(weapon, m_Weapon_iShotsFired)
		set_member(weapon, m_Weapon_iShotsFired, ++shotsFired)
		if (shotsFired > 1) {
			return
		}
	}
	new Float:lastFire = Float:get_member(weapon, m_Weapon_flLastFire)
	new Float:time = get_gametime()
	if (lastFire != 0.0) {
		accuracy -= (0.325 - (time - lastFire)) * 0.275
		if (accuracy > 0.9) {
			accuracy = 0.9
		} else if (accuracy < 0.6) {
			accuracy = 0.6
		}
		set_member(weapon, m_Weapon_flAccuracy, accuracy)
	}
	set_member(weapon, m_Weapon_flLastFire, time)
	if (isEmptyFire(weapon, clip)) {
		return
	}
	set_member(weapon, m_Weapon_iClip, --clip)
	set_member(player, m_iWeaponVolume, NORMAL_GUN_VOLUME)
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
		8192.0,
		1,
		BULLET_PLAYER_9MM,
		GLOCK18_DAMAGE,
		GLOCK18_RANGE_MODIFER,
		true,
		get_member(player, random_seed)
	)
	if (isBurstMode) {
		send_weapon_anim(player, clip ? GLOCK18_SHOOT1 : GLOCK18_SHOOT_EMPTY)
		playFireSound(player, GLOCK18_SHOOT_SOUND[0])
	} else {
		send_weapon_anim(player, clip ? GLOCK18_SHOOT3 : GLOCK18_SHOOT_EMPTY)
		playFireSound(player, GLOCK18_SHOOT_SOUND[1])
	}
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 2.5)
	if (isBurstMode) {
		set_member(weapon, m_Weapon_iRemainingShotsFired, 1)
		set_member(weapon, m_Weapon_flNextRemainingShoot, time + 0.1)
	}
}

@Glock18_FireRemaining(weapon, player, clip, shotsFired) {
	// refactoring from 1 to 3
	if (--clip < 0) {
		set_member(weapon, m_Weapon_iClip, 0)
		set_member(weapon, m_Weapon_iRemainingShotsFired, 3)
		set_member(weapon, m_Weapon_flNextRemainingShoot, 0.0)
		return
	}
	set_member(weapon, m_Weapon_iClip, clip)
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
		0.05,
		8192.0,
		1,
		BULLET_PLAYER_9MM,
		GLOCK18_DAMAGE_BURST,
		GLOCK18_RANGE_MODIFER_BURST,
		true,
		get_member(player, random_seed)
	)
	send_weapon_anim(player, clip ? GLOCK18_SHOOT1 : GLOCK18_SHOOT_EMPTY)
	playFireSound(player, GLOCK18_SHOOT_SOUND[0])
	set_member(weapon, m_Weapon_iRemainingShotsFired, ++shotsFired)
	if (shotsFired != 3) {
		set_member(weapon, m_Weapon_flNextRemainingShoot, get_gametime() + 0.1)
	} else {
		set_member(weapon, m_Weapon_flNextRemainingShoot, 0.0)
	}
}

@Glock18_SecondaryAttack(weapon, player, WeaponState:weaponState) {
	if (weaponState & WPNSTATE_GLOCK18_BURST_MODE) {
		set_member(weapon, m_Weapon_iWeaponState, weaponState & ~WPNSTATE_GLOCK18_BURST_MODE)
		client_print(player, print_center, "#Cstrike_TitlesTXT_Switch_To_SemiAuto")
	} else {
		set_member(weapon, m_Weapon_iWeaponState, weaponState | WPNSTATE_GLOCK18_BURST_MODE)
		client_print(player, print_center, "#Cstrike_TitlesTXT_Switch_To_BurstFire")
	}
	set_member(weapon, m_Weapon_flNextSecondaryAttack, 0.3)
}

@Glock18_Reload(weapon, player) {
	new anim = random_num(0, 1) ? GLOCK18_RELOAD : GLOCK18_RELOAD2
	if (!weapon_default_reload(weapon, player, anim, GLOCK18_RELOAD_TIME)) {
		return
	}
	rg_set_animation(player, PLAYER_RELOAD)
	set_member(weapon, m_Weapon_flAccuracy, 0.9)
}

@Glock18_Idle(weapon, player, Float:idleTime) {
	// need rework after grenades
	ExecuteHamB(Ham_Weapon_ResetEmptySound, weapon)
	if (idleTime > 0.0) {
		return
	}
	if (!get_member(weapon, m_Weapon_iClip)) {
		return
	}
	new Float:random = random_float(0.0, 1.0)
	new anim
	new Float:time
	if (random <= 0.3) {
		anim = GLOCK18_IDLE3
		time = 3.0625
	} else if (random <= 0.6) {
		anim = GLOCK18_IDLE1
		time = 3.75
	} else {
		anim = GLOCK18_IDLE2
		time = 2.5
	}
	set_member(weapon, m_Weapon_flTimeWeaponIdle, time)
	send_weapon_anim(player, anim)
}

const USP_MAX_SPEED = 250
const USP_WEIGHT = 5
const USP_DAMAGE = 34
const USP_DAMAGE_SILENCER = 30
const Float:USP_RANGE_MODIFER = 0.79
const Float:USP_RELOAD_TIME = 2.7
const Float:USP_ADJUST_SILENCER_TIME = 3.13
new const USP_SHOOT_SOUND[3][] = {
	"weapons/usp1.wav",
	"weapons/usp1.wav",
	"weapons/usp_unsil-1.wav",
}

enum {
	USP_SILENCER_IDLE,
	USP_SILENCER_SHOOT1,
	USP_SILENCER_SHOOT2,
	USP_SILENCER_SHOOT3,
	USP_SILENCER_SHOOT_EMPTY,
	USP_SILENCER_RELOAD,
	USP_SILENCER_DRAW,
	USP_ATTACH_SILENCER,
	USP_IDLE,
	USP_SHOOT1,
	USP_SHOOT2,
	USP_SHOOT3,
	USP_SHOOT_EMPTY,
	USP_RELOAD,
	USP_DRAW,
	USP_DETACH_SILENCER,
}

createUSP() {
	new Weapon:weapon = create_weapon("default_usp", weapon_type_secondary)
	set_weapon_var(weapon, "name", "DEFAULT_USP")
	set_weapon_var(weapon, "view_model", create_model("models/v_usp.mdl"))
	set_weapon_var(weapon, "player_model", create_model("models/p_usp.mdl"))
	set_weapon_var(weapon, "world_model", create_model("models/w_usp.mdl"))
	set_weapon_var(weapon, "hud", "weapon_usp")
	set_weapon_var(weapon, "max_clip", 12)
	set_weapon_var(weapon, "max_ammo", 100)
	set_weapon_var(weapon, "slot", PRIMARY_WEAPON_SLOT)
	set_weapon_var(weapon, "weight", USP_WEIGHT)
	set_weapon_var(weapon, "crosshair_size", CrosshairSize_Size8)
	set_weapon_var(weapon, "forward_deploy", "@USP_Deploy")
	set_weapon_var(weapon, "forward_max_speed", "@USP_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@USP_PrimaryAttack")
	set_weapon_var(weapon, "forward_secondary_attack", "@USP_SecondaryAttack")
	set_weapon_var(weapon, "forward_reload", "@USP_Reload")
	set_weapon_var(weapon, "forward_idle", "@USP_Idle")
	precache_sounds(USP_SHOOT_SOUND)
}

@USP_Deploy(weapon, player) {
	set_member(weapon, m_Weapon_flAccuracy, 0.92)
	new anim = (get_member(weapon, m_Weapon_iWeaponState) & WPNSTATE_USP_SILENCED) ? USP_SILENCER_DRAW : USP_DRAW
	return weapon_default_deploy(weapon, player, anim, "onehanded")
}

@USP_MaxSpeed(weapon, player) {
	return USP_MAX_SPEED
}

@USP_PrimaryAttack(weapon, player, clip, ammo) {
	new bool:isSilenced = (get_member(weapon, m_Weapon_iWeaponState) & WPNSTATE_USP_SILENCED) ? true : false
	new Float:accuracy = get_member(weapon, m_Weapon_flAccuracy)
	new Float:spread
	new playerFlags = get_entvar(player, var_flags)
	if (isSilenced) {
		if (!(playerFlags & FL_ONGROUND)) {
			spread = 1.3 * (1.0 - accuracy)
		} else if (playerVelocityLength2d(player) > 0.0) {
			spread = 0.25 * (1.0 - accuracy)
		} else if (playerFlags & FL_DUCKING) {
			spread = 0.125 * (1.0 - accuracy)
		} else {
			spread = 0.15 * (1.0 - accuracy)
		}
	} else {
		if (!(playerFlags & FL_ONGROUND)) {
			spread = 1.2 * (1.0 - accuracy)
		} else if (playerVelocityLength2d(player) > 0.0) {
			spread = 0.225 * (1.0 - accuracy)
		} else if (playerFlags & FL_DUCKING) {
			spread = 0.08 * (1.0 - accuracy)
		} else {
			spread = 0.1 * (1.0 - accuracy)
		}
	}
	new shotsFired = get_member(weapon, m_Weapon_iShotsFired)
	set_member(weapon, m_Weapon_iShotsFired, ++shotsFired)
	if (shotsFired > 1) {
		return
	}
	new Float:lastFire = Float:get_member(weapon, m_Weapon_flLastFire)
	new Float:time = get_gametime()
	if (lastFire != 0.0) {
		accuracy -= (0.3 - (time - lastFire)) * 0.275
		if (accuracy > 0.92) {
			accuracy = 0.92
		} else if (accuracy < 0.6) {
			accuracy = 0.6
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
		1,
		BULLET_PLAYER_45ACP,
		isSilenced ? USP_DAMAGE_SILENCER : USP_DAMAGE,
		USP_RANGE_MODIFER,
		true,
		get_member(player, random_seed)
	)
	if (isSilenced) {
		send_weapon_anim(player, clip ? random_num(USP_SILENCER_SHOOT1, USP_SILENCER_SHOOT3) : USP_SILENCER_SHOOT_EMPTY)
		playFireSound(player, USP_SHOOT_SOUND[random_num(0, 1)])
	} else {
		set_entvar(player, var_effects, get_entvar(player, var_effects) | EF_MUZZLEFLASH)
		send_weapon_anim(player, clip ? random_num(USP_SHOOT1, USP_SHOOT3) : USP_SHOOT_EMPTY)
		playFireSound(player, USP_SHOOT_SOUND[2])
	}
	new Float:nextAttack = 0.225 - 0.075
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 2.0)
	new Float:punchAngle[3]
	get_entvar(player, var_punchangle, punchAngle)
	punchAngle[0] -= 2.0
	set_entvar(player, var_punchangle, punchAngle)
}

@USP_SecondaryAttack(weapon, player, WeaponState:weaponState) {
	if (weaponState & WPNSTATE_USP_SILENCED) {
		set_member(weapon, m_Weapon_iWeaponState, weaponState & ~WPNSTATE_USP_SILENCED)
		send_weapon_anim(player, USP_DETACH_SILENCER)
	} else {
		set_member(weapon, m_Weapon_iWeaponState, weaponState | WPNSTATE_USP_SILENCED)
		send_weapon_anim(player, USP_ATTACH_SILENCER)
	}
	set_member(weapon, m_Weapon_flNextPrimaryAttack, USP_ADJUST_SILENCER_TIME)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, USP_ADJUST_SILENCER_TIME)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, USP_ADJUST_SILENCER_TIME)
}

@USP_Reload(weapon, player) {
	new anim = (get_member(weapon, m_Weapon_iWeaponState) & WPNSTATE_USP_SILENCED) ? USP_SILENCER_RELOAD : USP_RELOAD
	if (!weapon_default_reload(weapon, player, anim, USP_RELOAD_TIME)) {
		return
	}
	rg_set_animation(player, PLAYER_RELOAD)
	set_member(weapon, m_Weapon_flAccuracy, 0.92)
}

@USP_Idle(weapon, player, Float:idleTime) {
	// need rework after grenades
	ExecuteHamB(Ham_Weapon_ResetEmptySound, weapon)
	if (idleTime > 0.0) {
		return
	}
	if (!get_member(weapon, m_Weapon_iClip)) {
		return
	}
	new anim = (get_member(weapon, m_Weapon_iWeaponState) & WPNSTATE_USP_SILENCED) ? USP_SILENCER_IDLE : USP_IDLE
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 60.0)
	send_weapon_anim(player, anim)
}

const P228_MAX_SPEED = 250
const P228_WEIGHT = 5
const P228_DAMAGE = 32
const Float:P228_RANGE_MODIFER = 0.8
const Float:P228_RELOAD_TIME = 2.7
new const P228_SHOOT_SOUND[] = "weapons/p228-1.wav"

enum {
	P228_IDLE,
	P228_SHOOT1,
	P228_SHOOT2,
	P228_SHOOT3,
	P228_SHOOT_EMPTY,
	P228_RELOAD,
	P228_DRAW,
}

createP228() {
	new Weapon:weapon = create_weapon("default_p228", weapon_type_secondary)
	set_weapon_var(weapon, "name", "DEFAULT_P228")
	set_weapon_var(weapon, "view_model", create_model("models/v_p228.mdl"))
	set_weapon_var(weapon, "player_model", create_model("models/p_p228.mdl"))
	set_weapon_var(weapon, "world_model", create_model("models/w_p228.mdl"))
	set_weapon_var(weapon, "hud", "weapon_p228")
	set_weapon_var(weapon, "max_clip", 13)
	set_weapon_var(weapon, "max_ammo", 52)
	set_weapon_var(weapon, "slot", PRIMARY_WEAPON_SLOT)
	set_weapon_var(weapon, "weight", P228_WEIGHT)
	set_weapon_var(weapon, "crosshair_size", CrosshairSize_Size8)
	set_weapon_var(weapon, "forward_deploy", "@P228_Deploy")
	set_weapon_var(weapon, "forward_max_speed", "@P228_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@P228_PrimaryAttack")
	set_weapon_var(weapon, "forward_reload", "@P228_Reload")
	set_weapon_var(weapon, "forward_idle", "@P228_Idle")
	precache_sound(P228_SHOOT_SOUND)
}

@P228_Deploy(weapon, player) {
	set_member(weapon, m_Weapon_flAccuracy, 0.9)
	return weapon_default_deploy(weapon, player, P228_DRAW, "onehanded")
}

@P228_MaxSpeed(weapon, player) {
	return P228_MAX_SPEED
}

@P228_PrimaryAttack(weapon, player, clip, ammo) {
	new Float:accuracy = get_member(weapon, m_Weapon_flAccuracy)
	new Float:spread
	new playerFlags = get_entvar(player, var_flags)
	if (!(playerFlags & FL_ONGROUND)) {
		spread = 1.5 * (1.0 - accuracy)
	} else if (playerVelocityLength2d(player) > 0.0) {
		spread = 0.255 * (1.0 - accuracy)
	} else if (playerFlags & FL_DUCKING) {
		spread = 0.075 * (1.0 - accuracy)
	} else {
		spread = 0.15 * (1.0 - accuracy)
	}
	new shotsFired = get_member(weapon, m_Weapon_iShotsFired)
	set_member(weapon, m_Weapon_iShotsFired, ++shotsFired)
	if (shotsFired > 1) {
		return
	}
	new Float:lastFire = Float:get_member(weapon, m_Weapon_flLastFire)
	new Float:time = get_gametime()
	if (lastFire != 0.0) {
		accuracy -= (0.325 - (time - lastFire)) * 0.3
		if (accuracy > 0.9) {
			accuracy = 0.9
		} else if (accuracy < 0.6) {
			accuracy = 0.6
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
		1,
		BULLET_PLAYER_357SIG,
		P228_DAMAGE,
		P228_RANGE_MODIFER,
		true,
		get_member(player, random_seed)
	)
	send_weapon_anim(player, clip ? random_num(P228_SHOOT1, P228_SHOOT3) : P228_SHOOT_EMPTY)
	playFireSound(player, P228_SHOOT_SOUND)
	new Float:nextAttack = 0.2 - 0.05
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 2.0)
	new Float:punchAngle[3]
	get_entvar(player, var_punchangle, punchAngle)
	punchAngle[0] -= 2.0
	set_entvar(player, var_punchangle, punchAngle)
}

@P228_Reload(weapon, player) {
	if (!weapon_default_reload(weapon, player, P228_RELOAD, P228_RELOAD_TIME)) {
		return
	}
	rg_set_animation(player, PLAYER_RELOAD)
	set_member(weapon, m_Weapon_flAccuracy, 0.9)
}

@P228_Idle(weapon, player, Float:idleTime) {
	// need rework after grenades
	ExecuteHamB(Ham_Weapon_ResetEmptySound, weapon)
	if (idleTime > 0.0) {
		return
	}
	if (!get_member(weapon, m_Weapon_iClip)) {
		return
	}
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 3.0625)
	send_weapon_anim(player, P228_IDLE)
}

const FIVESEVEN_MAX_SPEED = 250
const FIVESEVEN_WEIGHT = 5
const FIVESEVEN_DAMAGE = 20
const Float:FIVESEVEN_RANGE_MODIFER = 0.885
const Float:FIVESEVEN_RELOAD_TIME = 2.7
new const FIVESEVEN_SHOOT_SOUND[] = "weapons/fiveseven-1.wav"

enum {
	FIVESEVEN_IDLE,
	FIVESEVEN_SHOOT1,
	FIVESEVEN_SHOOT2,
	FIVESEVEN_SHOOT_EMPTY,
	FIVESEVEN_RELOAD,
	FIVESEVEN_DRAW,
}

createFiveSeven() {
	new Weapon:weapon = create_weapon("default_fiveseven", weapon_type_secondary)
	set_weapon_var(weapon, "name", "DEFAULT_FIVESEVEN")
	set_weapon_var(weapon, "view_model", create_model("models/v_fiveseven.mdl"))
	set_weapon_var(weapon, "player_model", create_model("models/p_fiveseven.mdl"))
	set_weapon_var(weapon, "world_model", create_model("models/w_fiveseven.mdl"))
	set_weapon_var(weapon, "hud", "weapon_fiveseven")
	set_weapon_var(weapon, "max_clip", 20)
	set_weapon_var(weapon, "max_ammo", 100)
	set_weapon_var(weapon, "slot", PRIMARY_WEAPON_SLOT)
	set_weapon_var(weapon, "weight", FIVESEVEN_WEIGHT)
	set_weapon_var(weapon, "crosshair_size", CrosshairSize_Size8)
	set_weapon_var(weapon, "forward_deploy", "@FiveSeven_Deploy")
	set_weapon_var(weapon, "forward_max_speed", "@FiveSeven_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@FiveSeven_PrimaryAttack")
	set_weapon_var(weapon, "forward_reload", "@FiveSeven_Reload")
	set_weapon_var(weapon, "forward_idle", "@FiveSeven_Idle")
	precache_sound(P228_SHOOT_SOUND)
}

@FiveSeven_Deploy(weapon, player) {
	set_member(weapon, m_Weapon_flAccuracy, 0.92)
	return weapon_default_deploy(weapon, player, FIVESEVEN_DRAW, "onehanded")
}

@FiveSeven_MaxSpeed(weapon, player) {
	return FIVESEVEN_MAX_SPEED
}

@FiveSeven_PrimaryAttack(weapon, player, clip, ammo) {
	new Float:accuracy = get_member(weapon, m_Weapon_flAccuracy)
	new Float:spread
	new playerFlags = get_entvar(player, var_flags)
	if (!(playerFlags & FL_ONGROUND)) {
		spread = 1.5 * (1.0 - accuracy)
	} else if (playerVelocityLength2d(player) > 0.0) {
		spread = 0.255 * (1.0 - accuracy)
	} else if (playerFlags & FL_DUCKING) {
		spread = 0.075 * (1.0 - accuracy)
	} else {
		spread = 0.15 * (1.0 - accuracy)
	}
	new shotsFired = get_member(weapon, m_Weapon_iShotsFired)
	set_member(weapon, m_Weapon_iShotsFired, ++shotsFired)
	if (shotsFired > 1) {
		return
	}
	new Float:lastFire = Float:get_member(weapon, m_Weapon_flLastFire)
	new Float:time = get_gametime()
	if (lastFire != 0.0) {
		accuracy -= (0.275 - (time - lastFire)) * 0.25
		if (accuracy > 0.92) {
			accuracy = 0.92
		} else if (accuracy < 0.725) {
			accuracy = 0.725
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
		1,
		BULLET_PLAYER_57MM,
		FIVESEVEN_DAMAGE,
		FIVESEVEN_RANGE_MODIFER,
		false,
		get_member(player, random_seed)
	)
	send_weapon_anim(player, clip ? random_num(FIVESEVEN_SHOOT1, FIVESEVEN_SHOOT2) : FIVESEVEN_SHOOT_EMPTY)
	playFireSound(player, FIVESEVEN_SHOOT_SOUND)
	new Float:nextAttack = 0.2 - 0.05
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 2.0)
	new Float:punchAngle[3]
	get_entvar(player, var_punchangle, punchAngle)
	punchAngle[0] -= 2.0
	set_entvar(player, var_punchangle, punchAngle)
}

@FiveSeven_Reload(weapon, player) {
	if (!weapon_default_reload(weapon, player, FIVESEVEN_RELOAD, FIVESEVEN_RELOAD_TIME)) {
		return
	}
	rg_set_animation(player, PLAYER_RELOAD)
	set_member(weapon, m_Weapon_flAccuracy, 0.92)
}

@FiveSeven_Idle(weapon, player, Float:idleTime) {
	// need rework after grenades
	ExecuteHamB(Ham_Weapon_ResetEmptySound, weapon)
	if (idleTime > 0.0) {
		return
	}
	if (!get_member(weapon, m_Weapon_iClip)) {
		return
	}
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 3.0625)
	send_weapon_anim(player, FIVESEVEN_IDLE)
}

const DEAGLE_MAX_SPEED = 250
const DEAGLE_WEIGHT = 7
const DEAGLE_DAMAGE = 54
const Float:DEAGLE_RANGE_MODIFER = 0.81
const Float:DEAGLE_RELOAD_TIME = 2.2
new const DEAGLE_SHOOT_SOUND[2][] = {
	"weapons/deagle-1.wav",
	"weapons/deagle-2.wav",
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
	new Weapon:weapon = create_weapon("default_deagle", weapon_type_secondary)
	set_weapon_var(weapon, "name", "DEFAULT_DEAGLE")
	set_weapon_var(weapon, "view_model", create_model("models/v_deagle.mdl"))
	set_weapon_var(weapon, "player_model", create_model("models/p_deagle.mdl"))
	set_weapon_var(weapon, "world_model", create_model("models/w_deagle.mdl"))
	set_weapon_var(weapon, "hud", "weapon_deagle")
	set_weapon_var(weapon, "max_clip", 7)
	set_weapon_var(weapon, "max_ammo", 35)
	set_weapon_var(weapon, "slot", PRIMARY_WEAPON_SLOT)
	set_weapon_var(weapon, "weight", DEAGLE_WEIGHT)
	set_weapon_var(weapon, "crosshair_size", CrosshairSize_Size8)
	set_weapon_var(weapon, "forward_deploy", "@Deagle_Deploy")
	set_weapon_var(weapon, "forward_max_speed", "@Deagle_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@Deagle_PrimaryAttack")
	set_weapon_var(weapon, "forward_reload", "@Deagle_Reload")
	set_weapon_var(weapon, "forward_idle", "@Deagle_Idle")
	precache_sounds(DEAGLE_SHOOT_SOUND)
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
		weapon,
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

const ELITE_MAX_SPEED = 250
const ELITE_WEIGHT = 5
const ELITE_DAMAGE = 36
const Float:ELITE_RANGE_MODIFER = 0.75
const Float:ELITE_RELOAD_TIME = 4.5
new const ELITE_SHOOT_SOUND[] = "weapons/elite_fire.wav"

enum {
	ELITE_IDLE,
	ELITE_IDLE_LEFTEMPTY,
	ELITE_SHOOTLEFT1,
	ELITE_SHOOTLEFT2,
	ELITE_SHOOTLEFT3,
	ELITE_SHOOTLEFT4,
	ELITE_SHOOTLEFT5,
	ELITE_SHOOTLEFTLAST,
	ELITE_SHOOTRIGHT1,
	ELITE_SHOOTRIGHT2,
	ELITE_SHOOTRIGHT3,
	ELITE_SHOOTRIGHT4,
	ELITE_SHOOTRIGHT5,
	ELITE_SHOOTRIGHTLAST,
	ELITE_RELOAD,
	ELITE_DRAW,
}

createElite() {
	new Weapon:weapon = create_weapon("default_elite", weapon_type_secondary)
	set_weapon_var(weapon, "name", "DEFAULT_ELITE")
	set_weapon_var(weapon, "view_model", create_model("models/v_elite.mdl"))
	set_weapon_var(weapon, "player_model", create_model("models/p_elite.mdl"))
	set_weapon_var(weapon, "world_model", create_model("models/w_elite.mdl"))
	set_weapon_var(weapon, "hud", "weapon_elite")
	set_weapon_var(weapon, "max_clip", 30)
	set_weapon_var(weapon, "max_ammo", 120)
	set_weapon_var(weapon, "slot", PRIMARY_WEAPON_SLOT)
	set_weapon_var(weapon, "weight", ELITE_WEIGHT)
	set_weapon_var(weapon, "crosshair_size", CrosshairSize_Size4)
	set_weapon_var(weapon, "forward_deploy", "@Elite_Deploy")
	set_weapon_var(weapon, "forward_max_speed", "@Elite_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@Elite_PrimaryAttack")
	set_weapon_var(weapon, "forward_reload", "@Elite_Reload")
	set_weapon_var(weapon, "forward_idle", "@Elite_Idle")
	precache_sound(ELITE_SHOOT_SOUND)
}

@Elite_Deploy(weapon, player) {
	set_member(weapon, m_Weapon_flAccuracy, 0.88)
	if (!(get_member(weapon, m_Weapon_iClip) & 1)) {
		set_member(weapon, m_Weapon_iWeaponState, WPNSTATE_ELITE_LEFT)
	}
	return weapon_default_deploy(weapon, player, ELITE_DRAW, "dualpistols")
}

@Elite_MaxSpeed(weapon, player) {
	return ELITE_MAX_SPEED
}

@Elite_PrimaryAttack(weapon, player, clip, ammo) {
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
		send_weapon_anim(player, random_num(ELITE_SHOOTLEFT1, ELITE_SHOOTLEFT4))
	} else {
		rg_set_animation(player, PLAYER_ATTACK2)
		set_member(weapon, m_Weapon_iWeaponState, weaponState | WPNSTATE_ELITE_LEFT)
		for (new i = 0; i < sizeof(src); ++i) {
			src[i] += viewRight[i] * 5.0
		}
		send_weapon_anim(player, random_num(ELITE_SHOOTRIGHT1, ELITE_SHOOTRIGHT4))
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
		ELITE_DAMAGE,
		ELITE_RANGE_MODIFER,
		true,
		get_member(player, random_seed)
	)
	playFireSound(player, ELITE_SHOOT_SOUND)
	new Float:nextAttack = 0.2 - 0.078
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 2.0)
	new Float:punchAngle[3]
	get_entvar(player, var_punchangle, punchAngle)
	punchAngle[0] -= 2.0
	set_entvar(player, var_punchangle, punchAngle)
}

@Elite_Reload(weapon, player) {
	if (!weapon_default_reload(weapon, player, ELITE_RELOAD, ELITE_RELOAD_TIME)) {
		return
	}
	rg_set_animation(player, PLAYER_RELOAD)
	set_member(weapon, m_Weapon_flAccuracy, 0.88)
}

@Elite_Idle(weapon, player, Float:idleTime) {
	// need rework after grenades
	ExecuteHamB(Ham_Weapon_ResetEmptySound, weapon)
	if (idleTime > 0.0) {
		return
	}
	new clip = get_member(weapon, m_Weapon_iClip)
	if (!clip) {
		return
	}
	new anim = (clip == 1) ? ELITE_IDLE_LEFTEMPTY : ELITE_IDLE
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 60.0)
	send_weapon_anim(player, anim)
}

const M3_MAX_SPEED = 230
const M3_WEIGHT = 20
const M3_DAMAGE = 20
new Float:M3_CONE_VECTOR[3] = { 0.0675, 0.0675, 0.0 }
new const M3_SHOOT_SOUND[] = "weapons/m3-1.wav"

enum {
	M3_IDLE,
	M3_SHOOT1,
	M3_SHOOT2,
	M3_RELOAD,
	M3_PUMP,
	M3_START_RELOAD,
	M3_DRAW,
	M3_HOLSTER,
}

createM3() {
	new Weapon:weapon = create_weapon("default_m3", weapon_type_primary)
	set_weapon_var(weapon, "name", "DEFAULT_M3")
	set_weapon_var(weapon, "view_model", create_model("models/v_m3.mdl"))
	set_weapon_var(weapon, "player_model", create_model("models/p_m3.mdl"))
	set_weapon_var(weapon, "world_model", create_model("models/w_m3.mdl"))
	set_weapon_var(weapon, "hud", "weapon_m3")
	set_weapon_var(weapon, "max_clip", 8)
	set_weapon_var(weapon, "max_ammo", 32)
	set_weapon_var(weapon, "slot", PRIMARY_WEAPON_SLOT)
	set_weapon_var(weapon, "weight", M3_WEIGHT)
	set_weapon_var(weapon, "flags", ITEM_FLAG_NOFIREUNDERWATER)
	set_weapon_var(weapon, "crosshair_size", CrosshairSize_Size8)
	set_weapon_var(weapon, "forward_deploy", "@M3_Deploy")
	set_weapon_var(weapon, "forward_max_speed", "@M3_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@M3_PrimaryAttack")
	set_weapon_var(weapon, "forward_reload", "@M3_Reload")
	set_weapon_var(weapon, "forward_idle", "@M3_Idle")
	precache_sound(M3_SHOOT_SOUND)
}

@M3_Deploy(weapon, player) {
	return weapon_default_deploy(weapon, player, M3_DRAW, "shotgun")
}

@M3_MaxSpeed(weapon, player) {
	return M3_MAX_SPEED
}

/*
BOOL CM3::PlayEmptySound()
{
	BOOL result = CBasePlayerWeapon::PlayEmptySound();
	m_iPlayEmptySound = 0;
	return result;
}
*/

@M3_PrimaryAttack(weapon, player, clip, ammo) {
	if (isEmptyFire(weapon, clip, true)) {
		return
	}
	set_member(weapon, m_Weapon_iClip, --clip)
	set_member(player, m_iWeaponVolume, LOUD_GUN_VOLUME)
	set_member(player, m_iWeaponFlash, BRIGHT_GUN_FLASH)
	set_member(player, m_flEjectBrass, get_gametime() + 0.45)
	set_entvar(player, var_effects, get_entvar(player, var_effects) | EF_MUZZLEFLASH)
	new Float:aiming[3]
	new Float:src[3]
	getGunPositionAndAiming(player, src, aiming)
	rg_set_animation(player, PLAYER_ATTACK1)
	rg_fire_buckshots(
		weapon,
		player,
		9,
		src,
		aiming,
		M3_CONE_VECTOR,
		3000.0,
		0,
		M3_DAMAGE
	)
	send_weapon_anim(player, random_num(M3_SHOOT1, M3_SHOOT2))
	playFireSound(player, M3_SHOOT_SOUND)
	new Float:nextAttack = 0.875
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, clip ? 2.5 : 0.875)
	set_member(weapon, m_Weapon_fInSpecialReload, 0)
	new Float:punchAngle[3]
	get_entvar(player, var_punchangle, punchAngle)
	if (get_entvar(player, var_flags) & FL_ONGROUND) {
		punchAngle[0] -= random_float(4.0, 6.0)
	} else {
		punchAngle[0] -= random_float(8.0, 11.0)
	}
	set_entvar(player, var_punchangle, punchAngle)
}

@M3_Reload(weapon, player) {
	weapon_default_shotgun_reload(
		weapon,
		player,
		M3_RELOAD,
		M3_START_RELOAD,
		0.45,
		0.55
	)
}

@M3_Idle(weapon, player, Float:idleTime) {
	// need rework after grenades
	ExecuteHamB(Ham_Weapon_ResetEmptySound, weapon)
	if (idleTime > 0.0) {
		return
	}
	//if (m_flTimeWeaponIdle < UTIL_WeaponTimeBase())
	if (Float:get_member(weapon, m_Weapon_flTimeWeaponIdle) > 0.0) {
		return
	}
	new maxClip = rg_get_iteminfo(weapon, ItemInfo_iMaxClip)
	new ammo = get_player_var(player, "ammo", weapon)
	if (!get_member(weapon, m_Weapon_iClip) && !get_member(weapon, m_Weapon_fInSpecialReload) && ammo) {
		ExecuteHamB(Ham_Weapon_Reload, weapon)
	} else if (get_member(weapon, m_Weapon_fInSpecialReload)) {
		if (get_member(weapon, m_Weapon_iClip) != maxClip && ammo) {
			ExecuteHamB(Ham_Weapon_Reload, weapon)
		} else {
			set_member(weapon, m_Weapon_fInSpecialReload, 0)
			set_member(weapon, m_Weapon_flTimeWeaponIdle, 1.5)
			send_weapon_anim(player, M3_PUMP)
		}
	} else {
		send_weapon_anim(player, M3_IDLE)
	}
}

const XM1014_MAX_SPEED = 240
const XM1014_WEIGHT = 20
const XM1014_DAMAGE = 20
new Float:XM1014_CONE_VECTOR[3] = { 0.0725, 0.0725, 0.0 }
new const XM1014_SHOOT_SOUND[] = "weapons/xm1014-1.wav"
new const XM1014_RELOAD_SOUND[2][] = {
	"weapons/reload1.wav",
	"weapons/reload3.wav",
}

enum {
	XM1014_IDLE,
	XM1014_FIRE1,
	XM1014_FIRE2,
	XM1014_RELOAD,
	XM1014_PUMP,
	XM1014_START_RELOAD,
	XM1014_DRAW,
}

createXM1014() {
	new Weapon:weapon = create_weapon("default_xm1014", weapon_type_primary)
	set_weapon_var(weapon, "name", "DEFAULT_XM1014")
	set_weapon_var(weapon, "view_model", create_model("models/v_xm1014.mdl"))
	set_weapon_var(weapon, "player_model", create_model("models/p_xm1014.mdl"))
	set_weapon_var(weapon, "world_model", create_model("models/w_xm1014.mdl"))
	set_weapon_var(weapon, "hud", "weapon_xm1014")
	set_weapon_var(weapon, "max_clip", 7)
	set_weapon_var(weapon, "max_ammo", 32)
	set_weapon_var(weapon, "slot", PRIMARY_WEAPON_SLOT)
	set_weapon_var(weapon, "weight", XM1014_WEIGHT)
	set_weapon_var(weapon, "flags", ITEM_FLAG_NOFIREUNDERWATER)
	set_weapon_var(weapon, "crosshair_size", CrosshairSize_Size9)
	set_weapon_var(weapon, "forward_deploy", "@XM1014_Deploy")
	set_weapon_var(weapon, "forward_max_speed", "@XM1014_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@XM1014_PrimaryAttack")
	set_weapon_var(weapon, "forward_reload", "@XM1014_Reload")
	set_weapon_var(weapon, "forward_idle", "@XM1014_Idle")
	precache_sound(XM1014_SHOOT_SOUND)
}

@XM1014_Deploy(weapon, player) {
	return weapon_default_deploy(weapon, player, XM1014_DRAW, "m249")
}

@XM1014_MaxSpeed(weapon, player) {
	return XM1014_MAX_SPEED
}

/*
BOOL CXM1014::PlayEmptySound()
{
	BOOL result = CBasePlayerWeapon::PlayEmptySound();
	m_iPlayEmptySound = 0;
	return result;
}
*/

@XM1014_PrimaryAttack(weapon, player, clip, ammo) {
	if (isEmptyFire(weapon, clip, true)) {
		return
	}
	set_member(weapon, m_Weapon_iClip, --clip)
	set_member(player, m_iWeaponVolume, LOUD_GUN_VOLUME)
	set_member(player, m_iWeaponFlash, BRIGHT_GUN_FLASH)
	// where is EjectBrass?
	set_entvar(player, var_effects, get_entvar(player, var_effects) | EF_MUZZLEFLASH)
	new Float:aiming[3]
	new Float:src[3]
	getGunPositionAndAiming(player, src, aiming)
	rg_set_animation(player, PLAYER_ATTACK1)
	rg_fire_buckshots(
		weapon,
		player,
		6,
		src,
		aiming,
		XM1014_CONE_VECTOR,
		3048.0,
		0,
		XM1014_DAMAGE
	)
	send_weapon_anim(player, random_num(XM1014_FIRE1, XM1014_FIRE2))
	playFireSound(player, XM1014_SHOOT_SOUND)
	new Float:nextAttack = 0.25
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, clip ? 2.25 : 0.75)
	set_member(weapon, m_Weapon_fInSpecialReload, 0)
	new Float:punchAngle[3]
	get_entvar(player, var_punchangle, punchAngle)
	if (get_entvar(player, var_flags) & FL_ONGROUND) {
		punchAngle[0] -= random_float(3.0, 5.0)
	} else {
		punchAngle[0] -= random_float(7.0, 10.0)
	}
	set_entvar(player, var_punchangle, punchAngle)
}

@XM1014_Reload(weapon, player) {
	weapon_default_shotgun_reload(
		weapon,
		player,
		XM1014_RELOAD,
		XM1014_START_RELOAD,
		0.3,
		0.55,
		XM1014_RELOAD_SOUND[0],
		XM1014_RELOAD_SOUND[1]
	)
}

@XM1014_Idle(weapon, player, Float:idleTime) {
	// need rework after grenades
	ExecuteHamB(Ham_Weapon_ResetEmptySound, weapon)
	if (idleTime > 0.0) {
		return
	}
	//if (m_flTimeWeaponIdle < UTIL_WeaponTimeBase())
	if (Float:get_member(weapon, m_Weapon_flTimeWeaponIdle) > 0.0) {
		return
	}
	new maxClip = rg_get_iteminfo(weapon, ItemInfo_iMaxClip)
	new ammo = get_player_var(player, "ammo", weapon)
	if (!get_member(weapon, m_Weapon_iClip) && !get_member(weapon, m_Weapon_fInSpecialReload) && ammo) {
		ExecuteHamB(Ham_Weapon_Reload, weapon)
	} else if (get_member(weapon, m_Weapon_fInSpecialReload)) {
		if (get_member(weapon, m_Weapon_iClip) != maxClip && ammo) {
			ExecuteHamB(Ham_Weapon_Reload, weapon)
		} else {
			set_member(weapon, m_Weapon_fInSpecialReload, 0)
			set_member(weapon, m_Weapon_flTimeWeaponIdle, 1.5)
			send_weapon_anim(player, XM1014_PUMP)
		}
	} else {
		send_weapon_anim(player, XM1014_IDLE)
	}
}

const MAC10_MAX_SPEED = 250
const MAC10_WEIGHT = 25
const MAC10_DAMAGE = 29
const Float:MAC10_RANGE_MODIFER = 0.82
const Float:MAC10_RELOAD_TIME = 3.15
const Float:MAC10_BASE_ACCURACY = 0.15
const Float:MAC10_ACCURACY_DIVISOR = 200.0
new const MAC10_SHOOT_SOUND[] = "weapons/mac10-1.wav"

enum {
	MAC10_IDLE,
	MAC10_RELOAD,
	MAC10_DRAW,
	MAC10_SHOOT1,
	MAC10_SHOOT2,
	MAC10_SHOOT3,
}

createMAC10() {
	new Weapon:weapon = create_weapon("default_mac10", weapon_type_primary)
	set_weapon_var(weapon, "name", "DEFAULT_MAC10")
	set_weapon_var(weapon, "view_model", create_model("models/v_mac10.mdl"))
	set_weapon_var(weapon, "player_model", create_model("models/p_mac10.mdl"))
	set_weapon_var(weapon, "world_model", create_model("models/w_mac10.mdl"))
	set_weapon_var(weapon, "hud", "weapon_mac10")
	set_weapon_var(weapon, "max_clip", 30)
	set_weapon_var(weapon, "max_ammo", 100)
	set_weapon_var(weapon, "slot", PRIMARY_WEAPON_SLOT)
	set_weapon_var(weapon, "weight", MAC10_WEIGHT)
	set_weapon_var(weapon, "base_accuracy", MAC10_BASE_ACCURACY)
	set_weapon_var(weapon, "crosshair_size", CrosshairSize_Size9)
	set_weapon_var(weapon, "forward_deploy", "@MAC10_Deploy")
	set_weapon_var(weapon, "forward_max_speed", "@MAC10_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@MAC10_PrimaryAttack")
	set_weapon_var(weapon, "forward_reload", "@MAC10_Reload")
	set_weapon_var(weapon, "forward_idle", "@MAC10_Idle")
	precache_sound(MAC10_SHOOT_SOUND)
}

@MAC10_Deploy(weapon, player) {
	set_member(weapon, m_Weapon_bDelayFire, false)
	set_member(weapon, m_Weapon_flAccuracy, MAC10_BASE_ACCURACY)
	return weapon_default_deploy(weapon, player, MAC10_DRAW, "onehanded")
}

@MAC10_MaxSpeed(weapon, player) {
	return MAC10_MAX_SPEED
}

@MAC10_PrimaryAttack(weapon, player, clip, ammo) {
	new Float:accuracy = get_member(weapon, m_Weapon_flAccuracy)
	new Float:spread
	new playerFlags = get_entvar(player, var_flags)
	if (!(playerFlags & FL_ONGROUND)) {
		spread = 0.375 * accuracy
	} else {
		spread = 0.03 * accuracy
	}
	set_member(weapon, m_Weapon_bDelayFire, true)
	new shotsFired = get_member(weapon, m_Weapon_iShotsFired) + 1
	set_member(weapon, m_Weapon_iShotsFired, shotsFired)
	accuracy = (shotsFired * shotsFired * shotsFired / MAC10_ACCURACY_DIVISOR) + 0.6
	if (accuracy > 1.65) {
		accuracy = 1.65
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
		weapon,
		player,
		src,
		aiming,
		spread,
		8192.0,
		1,
		BULLET_PLAYER_45ACP,
		MAC10_DAMAGE,
		MAC10_RANGE_MODIFER,
		false,
		get_member(player, random_seed)
	)
	send_weapon_anim(player, random_num(MAC10_SHOOT1, MAC10_SHOOT3))
	playFireSound(player, MAC10_SHOOT_SOUND)
	new Float:nextAttack = 0.07
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 2.0)
	if (!(playerFlags & FL_ONGROUND)) {
		weapon_kick_back(weapon, player, 1.3, 0.55, 0.4, 0.05, 4.75, 3.75, 5)
	} else if (playerVelocityLength2d(player) > 0.0) {
		weapon_kick_back(weapon, player, 0.9, 0.45, 0.25, 0.035, 3.5, 2.75, 7)
	} else if (playerFlags & FL_DUCKING) {
		weapon_kick_back(weapon, player, 0.75, 0.4, 0.175, 0.03, 2.75, 2.5, 10)
	} else {
		weapon_kick_back(weapon, player, 0.775, 0.425, 0.2, 0.03, 3.0, 2.75, 9)
	}
}

@MAC10_Reload(weapon, player) {
	if (!weapon_default_reload(weapon, player, MAC10_RELOAD, MAC10_RELOAD_TIME)) {
		return
	}
	rg_set_animation(player, PLAYER_RELOAD)
	set_member(weapon, m_Weapon_iShotsFired, 0)
	set_member(weapon, m_Weapon_flAccuracy, 0.0) // why not 0.15?
}

@MAC10_Idle(weapon, player, Float:idleTime) {
	// need rework after grenades
	ExecuteHamB(Ham_Weapon_ResetEmptySound, weapon)
	if (idleTime > 0.0) {
		return
	}
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 20.0)
	send_weapon_anim(player, MAC10_IDLE)
}

const TMP_MAX_SPEED = 250
const TMP_WEIGHT = 25
const TMP_DAMAGE = 20
const Float:TMP_RANGE_MODIFER = 0.85
const Float:TMP_RELOAD_TIME = 2.12
const Float:TMP_BASE_ACCURACY = 0.2
const Float:TMP_ACCURACY_DIVISOR = 200.0
new const TMP_SHOOT_SOUND[2][] = {
	"weapons/tmp-1.wav",
	"weapons/tmp-2.wav",
}

enum {
	TMP_IDLE,
	TMP_RELOAD,
	TMP_DRAW,
	TMP_SHOOT1,
	TMP_SHOOT2,
	TMP_SHOOT3,
}

createTMP() {
	new Weapon:weapon = create_weapon("default_tmp", weapon_type_primary)
	set_weapon_var(weapon, "name", "DEFAULT_TMP")
	set_weapon_var(weapon, "view_model", create_model("models/v_tmp.mdl"))
	set_weapon_var(weapon, "player_model", create_model("models/p_tmp.mdl"))
	set_weapon_var(weapon, "world_model", create_model("models/w_tmp.mdl"))
	set_weapon_var(weapon, "hud", "weapon_tmp")
	set_weapon_var(weapon, "max_clip", 30)
	set_weapon_var(weapon, "max_ammo", 120)
	set_weapon_var(weapon, "slot", PRIMARY_WEAPON_SLOT)
	set_weapon_var(weapon, "weight", TMP_WEIGHT)
	set_weapon_var(weapon, "base_accuracy", TMP_BASE_ACCURACY)
	set_weapon_var(weapon, "crosshair_size", CrosshairSize_Size7)
	set_weapon_var(weapon, "forward_deploy", "@TMP_Deploy")
	set_weapon_var(weapon, "forward_max_speed", "@TMP_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@TMP_PrimaryAttack")
	set_weapon_var(weapon, "forward_reload", "@TMP_Reload")
	set_weapon_var(weapon, "forward_idle", "@TMP_Idle")
	precache_sounds(TMP_SHOOT_SOUND)
}

@TMP_Deploy(weapon, player) {
	set_member(weapon, m_Weapon_bDelayFire, false)
	set_member(weapon, m_Weapon_iShotsFired, 0)
	set_member(weapon, m_Weapon_flAccuracy, TMP_BASE_ACCURACY)
	return weapon_default_deploy(weapon, player, TMP_DRAW, "onehanded")
}

@TMP_MaxSpeed(weapon, player) {
	return TMP_MAX_SPEED
}

@TMP_PrimaryAttack(weapon, player, clip, ammo) {
	new Float:accuracy = get_member(weapon, m_Weapon_flAccuracy)
	new Float:spread
	new playerFlags = get_entvar(player, var_flags)
	if (!(playerFlags & FL_ONGROUND)) {
		spread = 0.25 * accuracy
	} else {
		spread = 0.03 * accuracy
	}
	set_member(weapon, m_Weapon_bDelayFire, true)
	new shotsFired = get_member(weapon, m_Weapon_iShotsFired) + 1
	set_member(weapon, m_Weapon_iShotsFired, shotsFired)
	accuracy = (shotsFired * shotsFired * shotsFired / TMP_ACCURACY_DIVISOR) + 0.55
	if (accuracy > 1.4) {
		accuracy = 1.4
	}
	set_member(weapon, m_Weapon_flAccuracy, accuracy)
	if (isEmptyFire(weapon, clip)) {
		return
	}
	set_member(weapon, m_Weapon_iClip, --clip)
	set_member(player, m_iWeaponVolume, NORMAL_GUN_VOLUME)
	//set_member(player, m_iWeaponFlash, DIM_GUN_FLASH)
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
		8192.0,
		1,
		BULLET_PLAYER_9MM,
		TMP_DAMAGE,
		TMP_RANGE_MODIFER,
		false,
		get_member(player, random_seed)
	)
	send_weapon_anim(player, random_num(TMP_SHOOT1, TMP_SHOOT3))
	playFireSound(player, TMP_SHOOT_SOUND[random(sizeof(TMP_SHOOT_SOUND))])
	new Float:nextAttack = 0.07
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 2.0)
	if (!(playerFlags & FL_ONGROUND)) {
		weapon_kick_back(weapon, player, 1.1, 0.5, 0.35, 0.045, 4.5, 3.5, 6)
	} else if (playerVelocityLength2d(player) > 0.0) {
		weapon_kick_back(weapon, player, 0.8, 0.4, 0.2, 0.03, 3.0, 2.5, 7)
	} else if (playerFlags & FL_DUCKING) {
		weapon_kick_back(weapon, player, 0.7, 0.35, 0.125, 0.025, 2.5, 2.0, 10)
	} else {
		weapon_kick_back(weapon, player, 0.725, 0.375, 0.15, 0.025, 2.75, 2.25, 9)
	}
}

@TMP_Reload(weapon, player) {
	if (!weapon_default_reload(weapon, player, TMP_RELOAD, TMP_RELOAD_TIME)) {
		return
	}
	rg_set_animation(player, PLAYER_RELOAD)
	set_member(weapon, m_Weapon_iShotsFired, 0)
	set_member(weapon, m_Weapon_flAccuracy, TMP_BASE_ACCURACY)
}

@TMP_Idle(weapon, player, Float:idleTime) {
	// need rework after grenades
	ExecuteHamB(Ham_Weapon_ResetEmptySound, weapon)
	if (idleTime > 0.0) {
		return
	}
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 20.0)
	send_weapon_anim(player, TMP_IDLE)
}

const MP5_MAX_SPEED = 250
const MP5_WEIGHT = 25
const MP5_DAMAGE = 26
const Float:MP5_RANGE_MODIFER = 0.84
const Float:MP5_RELOAD_TIME = 2.63
const Float:MP5_BASE_ACCURACY = 0.0
const Float:MP5_ACCURACY_DIVISOR = 220.1
new const MP5_SHOOT_SOUND[2][] = {
	"weapons/mp5-1.wav",
	"weapons/mp5-2.wav",
}

enum {
	MP5_IDLE,
	MP5_RELOAD,
	MP5_DRAW,
	MP5_SHOOT1,
	MP5_SHOOT2,
	MP5_SHOOT3,
}

createMP5() {
	new Weapon:weapon = create_weapon("default_mp5navy", weapon_type_primary)
	set_weapon_var(weapon, "name", "DEFAULT_MP5")
	set_weapon_var(weapon, "view_model", create_model("models/v_mp5.mdl"))
	set_weapon_var(weapon, "player_model", create_model("models/p_mp5.mdl"))
	set_weapon_var(weapon, "world_model", create_model("models/w_mp5.mdl"))
	set_weapon_var(weapon, "hud", "weapon_mp5navy")
	set_weapon_var(weapon, "max_clip", 30)
	set_weapon_var(weapon, "max_ammo", 120)
	set_weapon_var(weapon, "slot", PRIMARY_WEAPON_SLOT)
	set_weapon_var(weapon, "weight", MP5_WEIGHT)
	set_weapon_var(weapon, "base_accuracy", MP5_BASE_ACCURACY)
	set_weapon_var(weapon, "crosshair_size", CrosshairSize_Size6)
	set_weapon_var(weapon, "forward_deploy", "@MP5_Deploy")
	set_weapon_var(weapon, "forward_max_speed", "@MP5_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@MP5_PrimaryAttack")
	set_weapon_var(weapon, "forward_reload", "@MP5_Reload")
	set_weapon_var(weapon, "forward_idle", "@MP5_Idle")
	precache_sounds(MP5_SHOOT_SOUND)
}

@MP5_Deploy(weapon, player) {
	set_member(weapon, m_Weapon_bDelayFire, false)
	set_member(weapon, m_Weapon_flAccuracy, MP5_BASE_ACCURACY)
	return weapon_default_deploy(weapon, player, MP5_DRAW, "mp5")
}

@MP5_MaxSpeed(weapon, player) {
	return MP5_MAX_SPEED
}

@MP5_PrimaryAttack(weapon, player, clip, ammo) {
	new Float:accuracy = get_member(weapon, m_Weapon_flAccuracy)
	new Float:spread
	new playerFlags = get_entvar(player, var_flags)
	if (!(playerFlags & FL_ONGROUND)) {
		spread = 0.2 * accuracy
	} else {
		spread = 0.04 * accuracy
	}
	set_member(weapon, m_Weapon_bDelayFire, true)
	new shotsFired = get_member(weapon, m_Weapon_iShotsFired) + 1
	set_member(weapon, m_Weapon_iShotsFired, shotsFired)
	accuracy = (shotsFired * shotsFired / MP5_ACCURACY_DIVISOR) + 0.45
	if (accuracy > 0.75) {
		accuracy = 0.75
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
		weapon,
		player,
		src,
		aiming,
		spread,
		8192.0,
		1,
		BULLET_PLAYER_9MM,
		MP5_DAMAGE,
		MP5_RANGE_MODIFER,
		false,
		get_member(player, random_seed)
	)
	send_weapon_anim(player, random_num(MP5_SHOOT1, MP5_SHOOT3))
	playFireSound(player, MP5_SHOOT_SOUND[random(sizeof(MP5_SHOOT_SOUND))])
	new Float:nextAttack = 0.075
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 2.0)
	if (!(playerFlags & FL_ONGROUND)) {
		weapon_kick_back(weapon, player, 0.9, 0.475, 0.35, 0.0425, 5.0, 3.0, 6)
	} else if (playerVelocityLength2d(player) > 0.0) {
		weapon_kick_back(weapon, player, 0.5, 0.275, 0.2, 0.03, 3.0, 2.0, 10)
	} else if (playerFlags & FL_DUCKING) {
		weapon_kick_back(weapon, player, 0.225, 0.15, 0.1, 0.015, 2.0, 1.0, 10)
	} else {
		weapon_kick_back(weapon, player, 0.25, 0.175, 0.125, 0.02, 2.25, 1.25, 10)
	}
}

@MP5_Reload(weapon, player) {
	if (!weapon_default_reload(weapon, player, MP5_RELOAD, MP5_RELOAD_TIME)) {
		return
	}
	rg_set_animation(player, PLAYER_RELOAD)
	set_member(weapon, m_Weapon_iShotsFired, 0)
	set_member(weapon, m_Weapon_flAccuracy, MP5_BASE_ACCURACY)
}

@MP5_Idle(weapon, player, Float:idleTime) {
	// need rework after grenades
	ExecuteHamB(Ham_Weapon_ResetEmptySound, weapon)
	if (idleTime > 0.0) {
		return
	}
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 20.0)
	send_weapon_anim(player, MP5_IDLE)
}

const UMP45_MAX_SPEED = 250
const UMP45_WEIGHT = 25
const UMP45_DAMAGE = 30
const Float:UMP45_RANGE_MODIFER = 0.82
const Float:UMP45_RELOAD_TIME = 3.5
const Float:UMP45_BASE_ACCURACY = 0.0
const Float:UMP45_ACCURACY_DIVISOR = 210.0
new const UMP45_SHOOT_SOUND[] = "weapons/ump45-1.wav"

enum {
	UMP45_IDLE,
	UMP45_RELOAD,
	UMP45_DRAW,
	UMP45_SHOOT1,
	UMP45_SHOOT2,
	UMP45_SHOOT3,
}

createUMP45() {
	new Weapon:weapon = create_weapon("default_ump45", weapon_type_primary)
	set_weapon_var(weapon, "name", "DEFAULT_UMP45")
	set_weapon_var(weapon, "view_model", create_model("models/v_ump45.mdl"))
	set_weapon_var(weapon, "player_model", create_model("models/p_ump45.mdl"))
	set_weapon_var(weapon, "world_model", create_model("models/w_ump45.mdl"))
	set_weapon_var(weapon, "hud", "weapon_ump45")
	set_weapon_var(weapon, "max_clip", 25)
	set_weapon_var(weapon, "max_ammo", 100)
	set_weapon_var(weapon, "slot", PRIMARY_WEAPON_SLOT)
	set_weapon_var(weapon, "weight", UMP45_WEIGHT)
	set_weapon_var(weapon, "base_accuracy", UMP45_BASE_ACCURACY)
	set_weapon_var(weapon, "crosshair_size", CrosshairSize_Size6)
	set_weapon_var(weapon, "forward_deploy", "@UMP45_Deploy")
	set_weapon_var(weapon, "forward_max_speed", "@UMP45_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@UMP45_PrimaryAttack")
	set_weapon_var(weapon, "forward_reload", "@UMP45_Reload")
	set_weapon_var(weapon, "forward_idle", "@UMP45_Idle")
	precache_sound(UMP45_SHOOT_SOUND)
}

@UMP45_Deploy(weapon, player) {
	set_member(weapon, m_Weapon_bDelayFire, false)
	set_member(weapon, m_Weapon_flAccuracy, UMP45_BASE_ACCURACY)
	return weapon_default_deploy(weapon, player, UMP45_DRAW, "carbine")
}

@UMP45_MaxSpeed(weapon, player) {
	return UMP45_MAX_SPEED
}

@UMP45_PrimaryAttack(weapon, player, clip, ammo) {
	new Float:accuracy = get_member(weapon, m_Weapon_flAccuracy)
	new Float:spread
	new playerFlags = get_entvar(player, var_flags)
	if (!(playerFlags & FL_ONGROUND)) {
		spread = 0.24 * accuracy
	} else {
		spread = 0.04 * accuracy
	}
	set_member(weapon, m_Weapon_bDelayFire, true)
	new shotsFired = get_member(weapon, m_Weapon_iShotsFired) + 1
	set_member(weapon, m_Weapon_iShotsFired, shotsFired)
	accuracy = (shotsFired * shotsFired / UMP45_ACCURACY_DIVISOR) + 0.5
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
		weapon,
		player,
		src,
		aiming,
		spread,
		8192.0,
		1,
		BULLET_PLAYER_45ACP,
		UMP45_DAMAGE,
		UMP45_RANGE_MODIFER,
		false,
		get_member(player, random_seed)
	)
	send_weapon_anim(player, random_num(UMP45_SHOOT1, UMP45_SHOOT3))
	playFireSound(player, UMP45_SHOOT_SOUND)
	new Float:nextAttack = 0.1
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 2.0)
	if (!(playerFlags & FL_ONGROUND)) {
		weapon_kick_back(weapon, player, 0.125, 0.65, 0.55, 0.0475, 5.5, 4.0, 10)
	} else if (playerVelocityLength2d(player) > 0.0) {
		weapon_kick_back(weapon, player, 0.55, 0.3, 0.225, 0.03, 3.5, 2.5, 10)
	} else if (playerFlags & FL_DUCKING) {
		weapon_kick_back(weapon, player, 0.25, 0.175, 0.125, 0.02, 2.25, 1.25, 10)
	} else {
		weapon_kick_back(weapon, player, 0.275, 0.2, 0.15, 0.0225, 2.5, 1.5, 10)
	}
}

@UMP45_Reload(weapon, player) {
	if (!weapon_default_reload(weapon, player, UMP45_RELOAD, UMP45_RELOAD_TIME)) {
		return
	}
	rg_set_animation(player, PLAYER_RELOAD)
	set_member(weapon, m_Weapon_iShotsFired, 0)
	set_member(weapon, m_Weapon_flAccuracy, UMP45_BASE_ACCURACY)
}

@UMP45_Idle(weapon, player, Float:idleTime) {
	// need rework after grenades
	ExecuteHamB(Ham_Weapon_ResetEmptySound, weapon)
	if (idleTime > 0.0) {
		return
	}
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 20.0)
	send_weapon_anim(player, UMP45_IDLE)
}

const P90_MAX_SPEED = 245
const P90_WEIGHT = 26
const P90_DAMAGE = 21
const Float:P90_RANGE_MODIFER = 0.885
const Float:P90_RELOAD_TIME = 3.4
const Float:P90_BASE_ACCURACY = 0.2
const Float:P90_ACCURACY_DIVISOR = 175.0
new const P90_SHOOT_SOUND[] = "weapons/p90-1.wav"

enum {
	P90_IDLE,
	P90_RELOAD,
	P90_DRAW,
	P90_SHOOT1,
	P90_SHOOT2,
	P90_SHOOT3,
}

createP90() {
	new Weapon:weapon = create_weapon("default_p90", weapon_type_primary)
	set_weapon_var(weapon, "name", "DEFAULT_P90")
	set_weapon_var(weapon, "view_model", create_model("models/v_p90.mdl"))
	set_weapon_var(weapon, "player_model", create_model("models/p_p90.mdl"))
	set_weapon_var(weapon, "world_model", create_model("models/w_p90.mdl"))
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
		weapon,
		player,
		src,
		aiming,
		spread,
		8192.0,
		1,
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

const GALIL_MAX_SPEED = 240
const GALIL_WEIGHT = 25
const GALIL_DAMAGE = 30
const Float:GALIL_RANGE_MODIFER = 0.98
const Float:GALIL_RELOAD_TIME = 2.45
const Float:GALIL_BASE_ACCURACY = 0.2
const Float:GALIL_ACCURACY_DIVISOR = 200.0
new const GALIL_SHOOT_SOUND[2][] = {
	"weapons/galil-1.wav",
	"weapons/galil-2.wav",
}

enum {
	GALIL_IDLE,
	GALIL_RELOAD,
	GALIL_DRAW,
	GALIL_SHOOT1,
	GALIL_SHOOT2,
	GALIL_SHOOT3,
}

createGalil() {
	new Weapon:weapon = create_weapon("default_galil", weapon_type_primary)
	set_weapon_var(weapon, "name", "DEFAULT_GALIL")
	set_weapon_var(weapon, "view_model", create_model("models/v_galil.mdl"))
	set_weapon_var(weapon, "player_model", create_model("models/p_galil.mdl"))
	set_weapon_var(weapon, "world_model", create_model("models/w_galil.mdl"))
	set_weapon_var(weapon, "hud", "weapon_galil")
	set_weapon_var(weapon, "max_clip", 35)
	set_weapon_var(weapon, "max_ammo", 90)
	set_weapon_var(weapon, "slot", PRIMARY_WEAPON_SLOT)
	set_weapon_var(weapon, "weight", GALIL_WEIGHT)
	set_weapon_var(weapon, "flags", ITEM_FLAG_NOFIREUNDERWATER)
	set_weapon_var(weapon, "base_accuracy", GALIL_BASE_ACCURACY)
	set_weapon_var(weapon, "crosshair_size", CrosshairSize_Size4)
	set_weapon_var(weapon, "forward_deploy", "@Galil_Deploy")
	set_weapon_var(weapon, "forward_max_speed", "@Galil_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@Galil_PrimaryAttack")
	set_weapon_var(weapon, "forward_reload", "@Galil_Reload")
	set_weapon_var(weapon, "forward_idle", "@Galil_Idle")
	precache_sounds(GALIL_SHOOT_SOUND)
}

@Galil_Deploy(weapon, player) {
	if (!weapon_default_deploy(weapon, player, GALIL_DRAW, "ak47")) {
		return false
	}
	set_member(weapon, m_Weapon_iShotsFired, 0)
	set_member(weapon, m_Weapon_flAccuracy, GALIL_BASE_ACCURACY)
	return true
}

@Galil_MaxSpeed(weapon, player) {
	return GALIL_MAX_SPEED
}

@Galil_PrimaryAttack(weapon, player, clip, ammo) {
	new Float:accuracy = get_member(weapon, m_Weapon_flAccuracy)
	new Float:spread
	new playerFlags = get_entvar(player, var_flags)
	new Float:playerVelocityLength = playerVelocityLength2d(player)
	if (!(playerFlags & FL_ONGROUND)) {
		spread = 0.04 + (0.3 * accuracy)
	} else if (playerVelocityLength > 140.0) {
		spread = 0.04 + (0.07 * accuracy)
	} else {
		spread = 0.0375 * accuracy
	}
	set_member(weapon, m_Weapon_bDelayFire, true)
	new shotsFired = get_member(weapon, m_Weapon_iShotsFired) + 1
	set_member(weapon, m_Weapon_iShotsFired, shotsFired)
	accuracy = ((shotsFired * shotsFired * shotsFired) / GALIL_ACCURACY_DIVISOR) + 0.35
	if (accuracy > 1.25) {
		accuracy = 1.25
	}
	set_member(weapon, m_Weapon_flAccuracy, accuracy)
	if (isEmptyFire(weapon, clip)) {
		return
	}
	set_member(weapon, m_Weapon_iClip, --clip)
	set_member(player, m_iWeaponVolume, NORMAL_GUN_VOLUME)
	set_member(player, m_iWeaponFlash, BRIGHT_GUN_FLASH)
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
		8192.0,
		2,
		BULLET_PLAYER_556MM,
		GALIL_DAMAGE,
		GALIL_RANGE_MODIFER,
		false,
		get_member(player, random_seed)
	)
	send_weapon_anim(player, random_num(GALIL_SHOOT1, GALIL_SHOOT3))
	playFireSound(player, GALIL_SHOOT_SOUND[random(sizeof(GALIL_SHOOT_SOUND))])
	new Float:nextAttack = 0.0875
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 1.28)
	if (playerVelocityLength > 0.0) { // why it first
		weapon_kick_back(weapon, player, 1.0, 0.45, 0.28, 0.045, 3.75, 3.0, 7)
	} else if (!(playerFlags & FL_ONGROUND)) {
		weapon_kick_back(weapon, player, 1.2, 0.5, 0.23, 0.15, 5.5, 3.5, 6)
	} else if (playerFlags & FL_DUCKING) {
		weapon_kick_back(weapon, player, 0.6, 0.3, 0.2, 0.0125, 3.25, 2.0, 7)
	} else {
		weapon_kick_back(weapon, player, 0.65, 0.35, 0.25, 0.015, 3.5, 2.25, 7)
	}
}

@Galil_Reload(weapon, player) {
	if (!weapon_default_reload(weapon, player, GALIL_RELOAD, GALIL_RELOAD_TIME)) {
		return
	}
	rg_set_animation(player, PLAYER_RELOAD)
	set_member(weapon, m_Weapon_bDelayFire, false)
	set_member(weapon, m_Weapon_flAccuracy, GALIL_BASE_ACCURACY)
	set_member(weapon, m_Weapon_iShotsFired, 0)
}

@Galil_Idle(weapon, player, Float:idleTime) {
	// need rework after grenades
	ExecuteHamB(Ham_Weapon_ResetEmptySound, weapon)
	if (idleTime > 0.0) {
		return
	}
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 20.0)
	send_weapon_anim(player, GALIL_IDLE)
}

const FAMAS_MAX_SPEED = 240
const FAMAS_WEIGHT = 75 // lol wtf so high
const FAMAS_DAMAGE = 30
const FAMAS_DAMAGE_BURST = 34
const Float:FAMAS_RANGE_MODIFER = 0.96
const Float:FAMAS_RELOAD_TIME = 3.3
const Float:FAMAS_BASE_ACCURACY = 0.2
const Float:FAMAS_ACCURACY_DIVISOR = 215.0
new const FAMAS_SHOOT_SOUND[2][] = {
	"weapons/famas-1.wav",
	"weapons/famas-2.wav",
}

enum {
	FAMAS_IDLE,
	FAMAS_RELOAD,
	FAMAS_DRAW,
	FAMAS_SHOOT1,
	FAMAS_SHOOT2,
	FAMAS_SHOOT3,
}

createFamas() {
	new Weapon:weapon = create_weapon("default_famas", weapon_type_primary)
	set_weapon_var(weapon, "name", "DEFAULT_FAMAS")
	set_weapon_var(weapon, "view_model", create_model("models/v_famas.mdl"))
	set_weapon_var(weapon, "player_model", create_model("models/p_famas.mdl"))
	set_weapon_var(weapon, "world_model", create_model("models/w_famas.mdl"))
	set_weapon_var(weapon, "hud", "weapon_famas")
	set_weapon_var(weapon, "max_clip", 25)
	set_weapon_var(weapon, "max_ammo", 90)
	set_weapon_var(weapon, "slot", PRIMARY_WEAPON_SLOT)
	set_weapon_var(weapon, "weight", FAMAS_WEIGHT)
	set_weapon_var(weapon, "flags", ITEM_FLAG_NOFIREUNDERWATER)
	set_weapon_var(weapon, "base_accuracy", FAMAS_BASE_ACCURACY)
	set_weapon_var(weapon, "crosshair_size", CrosshairSize_Size4)
	set_weapon_var(weapon, "forward_deploy", "@Famas_Deploy")
	set_weapon_var(weapon, "forward_max_speed", "@Famas_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@Famas_PrimaryAttack")
	set_weapon_var(weapon, "forward_fire_remaining", "@Famas_FireRemaining")
	set_weapon_var(weapon, "forward_secondary_attack", "@Famas_SecondaryAttack")
	set_weapon_var(weapon, "forward_reload", "@Famas_Reload")
	set_weapon_var(weapon, "forward_idle", "@Famas_Idle")
	precache_sounds(FAMAS_SHOOT_SOUND)
}

@Famas_Deploy(weapon, player) {
	if (!weapon_default_deploy(weapon, player, FAMAS_DRAW, "carbine")) {
		return false
	}
	set_member(weapon, m_Weapon_iShotsFired, 0)
	set_member(weapon, m_Weapon_flAccuracy, FAMAS_BASE_ACCURACY)
	set_member(weapon, m_Weapon_iRemainingShotsFired, 0)
	set_member(weapon, m_Weapon_flNextRemainingShoot, 0.0)
	return true
}

@Famas_MaxSpeed(weapon, player) {
	return FAMAS_MAX_SPEED
}

@Famas_PrimaryAttack(weapon, player, clip, ammo) {
	new bool:isBurstMode = (get_member(weapon, m_Weapon_iWeaponState) & WPNSTATE_FAMAS_BURST_MODE) ? true : false
	new Float:nextAttack = isBurstMode ? 0.55 : 0.0825
	new Float:accuracy = get_member(weapon, m_Weapon_flAccuracy)
	new Float:spread
	new playerFlags = get_entvar(player, var_flags)
	new Float:playerVelocityLength = playerVelocityLength2d(player)
	if (!(playerFlags & FL_ONGROUND)) {
		spread = 0.03 + (0.3 * accuracy)
	} else if (playerVelocityLength > 140.0) {
		spread = 0.03 + (0.07 * accuracy)
	} else {
		spread = 0.02 * accuracy
	}
	if (!isBurstMode) {
		spread += 0.01
	}
	set_member(weapon, m_Weapon_bDelayFire, true)
	new shotsFired = get_member(weapon, m_Weapon_iShotsFired) + 1
	set_member(weapon, m_Weapon_iShotsFired, shotsFired)
	accuracy = ((shotsFired * shotsFired * shotsFired) / FAMAS_ACCURACY_DIVISOR) + 0.3
	if (accuracy > 1.0) {
		accuracy = 1.0
	}
	set_member(weapon, m_Weapon_flAccuracy, accuracy)
	if (isEmptyFire(weapon, clip)) {
		return
	}
	set_member(weapon, m_Weapon_iClip, --clip)
	set_member(player, m_iWeaponVolume, NORMAL_GUN_VOLUME)
	set_member(player, m_iWeaponFlash, BRIGHT_GUN_FLASH)
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
		8192.0,
		2,
		BULLET_PLAYER_556MM,
		isBurstMode ? FAMAS_DAMAGE_BURST : FAMAS_DAMAGE,
		FAMAS_RANGE_MODIFER,
		false,
		get_member(player, random_seed)
	)
	send_weapon_anim(player, random_num(FAMAS_SHOOT1, FAMAS_SHOOT3))
	playFireSound(player, FAMAS_SHOOT_SOUND[random(sizeof(FAMAS_SHOOT_SOUND))])
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 1.1)
	if (playerVelocityLength > 0.0) { // why it first
		weapon_kick_back(weapon, player, 1.0, 0.45, 0.275, 0.05, 4.0, 2.5, 7)
	} else if (!(playerFlags & FL_ONGROUND)) {
		weapon_kick_back(weapon, player, 1.25, 0.45, 0.22, 0.18, 5.5, 4.0, 5)
	} else if (playerFlags & FL_DUCKING) {
		weapon_kick_back(weapon, player, 0.575, 0.325, 0.2, 0.011, 3.25, 2.0, 8)
	} else {
		weapon_kick_back(weapon, player, 0.625, 0.375, 0.25, 0.0125, 3.5, 2.25, 8)
	}
	if (isBurstMode) {
		set_member(weapon, m_Weapon_iRemainingShotsFired, 1)
		set_member(weapon, m_Weapon_flNextRemainingShoot, get_gametime() + 0.05)
		set_member(weapon, m_Weapon_fBurstSpread, spread)
	}
}

@Famas_FireRemaining(weapon, player, clip, shotsFired) {
	// refactoring from 1 to 3
	if (--clip < 0) {
		set_member(weapon, m_Weapon_iClip, 0)
		set_member(weapon, m_Weapon_iRemainingShotsFired, 3)
		set_member(weapon, m_Weapon_flNextRemainingShoot, 0.0)
		return
	}
	set_member(weapon, m_Weapon_iClip, clip)
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
		Float:get_member(weapon, m_Weapon_fBurstSpread),
		8192.0,
		2,
		BULLET_PLAYER_556MM,
		FAMAS_DAMAGE,
		FAMAS_RANGE_MODIFER,
		false,
		get_member(player, random_seed)
	)
	send_weapon_anim(player, random_num(FAMAS_SHOOT1, FAMAS_SHOOT3))
	playFireSound(player, FAMAS_SHOOT_SOUND[random(sizeof(FAMAS_SHOOT_SOUND))])
	set_member(weapon, m_Weapon_iRemainingShotsFired, ++shotsFired)
	if (shotsFired != 3) {
		set_member(weapon, m_Weapon_flNextRemainingShoot, get_gametime() + 0.1)
	} else {
		set_member(weapon, m_Weapon_flNextRemainingShoot, 0.0)
	}
}

@Famas_SecondaryAttack(weapon, player, WeaponState:weaponState) {
	if (weaponState & WPNSTATE_FAMAS_BURST_MODE) {
		set_member(weapon, m_Weapon_iWeaponState, weaponState & ~WPNSTATE_FAMAS_BURST_MODE)
		client_print(player, print_center, "#Cstrike_TitlesTXT_Switch_To_FullAuto")
	} else {
		set_member(weapon, m_Weapon_iWeaponState, weaponState | WPNSTATE_FAMAS_BURST_MODE)
		client_print(player, print_center, "#Cstrike_TitlesTXT_Switch_To_BurstFire")
	}
	set_member(weapon, m_Weapon_flNextSecondaryAttack, 0.3)
}

@Famas_Reload(weapon, player) {
	if (!weapon_default_reload(weapon, player, FAMAS_RELOAD, FAMAS_RELOAD_TIME)) {
		return
	}
	rg_set_animation(player, PLAYER_RELOAD)
	set_member(weapon, m_Weapon_bDelayFire, false)
	set_member(weapon, m_Weapon_flAccuracy, 0.0) // why 0.0?
	set_member(weapon, m_Weapon_iShotsFired, 0)
}

@Famas_Idle(weapon, player, Float:idleTime) {
	// need rework after grenades
	ExecuteHamB(Ham_Weapon_ResetEmptySound, weapon)
	if (idleTime > 0.0) {
		return
	}
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 20.0)
	send_weapon_anim(player, FAMAS_IDLE)
}

const AK47_MAX_SPEED = 221
const AK47_WEIGHT = 25
const AK47_DAMAGE = 36
const Float:AK47_RANGE_MODIFER = 0.98
const Float:AK47_RELOAD_TIME = 2.45
const Float:AK47_BASE_ACCURACY = 0.2
const Float:AK47_ACCURACY_DIVISOR = 200.0
new const AK47_SHOOT_SOUND[2][] = {
	"weapons/ak47-1.wav",
	"weapons/ak47-2.wav",
}

enum {
	AK47_IDLE,
	AK47_RELOAD,
	AK47_DRAW,
	AK47_SHOOT1,
	AK47_SHOOT2,
	AK47_SHOOT3,
}

createAK47() {
	new Weapon:weapon = create_weapon("default_ak47", weapon_type_primary)
	set_weapon_var(weapon, "name", "DEFAULT_AK47")
	set_weapon_var(weapon, "view_model", create_model("models/v_ak47.mdl"))
	set_weapon_var(weapon, "player_model", create_model("models/p_ak47.mdl"))
	set_weapon_var(weapon, "world_model", create_model("models/w_ak47.mdl"))
	set_weapon_var(weapon, "hud", "weapon_ak47")
	set_weapon_var(weapon, "max_clip", 30)
	set_weapon_var(weapon, "max_ammo", 90)
	set_weapon_var(weapon, "slot", PRIMARY_WEAPON_SLOT)
	set_weapon_var(weapon, "weight", AK47_WEIGHT)
	set_weapon_var(weapon, "base_accuracy", AK47_BASE_ACCURACY)
	set_weapon_var(weapon, "crosshair_size", CrosshairSize_Size4)
	set_weapon_var(weapon, "forward_deploy", "@AK47_Deploy")
	set_weapon_var(weapon, "forward_max_speed", "@AK47_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@AK47_PrimaryAttack")
	set_weapon_var(weapon, "forward_reload", "@AK47_Reload")
	set_weapon_var(weapon, "forward_idle", "@AK47_Idle")
	precache_sounds(AK47_SHOOT_SOUND)
}

@AK47_Deploy(weapon, player) {
	if (!weapon_default_deploy(weapon, player, AK47_DRAW, "ak47")) {
		return false
	}
	set_member(weapon, m_Weapon_iShotsFired, 0)
	set_member(weapon, m_Weapon_flAccuracy, AK47_BASE_ACCURACY)
	return true
}

@AK47_MaxSpeed(weapon, player) {
	return AK47_MAX_SPEED
}

@AK47_PrimaryAttack(weapon, player, clip, ammo) {
	new Float:accuracy = get_member(weapon, m_Weapon_flAccuracy)
	new Float:spread
	new playerFlags = get_entvar(player, var_flags)
	new Float:playerVelocityLength = playerVelocityLength2d(player)
	if (!(playerFlags & FL_ONGROUND)) {
		spread = 0.04 + (0.4 * accuracy)
	} else if (playerVelocityLength > 140.0) {
		spread = 0.04 + (0.07 * accuracy)
	} else {
		spread = 0.0275 * accuracy
	}
	set_member(weapon, m_Weapon_bDelayFire, true)
	new shotsFired = get_member(weapon, m_Weapon_iShotsFired) + 1
	set_member(weapon, m_Weapon_iShotsFired, shotsFired)
	accuracy = ((shotsFired * shotsFired * shotsFired) / AK47_ACCURACY_DIVISOR) + 0.35
	if (accuracy > 1.25) {
		accuracy = 1.25
	}
	set_member(weapon, m_Weapon_flAccuracy, accuracy)
	if (isEmptyFire(weapon, clip)) {
		return
	}
	set_member(weapon, m_Weapon_iClip, --clip)
	set_member(player, m_iWeaponVolume, NORMAL_GUN_VOLUME)
	set_member(player, m_iWeaponFlash, BRIGHT_GUN_FLASH)
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
		8192.0,
		2,
		BULLET_PLAYER_762MM,
		AK47_DAMAGE,
		AK47_RANGE_MODIFER,
		false,
		get_member(player, random_seed)
	)
	send_weapon_anim(player, random_num(AK47_SHOOT1, AK47_SHOOT3))
	playFireSound(player, AK47_SHOOT_SOUND[random(sizeof(AK47_SHOOT_SOUND))])
	new Float:nextAttack = 0.0955
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 1.9)
	if (playerVelocityLength > 0.0) { // why it first
		weapon_kick_back(weapon, player, 1.5, 0.45, 0.225, 0.05, 6.5, 2.5, 7)
	} else if (!(playerFlags & FL_ONGROUND)) {
		weapon_kick_back(weapon, player, 2.0, 1.0, 0.5, 0.35, 9.0, 6.0, 5)
	} else if (playerFlags & FL_DUCKING) {
		weapon_kick_back(weapon, player, 0.9, 0.35, 0.15, 0.025, 5.5, 1.5, 9)
	} else {
		weapon_kick_back(weapon, player, 1.0, 0.375, 0.175, 0.0375, 5.75, 1.75, 8)
	}
}

@AK47_Reload(weapon, player) {
	if (!weapon_default_reload(weapon, player, AK47_RELOAD, AK47_RELOAD_TIME)) {
		return
	}
	rg_set_animation(player, PLAYER_RELOAD)
	set_member(weapon, m_Weapon_bDelayFire, false)
	set_member(weapon, m_Weapon_iShotsFired, 0)
	set_member(weapon, m_Weapon_flAccuracy, AK47_BASE_ACCURACY)
}

@AK47_Idle(weapon, player, Float:idleTime) {
	// need rework after grenades
	ExecuteHamB(Ham_Weapon_ResetEmptySound, weapon)
	if (idleTime > 0.0) {
		return
	}
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 20.0)
	send_weapon_anim(player, AK47_IDLE)
}

const M4A1_MAX_SPEED = 230
const M4A1_WEIGHT = 25
const M4A1_DAMAGE = 32
const M4A1_DAMAGE_SILENCER = 33
const Float:M4A1_RANGE_MODIFER = 0.97
const Float:M4A1_RANGE_MODIFER_SILENCER = 0.95
const Float:M4A1_RELOAD_TIME = 3.05
const Float:M4A1_BASE_ACCURACY = 0.2
const Float:M4A1_ACCURACY_DIVISOR = 220.0
const Float:M4A1_ADJUST_SILENCER_TIME = 2.0
new const M4A1_SHOOT_SOUND[3][] = {
	"weapons/m4a1-1.wav",
	"weapons/m4a1_unsil-1.wav",
	"weapons/m4a1_unsil-2.wav",
}

enum {
	M4A1_SILENCER_IDLE,
	M4A1_SILENCER_SHOOT1,
	M4A1_SILENCER_SHOOT2,
	M4A1_SILENCER_SHOOT3,
	M4A1_SILENCER_RELOAD,
	M4A1_SILENCER_DRAW,
	M4A1_ATTACH_SILENCER,
	M4A1_IDLE,
	M4A1_SHOOT1,
	M4A1_SHOOT2,
	M4A1_SHOOT3,
	M4A1_RELOAD,
	M4A1_DRAW,
	M4A1_DETACH_SILENCER,
}

createM4A1() {
	new Weapon:weapon = create_weapon("default_m4a1", weapon_type_primary)
	set_weapon_var(weapon, "name", "DEFAULT_M4A1")
	set_weapon_var(weapon, "view_model", create_model("models/v_m4a1.mdl"))
	set_weapon_var(weapon, "player_model", create_model("models/p_m4a1.mdl"))
	set_weapon_var(weapon, "world_model", create_model("models/w_m4a1.mdl"))
	set_weapon_var(weapon, "hud", "weapon_m4a1")
	set_weapon_var(weapon, "max_clip", 30)
	set_weapon_var(weapon, "max_ammo", 90)
	set_weapon_var(weapon, "slot", PRIMARY_WEAPON_SLOT)
	set_weapon_var(weapon, "weight", M4A1_WEIGHT)
	set_weapon_var(weapon, "base_accuracy", M4A1_BASE_ACCURACY)
	set_weapon_var(weapon, "crosshair_size", CrosshairSize_Size4)
	set_weapon_var(weapon, "forward_deploy", "@M4A1_Deploy")
	set_weapon_var(weapon, "forward_max_speed", "@M4A1_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@M4A1_PrimaryAttack")
	set_weapon_var(weapon, "forward_secondary_attack", "@M4A1_SecondaryAttack")
	set_weapon_var(weapon, "forward_reload", "@M4A1_Reload")
	set_weapon_var(weapon, "forward_idle", "@M4A1_Idle")
	precache_sounds(M4A1_SHOOT_SOUND)
}

@M4A1_Deploy(weapon, player) {
	new anim = (get_member(weapon, m_Weapon_iWeaponState) & WPNSTATE_M4A1_SILENCED) ? M4A1_SILENCER_DRAW : M4A1_DRAW
	if (!weapon_default_deploy(weapon, player, anim, "rifle")) {
		return false
	}
	set_member(weapon, m_Weapon_bDelayFire, false)
	set_member(weapon, m_Weapon_iShotsFired, 0)
	set_member(weapon, m_Weapon_flAccuracy, M4A1_BASE_ACCURACY)
	return true
}

@M4A1_MaxSpeed(weapon, player) {
	return M4A1_MAX_SPEED
}

@M4A1_PrimaryAttack(weapon, player, clip, ammo) {
	new bool:isSilenced = (get_member(weapon, m_Weapon_iWeaponState) & WPNSTATE_M4A1_SILENCED) ? true : false
	new Float:accuracy = get_member(weapon, m_Weapon_flAccuracy)
	new Float:spread
	new playerFlags = get_entvar(player, var_flags)
	new Float:playerVelocityLength = playerVelocityLength2d(player)
	if (isSilenced) {
		if (!(playerFlags & FL_ONGROUND)) {
			spread = 0.035 + (0.4 * accuracy)
		} else if (playerVelocityLength > 140.0) {
			spread = 0.035 + (0.07 * accuracy)
		} else {
			spread = 0.025 * accuracy
		}
	} else {
		if (!(playerFlags & FL_ONGROUND)) {
			spread = 0.035 + (0.4 * accuracy)
		} else if (playerVelocityLength > 140.0) {
			spread = 0.035 + (0.07 * accuracy)
		} else {
			spread = 0.02 * accuracy
		}
	}
	set_member(weapon, m_Weapon_bDelayFire, true)
	new shotsFired = get_member(weapon, m_Weapon_iShotsFired) + 1
	set_member(weapon, m_Weapon_iShotsFired, shotsFired)
	accuracy = ((shotsFired * shotsFired * shotsFired) / M4A1_ACCURACY_DIVISOR) + 0.3
	if (accuracy > 1.0) {
		accuracy = 1.0
	}
	set_member(weapon, m_Weapon_flAccuracy, accuracy)
	if (isEmptyFire(weapon, clip)) {
		return
	}
	set_member(weapon, m_Weapon_iClip, --clip)
	set_member(player, m_iWeaponVolume, NORMAL_GUN_VOLUME)
	set_member(player, m_iWeaponFlash, BRIGHT_GUN_FLASH)
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
		8192.0,
		2,
		BULLET_PLAYER_556MM,
		isSilenced ? M4A1_DAMAGE_SILENCER : M4A1_DAMAGE,
		isSilenced ? M4A1_RANGE_MODIFER_SILENCER : M4A1_RANGE_MODIFER,
		false,
		get_member(player, random_seed)
	)
	if (isSilenced) {
		send_weapon_anim(player, random_num(M4A1_SILENCER_SHOOT1, M4A1_SILENCER_SHOOT3))
		playFireSound(player, M4A1_SHOOT_SOUND[0])
	} else {
		set_entvar(player, var_effects, get_entvar(player, var_effects) | EF_MUZZLEFLASH)
		send_weapon_anim(player, random_num(M4A1_SHOOT1, M4A1_SHOOT3))
		playFireSound(player, M4A1_SHOOT_SOUND[random_num(1, 2)])
	}
	new Float:nextAttack = 0.0875
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 1.5)
	if (playerVelocityLength > 0.0) { // why it first
		weapon_kick_back(weapon, player, 1.0, 0.45, 0.28, 0.045, 3.75, 3.0, 7)
	} else if (!(playerFlags & FL_ONGROUND)) {
		weapon_kick_back(weapon, player, 1.2, 0.5, 0.23, 0.15, 5.5, 3.5, 6)
	} else if (playerFlags & FL_DUCKING) {
		weapon_kick_back(weapon, player, 0.6, 0.3, 0.2, 0.0125, 3.25, 2.0, 7)
	} else {
		weapon_kick_back(weapon, player, 0.65, 0.35, 0.25, 0.015, 3.5, 2.25, 7)
	}
}

@M4A1_SecondaryAttack(weapon, player, WeaponState:weaponState) {
	if (weaponState & WPNSTATE_M4A1_SILENCED) {
		set_member(weapon, m_Weapon_iWeaponState, weaponState & ~WPNSTATE_M4A1_SILENCED)
		send_weapon_anim(player, M4A1_DETACH_SILENCER)
	} else {
		set_member(weapon, m_Weapon_iWeaponState, weaponState | WPNSTATE_M4A1_SILENCED)
		send_weapon_anim(player, M4A1_ATTACH_SILENCER)
	}
	set_member(weapon, m_Weapon_flNextPrimaryAttack, M4A1_ADJUST_SILENCER_TIME)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, M4A1_ADJUST_SILENCER_TIME)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, M4A1_ADJUST_SILENCER_TIME)
}

@M4A1_Reload(weapon, player) {
	new anim = (get_member(weapon, m_Weapon_iWeaponState) & WPNSTATE_M4A1_SILENCED) ? M4A1_SILENCER_RELOAD : M4A1_RELOAD
	if (!weapon_default_reload(weapon, player, anim, M4A1_RELOAD_TIME)) {
		return
	}
	rg_set_animation(player, PLAYER_RELOAD)
	set_member(weapon, m_Weapon_bDelayFire, false)
	set_member(weapon, m_Weapon_flAccuracy, M4A1_BASE_ACCURACY)
	set_member(weapon, m_Weapon_iShotsFired, 0)
}

@M4A1_Idle(weapon, player, Float:idleTime) {
	// need rework after grenades
	ExecuteHamB(Ham_Weapon_ResetEmptySound, weapon)
	if (idleTime > 0.0) {
		return
	}
	new anim = (get_member(weapon, m_Weapon_iWeaponState) & WPNSTATE_M4A1_SILENCED) ? M4A1_SILENCER_IDLE : M4A1_IDLE
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 20.0)
	send_weapon_anim(player, anim)
}

const SG552_MAX_SPEED = 235
const SG552_MAX_SPEED_ZOOM = 200
const SG552_WEIGHT = 25
const SG552_DAMAGE = 33
const Float:SG552_RANGE_MODIFER = 0.955
const Float:SG552_RELOAD_TIME = 3.0
const Float:SG552_BASE_ACCURACY = 0.2
const Float:SG552_ACCURACY_DIVISOR = 220.0
new const SG552_SHOOT_SOUND[2][] = {
	"weapons/sg552-1.wav",
	"weapons/sg552-2.wav",
}

enum {
	SG552_IDLE,
	SG552_RELOAD,
	SG552_DRAW,
	SG552_SHOOT1,
	SG552_SHOOT2,
	SG552_SHOOT3,
}

createSG552() {
	new Weapon:weapon = create_weapon("default_sg552", weapon_type_primary)
	set_weapon_var(weapon, "name", "DEFAULT_SG552")
	set_weapon_var(weapon, "view_model", create_model("models/v_sg552.mdl"))
	set_weapon_var(weapon, "player_model", create_model("models/p_sg552.mdl"))
	set_weapon_var(weapon, "world_model", create_model("models/w_sg552.mdl"))
	set_weapon_var(weapon, "hud", "weapon_sg552")
	set_weapon_var(weapon, "max_clip", 30)
	set_weapon_var(weapon, "max_ammo", 90)
	set_weapon_var(weapon, "slot", PRIMARY_WEAPON_SLOT)
	set_weapon_var(weapon, "weight", SG552_WEIGHT)
	set_weapon_var(weapon, "base_accuracy", SG552_BASE_ACCURACY)
	set_weapon_var(weapon, "crosshair_size", CrosshairSize_Size5)
	set_weapon_var(weapon, "forward_deploy", "@SG552_Deploy")
	set_weapon_var(weapon, "forward_max_speed", "@SG552_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@SG552_PrimaryAttack")
	set_weapon_var(weapon, "forward_secondary_attack", "@SG552_SecondaryAttack")
	set_weapon_var(weapon, "forward_reload", "@SG552_Reload")
	set_weapon_var(weapon, "forward_idle", "@SG552_Idle")
	precache_sounds(SG552_SHOOT_SOUND)
}

@SG552_Deploy(weapon, player) {
	if (!weapon_default_deploy(weapon, player, SG552_DRAW, "mp5")) {
		return false
	}
	set_member(weapon, m_Weapon_iShotsFired, 0)
	set_member(weapon, m_Weapon_flAccuracy, SG552_BASE_ACCURACY)
	return true
}

@SG552_MaxSpeed(weapon, player, fov) {
	return ((fov == DEFAULT_FOV) ? SG552_MAX_SPEED : SG552_MAX_SPEED_ZOOM)
}

@SG552_PrimaryAttack(weapon, player, clip, ammo) {
	new Float:accuracy = get_member(weapon, m_Weapon_flAccuracy)
	new Float:spread
	new playerFlags = get_entvar(player, var_flags)
	new Float:playerVelocityLength = playerVelocityLength2d(player)
	if (!(playerFlags & FL_ONGROUND)) {
		spread = 0.035 + (0.45 * accuracy)
	} else if (playerVelocityLength > 140.0) {
		spread = 0.035 + (0.075 * accuracy)
	} else {
		spread = 0.02 * accuracy
	}
	set_member(weapon, m_Weapon_bDelayFire, true)
	new shotsFired = get_member(weapon, m_Weapon_iShotsFired) + 1
	set_member(weapon, m_Weapon_iShotsFired, shotsFired)
	accuracy = ((shotsFired * shotsFired * shotsFired) / SG552_ACCURACY_DIVISOR) + 0.3
	if (accuracy > 1.0) {
		accuracy = 1.0
	}
	set_member(weapon, m_Weapon_flAccuracy, accuracy)
	if (isEmptyFire(weapon, clip)) {
		return
	}
	set_member(weapon, m_Weapon_iClip, --clip)
	set_member(player, m_iWeaponVolume, NORMAL_GUN_VOLUME)
	set_member(player, m_iWeaponFlash, BRIGHT_GUN_FLASH)
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
		8192.0,
		2,
		BULLET_PLAYER_556MM,
		SG552_DAMAGE,
		SG552_RANGE_MODIFER,
		false,
		get_member(player, random_seed)
	)
	send_weapon_anim(player, random_num(SG552_SHOOT1, SG552_SHOOT3))
	playFireSound(player, SG552_SHOOT_SOUND[random(sizeof(SG552_SHOOT_SOUND))])
	new Float:nextAttack = (get_member(player, m_iFOV) == DEFAULT_FOV) ? 0.0825 : 0.135
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 2.0)
	if (playerVelocityLength > 0.0) { // why it first
		weapon_kick_back(weapon, player, 1.0, 0.45, 0.28, 0.04, 4.25, 2.5, 7)
	} else if (!(playerFlags & FL_ONGROUND)) {
		weapon_kick_back(weapon, player, 1.25, 0.45, 0.22, 0.18, 6.0, 4.0, 5)
	} else if (playerFlags & FL_DUCKING) {
		weapon_kick_back(weapon, player, 0.6, 0.35, 0.2, 0.0125, 3.7, 2.0, 10)
	} else {
		weapon_kick_back(weapon, player, 0.625, 0.375, 0.25, 0.0125, 4.0, 2.25, 9)
	}
}

@SG552_SecondaryAttack(weapon, player, WeaponState:weaponState, fov) {
	new nextFov
	if (fov == DEFAULT_FOV) {
		nextFov = 55
	} else {
		nextFov = DEFAULT_FOV
	}
	set_member(player, m_iFOV, nextFov)
	//rg_reset_maxspeed(player) // why not call?
	set_member(weapon, m_Weapon_flNextSecondaryAttack, 0.3)
}

@SG552_Reload(weapon, player) {
	if (!weapon_default_reload(weapon, player, SG552_RELOAD, SG552_RELOAD_TIME)) {
		return
	}
	if (get_member(player, m_iFOV) != DEFAULT_FOV) {
		ExecuteHamB(Ham_Weapon_SecondaryAttack, weapon)
	}
	rg_set_animation(player, PLAYER_RELOAD)
	set_member(weapon, m_Weapon_bDelayFire, false)
	set_member(weapon, m_Weapon_iShotsFired, 0)
	set_member(weapon, m_Weapon_flAccuracy, SG552_BASE_ACCURACY)
}

@SG552_Idle(weapon, player, Float:idleTime) {
	// need rework after grenades
	ExecuteHamB(Ham_Weapon_ResetEmptySound, weapon)
	if (idleTime > 0.0) {
		return
	}
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 20.0)
	send_weapon_anim(player, SG552_IDLE)
}

const AUG_MAX_SPEED = 240
// where is zoom speed?
const AUG_WEIGHT = 25
const AUG_DAMAGE = 32
const Float:AUG_RANGE_MODIFER = 0.96
const Float:AUG_RELOAD_TIME = 3.3
const Float:AUG_BASE_ACCURACY = 0.2
const Float:AUG_ACCURACY_DIVISOR = 215.0
new const AUG_SHOOT_SOUND[] = "weapons/aug-1.wav"

enum {
	AUG_IDLE,
	AUG_RELOAD,
	AUG_DRAW,
	AUG_SHOOT1,
	AUG_SHOOT2,
	AUG_SHOOT3,
}

createAUG() {
	new Weapon:weapon = create_weapon("default_aug", weapon_type_primary)
	set_weapon_var(weapon, "name", "DEFAULT_AUG")
	set_weapon_var(weapon, "view_model", create_model("models/v_aug.mdl"))
	set_weapon_var(weapon, "player_model", create_model("models/p_aug.mdl"))
	set_weapon_var(weapon, "world_model", create_model("models/w_aug.mdl"))
	set_weapon_var(weapon, "hud", "weapon_aug")
	set_weapon_var(weapon, "max_clip", 30)
	set_weapon_var(weapon, "max_ammo", 90)
	set_weapon_var(weapon, "slot", PRIMARY_WEAPON_SLOT)
	set_weapon_var(weapon, "weight", AUG_WEIGHT)
	set_weapon_var(weapon, "base_accuracy", AUG_BASE_ACCURACY)
	set_weapon_var(weapon, "crosshair_size", CrosshairSize_Size3)
	set_weapon_var(weapon, "forward_deploy", "@AUG_Deploy")
	set_weapon_var(weapon, "forward_max_speed", "@AUG_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@AUG_PrimaryAttack")
	set_weapon_var(weapon, "forward_secondary_attack", "@AUG_SecondaryAttack")
	set_weapon_var(weapon, "forward_reload", "@AUG_Reload")
	set_weapon_var(weapon, "forward_idle", "@AUG_Idle")
	precache_sound(AUG_SHOOT_SOUND)
}

@AUG_Deploy(weapon, player) {
	if (!weapon_default_deploy(weapon, player, AUG_DRAW, "carbine")) {
		return false
	}
	set_member(weapon, m_Weapon_iShotsFired, 0)
	set_member(weapon, m_Weapon_flAccuracy, AUG_BASE_ACCURACY)
	return true
}

@AUG_MaxSpeed(weapon, player) {
	return AUG_MAX_SPEED
}

@AUG_PrimaryAttack(weapon, player, clip, ammo) {
	new Float:accuracy = get_member(weapon, m_Weapon_flAccuracy)
	new Float:spread
	new playerFlags = get_entvar(player, var_flags)
	new Float:playerVelocityLength = playerVelocityLength2d(player)
	if (!(playerFlags & FL_ONGROUND)) {
		spread = 0.035 + (0.4 * accuracy)
	} else if (playerVelocityLength > 140.0) {
		spread = 0.035 + (0.07 * accuracy)
	} else {
		spread = 0.02 * accuracy
	}
	set_member(weapon, m_Weapon_bDelayFire, true)
	new shotsFired = get_member(weapon, m_Weapon_iShotsFired) + 1
	set_member(weapon, m_Weapon_iShotsFired, shotsFired)
	accuracy = ((shotsFired * shotsFired * shotsFired) / AUG_ACCURACY_DIVISOR) + 0.3
	if (accuracy > 1.0) {
		accuracy = 1.0
	}
	set_member(weapon, m_Weapon_flAccuracy, accuracy)
	if (isEmptyFire(weapon, clip)) {
		return
	}
	set_member(weapon, m_Weapon_iClip, --clip)
	set_member(player, m_iWeaponVolume, NORMAL_GUN_VOLUME)
	set_member(player, m_iWeaponFlash, BRIGHT_GUN_FLASH)
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
		8192.0,
		2,
		BULLET_PLAYER_556MM,
		AUG_DAMAGE,
		AUG_RANGE_MODIFER,
		false,
		get_member(player, random_seed)
	)
	send_weapon_anim(player, random_num(AUG_SHOOT1, AUG_SHOOT3))
	playFireSound(player, AUG_SHOOT_SOUND)
	new Float:nextAttack = (get_member(player, m_iFOV) == DEFAULT_FOV) ? 0.0825 : 0.135
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 1.9)
	if (playerVelocityLength > 0.0) { // why it first
		weapon_kick_back(weapon, player, 1.0, 0.45, 0.275, 0.05, 4.0, 2.5, 7)
	} else if (!(playerFlags & FL_ONGROUND)) {
		weapon_kick_back(weapon, player, 1.25, 0.45, 0.22, 0.18, 5.5, 4.0, 5)
	} else if (playerFlags & FL_DUCKING) {
		weapon_kick_back(weapon, player, 0.575, 0.325, 0.2, 0.011, 3.25, 2.0, 8)
	} else {
		weapon_kick_back(weapon, player, 0.625, 0.375, 0.25, 0.0125, 3.5, 2.25, 8)
	}
}

@AUG_SecondaryAttack(weapon, player, WeaponState:weaponState, fov) {
	new nextFov
	if (fov == DEFAULT_FOV) {
		nextFov = 55
	} else {
		nextFov = DEFAULT_FOV
	}
	set_member(player, m_iFOV, nextFov)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, 0.3)
}

@AUG_Reload(weapon, player) {
	if (!weapon_default_reload(weapon, player, AUG_RELOAD, AUG_RELOAD_TIME)) {
		return
	}
	if (get_member(player, m_iFOV) != DEFAULT_FOV) {
		ExecuteHamB(Ham_Weapon_SecondaryAttack, weapon)
	}
	rg_set_animation(player, PLAYER_RELOAD)
	set_member(weapon, m_Weapon_bDelayFire, false)
	set_member(weapon, m_Weapon_flAccuracy, 0.0) // why 0.0
	set_member(weapon, m_Weapon_iShotsFired, 0)
}

@AUG_Idle(weapon, player, Float:idleTime) {
	// need rework after grenades
	ExecuteHamB(Ham_Weapon_ResetEmptySound, weapon)
	if (idleTime > 0.0) {
		return
	}
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 20.0)
	send_weapon_anim(player, AUG_IDLE)
}

const SCOUT_MAX_SPEED = 260
const SCOUT_MAX_SPEED_ZOOM = 220
const SCOUT_WEIGHT = 30
const SCOUT_DAMAGE = 75
const Float:SCOUT_RANGE_MODIFER = 0.98
const Float:SCOUT_RELOAD_TIME = 2.0
new const SCOUT_SHOOT_SOUND[] = "weapons/scout_fire-1.wav"

enum {
	SCOUT_IDLE,
	SCOUT_SHOOT1,
	SCOUT_SHOOT2,
	SCOUT_RELOAD,
	SCOUT_DRAW,
}

createScout() {
	new Weapon:weapon = create_weapon("default_scout", weapon_type_primary)
	set_weapon_var(weapon, "name", "DEFAULT_SCOUT")
	set_weapon_var(weapon, "view_model", create_model("models/v_scout.mdl"))
	set_weapon_var(weapon, "player_model", create_model("models/p_scout.mdl"))
	set_weapon_var(weapon, "world_model", create_model("models/w_scout.mdl"))
	set_weapon_var(weapon, "hud", "weapon_scout")
	set_weapon_var(weapon, "max_clip", 10)
	set_weapon_var(weapon, "max_ammo", 90)
	set_weapon_var(weapon, "slot", PRIMARY_WEAPON_SLOT)
	set_weapon_var(weapon, "weight", SCOUT_WEIGHT)
	set_weapon_var(weapon, "forward_deploy", "@Scout_Deploy")
	set_weapon_var(weapon, "forward_max_speed", "@Scout_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@Scout_PrimaryAttack")
	set_weapon_var(weapon, "forward_secondary_attack", "@Scout_SecondaryAttack")
	set_weapon_var(weapon, "forward_reload", "@Scout_Reload")
	set_weapon_var(weapon, "forward_idle", "@Scout_Idle")
	precache_sound(SCOUT_SHOOT_SOUND)
}

@Scout_Deploy(weapon, player) {
	if (!weapon_default_deploy(weapon, player, SCOUT_DRAW, "rifle")) {
		return false
	}
	new Float:nextAttack = 1.25
	set_member(player, m_flNextAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, 1.0)
	return true
}

@Scout_MaxSpeed(weapon, player, fov) {
	return ((fov == DEFAULT_FOV) ? SCOUT_MAX_SPEED : SCOUT_MAX_SPEED_ZOOM)
}

@Scout_PrimaryAttack(weapon, player, clip, ammo) {
	new Float:spread
	new playerFlags = get_entvar(player, var_flags)
	if (!(playerFlags & FL_ONGROUND)) {
		spread = 0.2
	} else if (playerVelocityLength2d(player) > 170.0) {
		spread = 0.075
	} else if (playerFlags & FL_DUCKING) {
		spread = 0.0
	} else {
		spread = 0.007
	}
	new fov = get_member(player, m_iFOV)
	if (fov != DEFAULT_FOV) {
		set_member(player, m_bResumeZoom, true)
		set_member(player, m_iLastZoom, fov)
		set_member(player, m_iFOV, DEFAULT_FOV)
	} else {
		spread += 0.025
	}
	if (isEmptyFire(weapon, clip)) {
		return
	}
	set_member(weapon, m_Weapon_iClip, --clip)
	set_member(player, m_iWeaponVolume, BIG_EXPLOSION_VOLUME)
	set_member(player, m_iWeaponFlash, NORMAL_GUN_FLASH)
	set_member(player, m_flEjectBrass, get_gametime() + 0.56)
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
		8192.0,
		3,
		BULLET_PLAYER_762MM,
		SCOUT_DAMAGE,
		SCOUT_RANGE_MODIFER,
		true,
		get_member(player, random_seed)
	)
	send_weapon_anim(player, random_num(SCOUT_SHOOT1, SCOUT_SHOOT2))
	playFireSound(player, SCOUT_SHOOT_SOUND)
	new Float:nextAttack = 1.25
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 1.8)
	new Float:punchAngle[3]
	get_entvar(player, var_punchangle, punchAngle)
	punchAngle[0] -= 2.0
	set_entvar(player, var_punchangle, punchAngle)
}

@Scout_SecondaryAttack(weapon, player, WeaponState:weaponState, fov) {
	new nextFov
	if (fov == DEFAULT_FOV) {
		nextFov = 40
	} else if (fov == 40) {
		nextFov = 15
	} else {
		nextFov = DEFAULT_FOV
	}
	set_member(player, m_iFOV, nextFov)
	rg_reset_maxspeed(player)
	emit_sound(player, CHAN_ITEM, "weapons/zoom.wav", 0.2, 2.4, 0, PITCH_NORM)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, 0.3)
}

@Scout_Reload(weapon, player) {
	if (!weapon_default_reload(weapon, player, SCOUT_RELOAD, SCOUT_RELOAD_TIME)) {
		return
	}
	if (get_member(player, m_iFOV) != DEFAULT_FOV) {
		set_member(player, m_iFOV, 15)
		ExecuteHamB(Ham_Weapon_SecondaryAttack, weapon)
	}
	rg_set_animation(player, PLAYER_RELOAD)
}

@Scout_Idle(weapon, player, Float:idleTime) {
	// need rework after grenades
	ExecuteHamB(Ham_Weapon_ResetEmptySound, weapon)
	if (idleTime > 0.0) {
		return
	}
	if (!get_member(weapon, m_Weapon_iClip)) {
		return
	}
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 60.0)
	send_weapon_anim(player, SCOUT_IDLE)
}

const AWP_MAX_SPEED = 210
const AWP_MAX_SPEED_ZOOM = 150
const AWP_WEIGHT = 30
const AWP_DAMAGE = 115
const Float:AWP_RANGE_MODIFER = 0.99
const Float:AWP_RELOAD_TIME = 2.5
new const AWP_SHOOT_SOUND[] = "weapons/awp1.wav"

enum {
	AWP_IDLE,
	AWP_SHOOT1,
	AWP_SHOOT2,
	AWP_SHOOT3,
	AWP_RELOAD,
	AWP_DRAW,
}

createAWP() {
	new Weapon:weapon = create_weapon("default_awp", weapon_type_primary)
	set_weapon_var(weapon, "name", "DEFAULT_AWP")
	set_weapon_var(weapon, "view_model", create_model("models/v_awp.mdl"))
	set_weapon_var(weapon, "player_model", create_model("models/p_awp.mdl"))
	set_weapon_var(weapon, "world_model", create_model("models/w_awp.mdl"))
	set_weapon_var(weapon, "hud", "weapon_awp")
	set_weapon_var(weapon, "max_clip", 10)
	set_weapon_var(weapon, "max_ammo", 30)
	set_weapon_var(weapon, "slot", PRIMARY_WEAPON_SLOT)
	set_weapon_var(weapon, "weight", AWP_WEIGHT)
	set_weapon_var(weapon, "forward_deploy", "@AWP_Deploy")
	set_weapon_var(weapon, "forward_max_speed", "@AWP_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@AWP_PrimaryAttack")
	set_weapon_var(weapon, "forward_secondary_attack", "@AWP_SecondaryAttack")
	set_weapon_var(weapon, "forward_reload", "@AWP_Reload")
	set_weapon_var(weapon, "forward_idle", "@AWP_Idle")
	precache_sound(AWP_SHOOT_SOUND)
}

@AWP_Deploy(weapon, player) {
	if (!weapon_default_deploy(weapon, player, AWP_DRAW, "rifle")) {
		return false
	}
	new Float:nextAttack = 1.45
	set_member(player, m_flNextAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, 1.0)
	return true
}

@AWP_MaxSpeed(weapon, player, fov) {
	return ((fov == DEFAULT_FOV) ? AWP_MAX_SPEED : AWP_MAX_SPEED_ZOOM)
}

@AWP_PrimaryAttack(weapon, player, clip, ammo) {
	new Float:spread
	new playerFlags = get_entvar(player, var_flags)
	if (!(playerFlags & FL_ONGROUND)) {
		spread = 0.85
	} else if (playerVelocityLength2d(player) > 140.0) {
		spread = 0.25
	} else if (playerVelocityLength2d(player) > 10.0) {
		spread = 0.1
	} else if (playerFlags & FL_DUCKING) {
		spread = 0.0
	} else {
		spread = 0.001
	}
	new fov = get_member(player, m_iFOV)
	if (fov != DEFAULT_FOV) {
		set_member(player, m_bResumeZoom, true)
		set_member(player, m_iLastZoom, fov)
		set_member(player, m_iFOV, DEFAULT_FOV)
	} else {
		spread += 0.08
	}
	if (isEmptyFire(weapon, clip)) {
		return
	}
	set_member(weapon, m_Weapon_iClip, --clip)
	set_member(player, m_iWeaponVolume, BIG_EXPLOSION_VOLUME)
	set_member(player, m_iWeaponFlash, NORMAL_GUN_FLASH)
	set_member(player, m_flEjectBrass, get_gametime() + 0.55)
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
		8192.0,
		3,
		BULLET_PLAYER_338MAG,
		AWP_DAMAGE,
		AWP_RANGE_MODIFER,
		true,
		get_member(player, random_seed)
	)
	send_weapon_anim(player, random_num(AWP_SHOOT1, AWP_SHOOT3))
	playFireSound(player, AWP_SHOOT_SOUND)
	new Float:nextAttack = 1.45
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 2.0)
	new Float:punchAngle[3]
	get_entvar(player, var_punchangle, punchAngle)
	punchAngle[0] -= 2.0
	set_entvar(player, var_punchangle, punchAngle)
}

@AWP_SecondaryAttack(weapon, player, WeaponState:weaponState, fov) {
	new nextFov
	if (fov == DEFAULT_FOV) {
		nextFov = 40
	} else if (fov == 40) {
		nextFov = 10
	} else {
		nextFov = DEFAULT_FOV
	}
	set_member(player, m_iFOV, nextFov)
	rg_reset_maxspeed(player)
	emit_sound(player, CHAN_ITEM, "weapons/zoom.wav", 0.2, 2.4, 0, PITCH_NORM)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, 0.3)
}

@AWP_Reload(weapon, player) {
	if (!weapon_default_reload(weapon, player, AWP_RELOAD, AWP_RELOAD_TIME)) {
		return
	}
	rg_set_animation(player, PLAYER_RELOAD)
	if (get_member(player, m_iFOV) != DEFAULT_FOV) {
		set_member(player, m_iFOV, 15)
		ExecuteHamB(Ham_Weapon_SecondaryAttack, weapon)
	}
}

@AWP_Idle(weapon, player, Float:idleTime) {
	ExecuteHamB(Ham_Weapon_ResetEmptySound, weapon)
	if (idleTime > 0.0) {
		return
	}
	if (!get_member(weapon, m_Weapon_iClip)) {
		return
	}
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 60.0)
	send_weapon_anim(player, AWP_IDLE)
}

const G3SG1_MAX_SPEED = 210
const G3SG1_MAX_SPEED_ZOOM = 150
const G3SG1_WEIGHT = 20
const G3SG1_DAMAGE = 80
const Float:G3SG1_RANGE_MODIFER = 0.98
const Float:G3SG1_RELOAD_TIME = 3.5
new const G3SG1_SHOOT_SOUND[] = "weapons/g3sg1-1.wav"

enum {
	G3SG1_IDLE,
	G3SG1_SHOOT1,
	G3SG1_SHOOT2,
	G3SG1_RELOAD,
	G3SG1_DRAW,
}

createG3SG1() {
	new Weapon:weapon = create_weapon("default_g3sg1", weapon_type_primary)
	set_weapon_var(weapon, "name", "DEFAULT_G3SG1")
	set_weapon_var(weapon, "view_model", create_model("models/v_g3sg1.mdl"))
	set_weapon_var(weapon, "player_model", create_model("models/p_g3sg1.mdl"))
	set_weapon_var(weapon, "world_model", create_model("models/w_g3sg1.mdl"))
	set_weapon_var(weapon, "hud", "weapon_g3sg1")
	set_weapon_var(weapon, "max_clip", 20)
	set_weapon_var(weapon, "max_ammo", 90)
	set_weapon_var(weapon, "slot", PRIMARY_WEAPON_SLOT)
	set_weapon_var(weapon, "weight", G3SG1_WEIGHT)
	set_weapon_var(weapon, "forward_deploy", "@G3SG1_Deploy")
	set_weapon_var(weapon, "forward_max_speed", "@G3SG1_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@G3SG1_PrimaryAttack")
	set_weapon_var(weapon, "forward_secondary_attack", "@G3SG1_SecondaryAttack")
	set_weapon_var(weapon, "forward_reload", "@G3SG1_Reload")
	set_weapon_var(weapon, "forward_idle", "@G3SG1_Idle")
	precache_sound(G3SG1_SHOOT_SOUND)
}

@G3SG1_Deploy(weapon, player) {
	if (!weapon_default_deploy(weapon, player, G3SG1_DRAW, "mp5")) {
		return false
	}
	set_member(weapon, m_Weapon_flAccuracy, 0.2)
	return true
}

@G3SG1_MaxSpeed(weapon, player, fov) {
	return ((fov == DEFAULT_FOV) ? G3SG1_MAX_SPEED : G3SG1_MAX_SPEED_ZOOM)
}

@G3SG1_PrimaryAttack(weapon, player, clip, ammo) {
	new Float:spread
	new playerFlags = get_entvar(player, var_flags)
	if (!(playerFlags & FL_ONGROUND)) {
		spread = 0.45
	} else if (playerVelocityLength2d(player) > 0.0) {
		spread = 0.15
	} else if (playerFlags & FL_DUCKING) {
		spread = 0.035
	} else {
		spread = 0.055
	}
	new fov = get_member(player, m_iFOV)
	if (fov != DEFAULT_FOV) {
		spread += 0.025
	}
	new Float:time = get_gametime()
	new Float:lastFire = Float:get_member(weapon, m_Weapon_flLastFire)
	new Float:accuracy
	if (lastFire) {
		accuracy = (time - lastFire) * 0.3 + 0.55
		if (accuracy > 0.98) {
			accuracy = 0.98
		}
	} else {
		accuracy = 0.98
	}
	set_member(weapon, m_Weapon_flAccuracy, accuracy)
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
		(1.0 - accuracy) * spread,
		8192.0,
		3,
		BULLET_PLAYER_762MM,
		G3SG1_DAMAGE,
		G3SG1_RANGE_MODIFER,
		true,
		get_member(player, random_seed)
	)
	send_weapon_anim(player, random_num(G3SG1_SHOOT1, G3SG1_SHOOT2))
	playFireSound(player, G3SG1_SHOOT_SOUND)
	new Float:nextAttack = 0.25
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 1.8)
	new Float:punchAngle[3]
	get_entvar(player, var_punchangle, punchAngle)
	punchAngle[0] -= punchAngle[0] * 0.25 + random_float(0.75, 1.75)
	punchAngle[1] += random_float(-0.75, 0.75)
	set_entvar(player, var_punchangle, punchAngle)
}

@G3SG1_SecondaryAttack(weapon, player, WeaponState:weaponState, fov) {
	new nextFov
	if (fov == DEFAULT_FOV) {
		nextFov = 40
	} else if (fov == 40) {
		nextFov = 15
	} else {
		nextFov = DEFAULT_FOV
	}
	set_member(player, m_iFOV, nextFov)
	rg_reset_maxspeed(player)
	emit_sound(player, CHAN_ITEM, "weapons/zoom.wav", 0.2, 2.4, 0, PITCH_NORM)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, 0.3)
}

@G3SG1_Reload(weapon, player) {
	if (!weapon_default_reload(weapon, player, G3SG1_RELOAD, G3SG1_RELOAD_TIME)) {
		return
	}
	if (get_member(player, m_iFOV) != DEFAULT_FOV) {
		set_member(player, m_iFOV, 15)
		ExecuteHamB(Ham_Weapon_SecondaryAttack, weapon)
	}
	set_member(weapon, m_Weapon_flAccuracy, 0.2)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, Float:get_member(weapon, m_Weapon_flTimeWeaponIdle) + 1.2)
	rg_set_animation(player, PLAYER_RELOAD)
}

@G3SG1_Idle(weapon, player, Float:idleTime) {
	// need rework after grenades
	ExecuteHamB(Ham_Weapon_ResetEmptySound, weapon)
	if (idleTime > 0.0) {
		return
	}
	if (!get_member(weapon, m_Weapon_iClip)) {
		return
	}
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 60.0)
	send_weapon_anim(player, G3SG1_IDLE)
}

const SG550_MAX_SPEED = 210
const SG550_MAX_SPEED_ZOOM = 150
const SG550_WEIGHT = 20
const SG550_DAMAGE = 70
const Float:SG550_RANGE_MODIFER = 0.98
const Float:SG550_RELOAD_TIME = 3.35
new const SG550_SHOOT_SOUND[] = "weapons/sg550-1.wav"

enum {
	SG550_IDLE,
	SG550_SHOOT1,
	SG550_SHOOT2,
	SG550_RELOAD,
	SG550_DRAW,
}

createSG550() {
	new Weapon:weapon = create_weapon("default_sg550", weapon_type_primary)
	set_weapon_var(weapon, "name", "DEFAULT_SG550")
	set_weapon_var(weapon, "view_model", create_model("models/v_sg550.mdl"))
	set_weapon_var(weapon, "player_model", create_model("models/p_sg550.mdl"))
	set_weapon_var(weapon, "world_model", create_model("models/w_sg550.mdl"))
	set_weapon_var(weapon, "hud", "weapon_sg550")
	set_weapon_var(weapon, "max_clip", 30)
	set_weapon_var(weapon, "max_ammo", 90)
	set_weapon_var(weapon, "slot", PRIMARY_WEAPON_SLOT)
	set_weapon_var(weapon, "weight", SG550_WEIGHT)
	set_weapon_var(weapon, "forward_deploy", "@SG550_Deploy")
	set_weapon_var(weapon, "forward_max_speed", "@SG550_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@SG550_PrimaryAttack")
	set_weapon_var(weapon, "forward_secondary_attack", "@SG550_SecondaryAttack")
	set_weapon_var(weapon, "forward_reload", "@SG550_Reload")
	set_weapon_var(weapon, "forward_idle", "@SG550_Idle")
	precache_sound(SG550_SHOOT_SOUND)
}

@SG550_Deploy(weapon, player) {
	if (!weapon_default_deploy(weapon, player, SG550_DRAW, "rifle")) {
		return false
	}
	set_member(weapon, m_Weapon_flAccuracy, 0.9)
	return true
}

@SG550_MaxSpeed(weapon, player, fov) {
	return ((fov == DEFAULT_FOV) ? SG550_MAX_SPEED : SG550_MAX_SPEED_ZOOM)
}

@SG550_PrimaryAttack(weapon, player, clip, ammo) {
	new Float:accuracy = get_member(weapon, m_Weapon_flAccuracy)
	new Float:spread
	new playerFlags = get_entvar(player, var_flags)
	if (!(playerFlags & FL_ONGROUND)) {
		spread = 0.45 * (1.0 - accuracy)
	} else if (playerVelocityLength2d(player) > 0.0) {
		spread = 0.15
	} else if (playerFlags & FL_DUCKING) {
		spread = 0.04 * (1.0 - accuracy)
	} else {
		spread = 0.05 * (1.0 - accuracy)
	}
	new fov = get_member(player, m_iFOV)
	if (fov != DEFAULT_FOV) {
		spread += 0.025
	}
	new Float:time = get_gametime()
	new Float:lastFire = Float:get_member(weapon, m_Weapon_flLastFire)
	if (lastFire) {
		new Float:accuracy = (time - lastFire) * 0.35 + 0.65
		if (accuracy > 0.98) {
			accuracy = 0.98
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
		8192.0,
		2,
		BULLET_PLAYER_556MM,
		SG550_DAMAGE,
		SG550_RANGE_MODIFER,
		true,
		get_member(player, random_seed)
	)
	send_weapon_anim(player, random_num(SG550_SHOOT1, SG550_SHOOT2))
	playFireSound(player, SG550_SHOOT_SOUND)
	new Float:nextAttack = 0.25
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 1.8)
	new Float:punchAngle[3]
	get_entvar(player, var_punchangle, punchAngle)
	punchAngle[0] -= punchAngle[0] * 0.25 + random_float(0.75, 1.25)
	punchAngle[1] += random_float(-0.75, 0.75)
	set_entvar(player, var_punchangle, punchAngle)
}

@SG550_SecondaryAttack(weapon, player, WeaponState:weaponState, fov) {
	new nextFov
	if (fov == DEFAULT_FOV) {
		nextFov = 40
	} else if (fov == 40) {
		nextFov = 15
	} else {
		nextFov = DEFAULT_FOV
	}
	set_member(player, m_iFOV, nextFov)
	rg_reset_maxspeed(player)
	emit_sound(player, CHAN_ITEM, "weapons/zoom.wav", 0.2, 2.4, 0, PITCH_NORM)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, 0.3)
}

@SG550_Reload(weapon, player) {
	if (!weapon_default_reload(weapon, player, SG550_RELOAD, SG550_RELOAD_TIME)) {
		return
	}
	if (get_member(player, m_iFOV) != DEFAULT_FOV) {
		set_member(player, m_iFOV, 15)
		ExecuteHamB(Ham_Weapon_SecondaryAttack, weapon)
	}
	rg_set_animation(player, PLAYER_RELOAD)
}

@SG550_Idle(weapon, player, Float:idleTime) {
	// need rework after grenades
	ExecuteHamB(Ham_Weapon_ResetEmptySound, weapon)
	if (idleTime > 0.0) {
		return
	}
	if (!get_member(weapon, m_Weapon_iClip)) {
		return
	}
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 60.0)
	send_weapon_anim(player, SG550_IDLE)
}

const M249_MAX_SPEED = 220
const M249_WEIGHT = 25
const M249_DAMAGE = 32
const Float:M249_RANGE_MODIFER = 0.97
const Float:M249_RELOAD_TIME = 4.7
const Float:M249_BASE_ACCURACY = 0.2
const Float:M249_ACCURACY_DIVISOR = 175.0
new const M249_SHOOT_SOUND[2][] = {
	"weapons/m249-1.wav",
	"weapons/m249-2.wav",
}

enum {
	M249_IDLE,
	M249_SHOOT1,
	M249_SHOOT2,
	M249_RELOAD,
	M249_DRAW,
}

createM249() {
	new Weapon:weapon = create_weapon("default_m249", weapon_type_primary)
	set_weapon_var(weapon, "name", "DEFAULT_M249")
	set_weapon_var(weapon, "view_model", create_model("models/v_m249.mdl"))
	set_weapon_var(weapon, "player_model", create_model("models/p_m249.mdl"))
	set_weapon_var(weapon, "world_model", create_model("models/w_m249.mdl"))
	set_weapon_var(weapon, "hud", "weapon_m249")
	set_weapon_var(weapon, "max_clip", 100)
	set_weapon_var(weapon, "max_ammo", 200)
	set_weapon_var(weapon, "slot", PRIMARY_WEAPON_SLOT)
	set_weapon_var(weapon, "weight", M249_WEIGHT)
	set_weapon_var(weapon, "base_accuracy", M249_BASE_ACCURACY)
	set_weapon_var(weapon, "crosshair_size", CrosshairSize_Size6)
	set_weapon_var(weapon, "forward_deploy", "@M249_Deploy")
	set_weapon_var(weapon, "forward_max_speed", "@M249_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@M249_PrimaryAttack")
	set_weapon_var(weapon, "forward_reload", "@M249_Reload")
	set_weapon_var(weapon, "forward_idle", "@M249_Idle")
	precache_sounds(M249_SHOOT_SOUND)
}

@M249_Deploy(weapon, player) {
	set_member(weapon, m_Weapon_iShotsFired, 0)
	set_member(weapon, m_Weapon_flAccuracy, M249_BASE_ACCURACY)
	return weapon_default_deploy(weapon, player, M249_DRAW, "m249")
}

@M249_MaxSpeed(weapon, player) {
	return M249_MAX_SPEED
}

@M249_PrimaryAttack(weapon, player, clip, ammo) {
	new Float:accuracy = get_member(weapon, m_Weapon_flAccuracy)
	new Float:spread
	new playerFlags = get_entvar(player, var_flags)
	new Float:playerVelocityLength = playerVelocityLength2d(player)
	if (!(playerFlags & FL_ONGROUND)) {
		spread = 0.045 + (0.5 * accuracy)
	} else if (playerVelocityLength > 140.0) {
		spread = 0.045 + (0.095 * accuracy)
	} else {
		spread = 0.03 * accuracy
	}
	set_member(weapon, m_Weapon_bDelayFire, true)
	new shotsFired = get_member(weapon, m_Weapon_iShotsFired) + 1
	set_member(weapon, m_Weapon_iShotsFired, shotsFired)
	accuracy = ((shotsFired * shotsFired * shotsFired) / M249_ACCURACY_DIVISOR) + 0.4
	if (accuracy > 0.9) {
		accuracy = 0.9
	}
	set_member(weapon, m_Weapon_flAccuracy, accuracy)
	if (isEmptyFire(weapon, clip)) {
		return
	}
	set_member(weapon, m_Weapon_iClip, --clip)
	set_member(player, m_iWeaponVolume, NORMAL_GUN_VOLUME)
	set_member(player, m_iWeaponFlash, BRIGHT_GUN_FLASH)
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
		8192.0,
		2,
		BULLET_PLAYER_556MM,
		M249_DAMAGE,
		M249_RANGE_MODIFER,
		false,
		get_member(player, random_seed)
	)
	send_weapon_anim(player, random_num(M249_SHOOT1, M249_SHOOT2))
	playFireSound(player, M249_SHOOT_SOUND[random(sizeof(M249_SHOOT_SOUND))])
	new Float:nextAttack = 0.1
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 1.6)
	if (!(playerFlags & FL_ONGROUND)) {
		weapon_kick_back(weapon, player, 1.8, 0.65, 0.45, 0.125, 5.0, 3.5, 8)
	} else if (playerVelocityLength > 0.0) {
		weapon_kick_back(weapon, player, 1.1, 0.5, 0.3, 0.06, 4.0, 3.0, 8)
	} else if (playerFlags & FL_DUCKING) {
		weapon_kick_back(weapon, player, 0.75, 0.325, 0.25, 0.025, 3.5, 2.5, 9)
	} else {
		weapon_kick_back(weapon, player, 0.8, 0.35, 0.3, 0.03, 3.75, 3.0, 9)
	}
}

@M249_Reload(weapon, player) {
	if (!weapon_default_reload(weapon, player, M249_RELOAD, M249_RELOAD_TIME)) {
		return
	}
	rg_set_animation(player, PLAYER_RELOAD)
	set_member(weapon, m_Weapon_bDelayFire, false)
	set_member(weapon, m_Weapon_iShotsFired, 0)
	set_member(weapon, m_Weapon_flAccuracy, M249_BASE_ACCURACY)
}

@M249_Idle(weapon, player, Float:idleTime) {
	// need rework after grenades
	ExecuteHamB(Ham_Weapon_ResetEmptySound, weapon)
	if (idleTime > 0.0) {
		return
	}
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 20.0)
	send_weapon_anim(player, M249_IDLE)
}

const HEGRENADE_MAX_SPEED = 250
const HEGRENADE_WEIGHT = 2

enum {
	HEGRENADE_IDLE,
	HEGRENADE_PULLPIN,
	HEGRENADE_THROW,
	HEGRENADE_DRAW,
}

createHeGrenade() {
	new Weapon:weapon = create_weapon("default_hegrenade", weapon_type_grenade)
	set_weapon_var(weapon, "name", "DEFAULT_HE_GRENADE")
	set_weapon_var(weapon, "view_model", create_model("models/v_hegrenade.mdl"))
	set_weapon_var(weapon, "player_model", create_model("models/p_hegrenade.mdl"))
	set_weapon_var(weapon, "world_model", create_model("models/w_hegrenade.mdl"))
	set_weapon_var(weapon, "hud", "weapon_hegrenade")
	set_weapon_var(weapon, "max_ammo", 1)
	set_weapon_var(weapon, "slot", PRIMARY_WEAPON_SLOT)
	set_weapon_var(weapon, "weight", HEGRENADE_WEIGHT)
	set_weapon_var(weapon, "flags", ITEM_FLAG_LIMITINWORLD | ITEM_FLAG_EXHAUSTIBLE)
	set_weapon_var(weapon, "crosshair_size", CrosshairSize_Size8)
	set_weapon_var(weapon, "forward_deploy", "@HeGrenade_Deploy")
	set_weapon_var(weapon, "forward_holster", "@HeGrenade_Holster")
	set_weapon_var(weapon, "forward_max_speed", "@HeGrenade_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@HeGrenade_PrimaryAttack")
	set_weapon_var(weapon, "forward_reload", "@HeGrenade_Reload")
	set_weapon_var(weapon, "forward_idle", "@HeGrenade_Idle")
}

@HeGrenade_Deploy(weapon, player) {
	if (!weapon_default_deploy(weapon, player, HEGRENADE_DRAW, "grenade")) {
		return false
	}
	set_member(weapon, m_flReleaseThrow, -1.0)
	return true
}

@HeGrenade_Holster(weapon, player) {
	set_member(player, m_flNextAttack, 0.5)
	if (!get_player_var(player, "ammo", weapon)) {
		destroyItem(weapon, player)
	}
	set_member(weapon, m_flStartThrow, 0.0)
	set_member(weapon, m_flReleaseThrow, -1.0)
}

@HeGrenade_MaxSpeed(weapon, player) {
	return HEGRENADE_MAX_SPEED
}

@HeGrenade_PrimaryAttack(weapon, player, clip, ammo) {
	if (Float:get_member(weapon, m_flStartThrow) || get_player_var(player, "ammo", weapon) <= 0) {
		return
	}
	set_member(weapon, m_flReleaseThrow, 0.0)
	set_member(weapon, m_flStartThrow, get_gametime())
	set_member(weapon, m_Weapon_flTimeWeaponIdle, 0.5)
	send_weapon_anim(player, HEGRENADE_PULLPIN)
}

@HeGrenade_Idle(weapon, player) {
	// need rework after grenades

	new Float:startThrow = Float:get_member(weapon, m_flStartThrow)
	new Float:releaseThrow = Float:get_member(weapon, m_flReleaseThrow)
	if (releaseThrow == 0.0 && startThrow) {
		set_member(weapon, m_flReleaseThrow, get_gametime())
	}
	if (Float:get_member(weapon, m_Weapon_flTimeWeaponIdle) > 0.0) {
		return
	}
	new ammo = get_player_var(player, "ammo", weapon)
	if (startThrow) {
		//m_pPlayer->Radio("%!MRAD_FIREINHOLE", "#Fire_in_the_hole");
		new Float:src[3]
		new Float:throw[3]
		new Float:aimingAngle[3]
		new Float:viewForward[3]
		new Float:playerVelocity[3]
		// to default_projectile_throw
		ExecuteHamB(Ham_Player_GetGunPosition, player, src)
		get_entvar(player, var_velocity, playerVelocity)
		getAimingAngle(player, aimingAngle)
		if (aimingAngle[0] < 0.0) {
			aimingAngle[0] = -10.0 + aimingAngle[0] * ((80.0) / 90.0)
		} else {
			aimingAngle[0] = -10.0 + aimingAngle[0] * ((100.0) / 90.0)
		}
		new Float:velocity = (90.0 - aimingAngle[0]) * 6.0
		if (velocity > 750.0) {
			velocity = 750.0
		}
		engfunc(EngFunc_MakeVectors, aimingAngle)
		global_get(glb_v_forward, viewForward)
		for (new i = 0; i < 3; ++i) {
			src[i] += viewForward[i] * 16.0
			throw[i] = viewForward[i] * velocity + playerVelocity[i]
		}
		new grenade = weapon_throw_grenade(weapon, player, src, throw, 1.5)
		if (!is_nullent(grenade)) {
			SetThink(grenade, "@HeGrenade_Think")
			SetTouch(grenade, "@HeGrenade_Touch")
		}
		send_weapon_anim(player, HEGRENADE_THROW)
		rg_set_animation(player, PLAYER_ATTACK1)
		set_member(weapon, m_flStartThrow, 0.0)
		set_member(weapon, m_Weapon_flNextPrimaryAttack, 0.5)
		set_member(weapon, m_Weapon_flTimeWeaponIdle, 0.75)
		set_player_var(player, "ammo", weapon, --ammo)
		if (ammo <= 0) {
			set_member(weapon, m_Weapon_flNextPrimaryAttack, 0.5)
			set_member(weapon, m_Weapon_flNextSecondaryAttack, 0.5)
			set_member(weapon, m_Weapon_flTimeWeaponIdle, 0.5)
		}
	} else if (releaseThrow > 0.0) {
		set_member(weapon, m_flStartThrow, 0.0)
		if (ammo) {
			send_weapon_anim(player, HEGRENADE_DRAW)
		} else {
			ExecuteHamB(Ham_Weapon_RetireWeapon, weapon)
			return
		}
		set_member(weapon, m_flReleaseThrow, -1.0)
		set_member(weapon, m_Weapon_flTimeWeaponIdle, random_float(10.0, 15.0))
	} else if (ammo) {
		set_member(weapon, m_Weapon_flTimeWeaponIdle, random_float(10.0, 15.0))
		send_weapon_anim(player, HEGRENADE_IDLE)
	}
}

@HeGrenade_Think(grenade) {
	/*
	if (!IsInWorld()) {
		// UTIL_Remove(this);
		return;
	}
	*/
	new Float:time = get_gametime()
	set_entvar(grenade, var_nextthink, time + 0.1)
	if (Float:get_entvar(grenade, var_dmgtime) <= time) {
		SetThink(grenade, "")
		SetTouch(grenade, "")
		set_entvar(grenade, var_flags, FL_KILLME)
		return
	}
	if (get_entvar(grenade, var_waterlevel)) {
		modifyVelocityByScalar(grenade, 0.5)
		set_entvar(grenade, var_framerate, 0.2)
	}
}

@HeGrenade_Touch(grenade, other) {
	if (get_entvar(grenade, var_owner) == other) {
		return
	}
	if (FClassnameIs(other, "func_breakable") && get_entvar(other, var_rendermode) != kRenderNormal) {
		modifyVelocityByScalar(grenade, -2.0)
		return
	}
	new flags = get_entvar(grenade, var_flags)
	if (flags & FL_ONGROUND) {
		modifyVelocityByScalar(grenade, 0.8)
	} else {
		new bounceCount = get_member(grenade, m_Grenade_iBounceCount)
		if (bounceCount < 5) {
			emit_sound(grenade, CHAN_VOICE, "weapons/he_bounce-1.wav", 0.25, ATTN_NORM, 0, PITCH_NORM)
		}
		if (bounceCount >= 10) {
			set_entvar(grenade, var_groundentity, -1) // 0 ?
			set_entvar(grenade, var_flags, flags | FL_ONGROUND)
			set_entvar(grenade, var_velocity, NULL_VECTOR)
		}
		set_member(grenade, m_Grenade_iBounceCount, ++bounceCount)
	}
	new Float:velocity[3]
	get_entvar(grenade, var_velocity, velocity)
	new Float:frameRate = vector_length(velocity) / 200.0
	if (frameRate > 1.0) {
		frameRate = 1.0
	} else if (frameRate < 0.5) {
		frameRate = 0.0
	}
	set_entvar(grenade, var_framerate, frameRate)
}

destroyItem(weapon, player) {
	const WEAPON_SUIT = 31
	if (ExecuteHamB(Ham_RemovePlayerItem, player, weapon)) {
		new weaponId = get_member(weapon, m_iId)
		new weapons = get_entvar(player, var_weapons) & ~(1<<weaponId)
		if ((weapons & ~(1<<WEAPON_SUIT)) == 0) {
			set_member(player, m_iHideHUD, get_member(player, m_iHideHUD) | HIDEHUD_WEAPONS)
		}
		set_entvar(player, var_weapons, weapons)
	}
	ExecuteHamB(Ham_Item_Kill, weapon)
}
