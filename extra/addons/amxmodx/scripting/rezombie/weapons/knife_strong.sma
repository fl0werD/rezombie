#include <amxmodx>
#include <hamsandwich>
#include <fakemeta>
#include <reapi>
#include <rezombie>

const STRONG_PRIMARY_DAMAGE = 75
const STRONG_PRIMARY_DISTANCE = 32
const STRONG_SECONDARY_DAMAGE = 200
const STRONG_SECONDARY_DISTANCE = 64
const Float:SRTONG_SECONDARY_BACK_DAMAGE_MULTIPLIER = 3.0
const STRONG_MAX_SPEED = 250
const STRONG_WEIGHT = 5

enum {
	STRONG_IDLE,
	STRONG_SLASH1,
	STRONG_SLASH2,
	STRONG_DRAW,
	STRONG_STAB,
	STRONG_STAB_MISS,
	STRONG_MIDSLASH1,
	STRONG_MIDSLASH2,
}

new const DRAW_SOUND[] = "rezombie/weapons/knife_strong/draw.wav"
new const HIT_SOUNDS[][] = {
	"rezombie/weapons/knife_strong/hit1.wav",
	"rezombie/weapons/knife_strong/hit2.wav",
}
new const SLASH_SOUND[] = "rezombie/weapons/knife_strong/slash.wav"
new const STAB_SOUND[] = "rezombie/weapons/knife_strong/stab.wav"
new const WALL_SOUND[] = "rezombie/weapons/knife_strong/wall.wav"
new const HUD_SPRITE[] = "sprites/rezombie/knife_strong/640hud23.spr"

public plugin_precache() {
	register_plugin("Knife: Strong", "1.0.0", "fl0wer")

	new Weapon:weapon = create_weapon("knife_strong", weapon_type_melee)
	set_weapon_var(weapon, "name", "KNIFE_STRONG")
	set_weapon_var(weapon, "view_model", create_model("rezombie/weapons/knife_strong/v_knife.mdl"))
	//set_weapon_var(weapon, "player_model", create_model("models/weapons/knife_strong/p.mdl"))
	set_weapon_var(weapon, "hud", "rezombie/knife_strong/hud")
	//set_weapon_var(weapon, "max_clip", 20)
	//set_weapon_var(weapon, "max_ammo", 120)
	//set_weapon_var(weapon, "slot", PRIMARY_WEAPON_SLOT)
	//set_weapon_var(weapon, "weight", GLOCK18_WEIGHT)
	//set_weapon_var(weapon, "crosshair_size", CrosshairSize_Size8)
	set_weapon_var(weapon, "forward_deploy", "@Strong_Deploy")
	set_weapon_var(weapon, "forward_max_speed", "@Strong_MaxSpeed")
	set_weapon_var(weapon, "forward_primary_attack", "@Strong_PrimaryAttack")
	set_weapon_var(weapon, "forward_secondary_attack", "@Strong_SecondaryAttack")
	//set_weapon_var(weapon, "forward_idle", "@Glock18_Idle")

	precache_sound(DRAW_SOUND)
	precache_sounds(HIT_SOUNDS)
	precache_sound(SLASH_SOUND)
	precache_sound(STAB_SOUND)
	precache_sound(WALL_SOUND)
	precache_model(HUD_SPRITE)
}

@Strong_Deploy(weapon, player) {
	new bool:result = weapon_default_deploy(weapon, player, STRONG_DRAW, "knife")
	if (result) {
		set_member(player, m_flNextAttack, 1.0)
		rh_emit_sound2(player, 0, CHAN_ITEM, DRAW_SOUND)
	}
	return result
}

@Strong_MaxSpeed(weapon, player) {
	return STRONG_MAX_SPEED
}

@Strong_PrimaryAttack(weapon, player) {
	new MeleeAttackResult:result = melee_default_attack(
		weapon,
		player,
		STRONG_PRIMARY_DAMAGE,
		STRONG_PRIMARY_DISTANCE
	)
	new Float:nextAttack
	new Float:idleTime
	switch (result) {
		case melee_attack_miss: {
			nextAttack = idleTime = 2.2
			send_weapon_anim(player, random_num(STRONG_SLASH1, STRONG_SLASH2))
			play_weapon_sound(player, SLASH_SOUND)
		}
		case melee_attack_hit: {
			nextAttack = idleTime = 1.2
			send_weapon_anim(player, random_num(STRONG_MIDSLASH1, STRONG_MIDSLASH2))
			play_random_weapon_sound(player, HIT_SOUNDS)
		}
		case melee_attack_hit_wall: {
			nextAttack = idleTime = 1.2
			send_weapon_anim(player, random_num(STRONG_MIDSLASH1, STRONG_MIDSLASH2))
			play_weapon_sound(player, WALL_SOUND)
		}
	}
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, idleTime)
	return result
}

@Strong_SecondaryAttack(weapon, player) {
	new MeleeAttackResult:result = melee_default_attack(
		weapon,
		player,
		STRONG_SECONDARY_DAMAGE,
		STRONG_SECONDARY_DISTANCE,
		SRTONG_SECONDARY_BACK_DAMAGE_MULTIPLIER
	)
	new Float:nextAttack
	new Float:idleTime
	switch (result) {
		case melee_attack_miss: {
			nextAttack = idleTime = 1.75
			send_weapon_anim(player, STRONG_STAB_MISS)
			play_weapon_sound(player, SLASH_SOUND)
		}
		case melee_attack_hit: {
			nextAttack = idleTime = 2.0
			send_weapon_anim(player, STRONG_STAB)
			play_weapon_sound(player, STAB_SOUND)
		}
		case melee_attack_hit_wall: {
			nextAttack = idleTime = 2.0
			send_weapon_anim(player, STRONG_STAB)
			play_weapon_sound(player, WALL_SOUND)
		}
	}
	set_member(weapon, m_Weapon_flNextPrimaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flNextSecondaryAttack, nextAttack)
	set_member(weapon, m_Weapon_flTimeWeaponIdle, idleTime)
	return result
}

@Glock18_Idle(weapon, player, Float:idleTime) {
	// need rework after grenades
	/*ExecuteHamB(Ham_Weapon_ResetEmptySound, weapon)
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
	send_weapon_anim(player, anim)*/
}
