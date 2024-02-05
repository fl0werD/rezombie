#if defined _rz_game_modes_included
	#endinput
#endif

#define _rz_game_modes_included

enum RespawnType {
	Respawn_Off,
	Respawn_ToHumansTeam,
	Respawn_ToZombiesTeam,
	Respawn_Balance,
};

native Mode:create_mode(const handle[], const luanchForward[], bool:isSupportTarget = false);

native any:get_mode_var(Mode:mode, const var[], any:...);
native set_mode_var(Mode:mode, const var[], any:...);

const MAX_MODE_VAR_LENGTH = 64;

stock get_mode_var_string(Mode:mode, const var[]) {
	new value[MAX_MODE_VAR_LENGTH];
	get_mode_var(mode, var, value, charsmax(value));
	return value;
}

native mode_add_ambience_sound(Mode:mode, const path[]);
native [PLATFORM_MAX_PATH]mode_random_ambience_sound(Mode:mode);

native modes_count();
native Mode:modes_begin();
native Mode:modes_end();
native Mode:find_mode(const handle[]);

native launch_mode(Mode:mode, target = 0);

forward @mode_start(Mode:mode, target);

stock Mode:checkModeExists(const handle[]) {
	new Mode:mode = find_mode(handle);
	if (mode == null) {
		log(Log_Warning, "Mode '%s' not found", handle);
	}
	return mode;
}
