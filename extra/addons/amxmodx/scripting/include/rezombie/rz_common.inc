#if defined _rz_common_included
	#endinput
#endif

#define _rz_common_included

native register_translate(const translateFile[]);

native bool:add_translate(const name[]);

const MAX_PLURAL_LENGTH = 64;

enum Plurals {
	Plural_One,
	Plural_Few,
	Plural_Many,
};

stock quantity(number, const plurals[Plurals][MAX_PLURAL_LENGTH]) {
	new Plurals:plural = Plural_Many;
	if (number > 10 && ((number % 100) / 10) == 1) {
		plural = Plural_Many;
	} else {
		switch (number % 10) {
			case 1: plural = Plural_One;
			case 2, 3, 4: plural = Plural_Few;
		}
	}
	return plurals[plural];
}

native create_color(const name[], const color[])

enum LogLevel {
	Log_Fatal = 100,
	Log_Error = 200,
	Log_Warning = 300,
	Log_Info = 400,
};

native log(LogLevel:logLevel, const message[], any:...);

native print_to_chat(player, sender, const text[], any:...)

stock Array:get_alive_players(&alivesCount = 0) {
	static Array:AlivesArray = Invalid_Array;
	if (AlivesArray == Invalid_Array) {
		AlivesArray = ArrayCreate(1);
	} else {
		ArrayClear(AlivesArray);
	}
	for (new player = 1; player <= MaxClients; ++player) {
		if (!is_user_alive(player)) {
			continue;
		}
		ArrayPushCell(AlivesArray, player);
	}
	alivesCount = ArraySize(AlivesArray);
	return AlivesArray;
}

enum PlayersCountFlags (<<=1) {
	PlayersCount_Alive = (1<<0),
	PlayersCount_Dead,
	PlayersCount_Humans,
	PlayersCount_Zombies,
};

native get_players_count(PlayersCountFlags:flags = any:0);

native play_ambience_sound(const soundPath[]);
native stop_ambience_sound();

native bool:is_extra_exists(ExtraType:type, const key[]);

native any:get_extra(ExtraType:type, const key[], any:...);
native set_extra(ExtraType:type, const key[], any:...);

stock get_extra_string(const key[]) {
	new value[MAX_EXTRA_VAR_LENGTH];
	get_extra(extra_string, key, value, charsmax(value));
	return value;
}

native send_death_msg(receiver, attacker, victim, const weaponName[], bool:isHeadshot = false);
native send_score_attrib(receiver, player, attribFlags = 0);
native send_item_pickup(receiver, const className[]);
native send_money(receiver, amount, bool:isTrackChange = true);
native send_status_icon(receiver, status, const sprite[], const color[] = "");

native message_beam_follow(entity, spriteIndex, life, lineWidth, const color[]);
