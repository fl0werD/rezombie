#if defined _rezombie_included
	#endinput
#endif

#define _rezombie_included

#pragma reqlib rezombie
#if !defined AMXMODX_NOAUTOLOAD
	#pragma loadlib rezombie
#endif

/**
 * Constants
 */
new const any:null = 0;
stock const TO_ALL = 0;

stock const MAX_HANDLE_LENGTH = 32;
stock const MAX_LANGKEY_LENGTH = 32;
stock const MAX_REFERENCE_LENGTH = 32;
stock const MAX_PLAYER_MODEL_LENGTH = 32;
stock const MAX_RESOURCE_PATH = 64;

stock const DEFAULT_FOV = 90;

stock const WEAPON_NOCLIP = -1;

/**
 * Game teams
 */
enum Team {
	TEAM_ZOMBIE = 1,
	TEAM_HUMAN = 2,
	TEAM_SPECTATOR,
};

/**
 * Forward return types
 */
enum {
	RZ_CONTINUE,
	RZ_SUPERCEDE,
	RZ_BREAK,
};

enum HudChannel {
	TIMER_CHANNEL,
	PLAYER_INFO_CHANNEL,
	DAMAGER_CHANNEL,
	NOTICE_CHANNEL,
};

#include <rezombie/rz_common>
#include <rezombie/rz_game_rules>
#include <rezombie/rz_modes>
#include <rezombie/rz_player>
#include <rezombie/rz_weapons>
#include <rezombie/rz_models>
#include <rezombie/rz_items>
#include <rezombie/rz_map>
#include <rezombie/rz_currency>
#include <rezombie/rz_menu>

stock const CBasePlayerWeapon_Members:m_Weapon_iRemainingShotsFired = m_Weapon_iFamasShotsFired
stock const CBasePlayerWeapon_Members:m_Weapon_flNextRemainingShoot = m_Weapon_flFamasShoot

stock register_commands(const commands[][], const handler[], const size = sizeof(commands)) {
	for (new command = 0; command < size; ++command) {
		register_clcmd(commands[command], handler)
	}
}

stock precache_sounds(const sounds[][], const size = sizeof(sounds)) {
	for (new sound = 0; sound < size; ++sound) {
		precache_sound(sounds[sound])
	}
}

/**
* You have to create len and text[] variables before call add_formatex
*/
#define add_formatex(%0)			len += formatex(text[len], charsmax(text) - len, %0)

// bad with string
#define bind_convar(%0,%1,%2)		bind_pcvar_%0(create_cvar(%2), %1)
