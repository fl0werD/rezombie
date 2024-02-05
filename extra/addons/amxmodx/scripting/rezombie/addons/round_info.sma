#include <amxmodx>
#include <reapi>
#include <rezombie>
/*
new RGB_WHITE[3] = { 255, 255, 255 }
new RGB_HUMAN[3] = { 0, 50, 255 }
new RGB_ZOMBIE[3] = { 255, 150, 0 }

new RGBA_WHITE[4] = { 255, 255, 255, 255 }
new RGBA_RED[4] = { 255, 50, 0, 255 }
new RGBA_HUMAN[4] = { 0, 50, 255, 255 }
new RGBA_ZOMBIE[4] = { 255, 150, 0, 255 }
*/
// 1 vs %d

// timer dhud?

new Mode:SurvivorMode
new Mode:NemesisMode
new Mode:SniperMode
new Mode:AssassinMode
new Mode:SwarmMode
new Mode:PlagueMode
new Mode:ArmageddonMode
new Mode:SvaMode
new Class:SurvivorClass
new Class:NemesisClass
new Class:SniperClass
new Class:AssassinClass
new CurrentModeClassTarget

new Currency:MoneyCurrency
new Currency:AmmoPacksCurrency

// add reset target

public plugin_init() {
	register_plugin("Round Info", "1.0.0", "fl0wer")

	SurvivorMode = checkModeExists("survivor")
	NemesisMode = checkModeExists("nemesis")
	SniperMode = checkModeExists("sniper")
	AssassinMode = checkModeExists("assassin")
	SwarmMode = checkModeExists("swarm")
	PlagueMode = checkModeExists("plague")
	ArmageddonMode = checkModeExists("armageddon")
	SvaMode = checkModeExists("sva")
	SurvivorClass = checkClassExists("survivor")
	NemesisClass = checkClassExists("nemesis")
	SniperClass = checkClassExists("sniper")
	AssassinClass = checkClassExists("assassin")
	MoneyCurrency = checkCurrencyExists("money")
	AmmoPacksCurrency = checkCurrencyExists("ammo_packs")
}

new const SOUND_WIN_HUMANS[][] = { "ambience/guit1.wav", "ambience/guit1.wav" };
new const SOUND_WIN_ZOMBIES[][] = { "ambience/guit1.wav", "ambience/guit1.wav", "ambience/guit1.wav" };

public plugin_precache() {
	precache_sound(SOUND_WIN_HUMANS[0])
	precache_sound(SOUND_WIN_HUMANS[1])
	precache_sound(SOUND_WIN_ZOMBIES[0])
	precache_sound(SOUND_WIN_ZOMBIES[1])
	precache_sound(SOUND_WIN_ZOMBIES[2])
}

@game_state_changed(GameState:oldState, GameState:newState) {
	server_print("Game STATE: %d %d", oldState, newState)
}

@round_state_changed(RoundState:oldState, RoundState:newState) {
	server_print("Round STATE: %d %d", oldState, newState)
	if (newState == round_state_prepare) {
		stop_ambience_sound()
	}
}

@mode_start(Mode:mode, target) {
	new sound[PLATFORM_MAX_PATH]
	sound = mode_random_ambience_sound(mode)
	if (sound[0]) {
		play_ambience_sound(sound)
	}
	new Float:holdTime = 5.0
	send_large_hud(
		TO_ALL,
		large_hud_params(-1.0, 0.2, get_mode_var_string(mode, "color"), .holdTime = holdTime),
		"%l", get_mode_var_string(mode, "name")
	)
	new descHudParams[HudParams]
	descHudParams = hud_params(
		.x = -1.0,
		.y = 0.24,
		.color1 = "white",
		.color2 = "white",
		.holdTime = holdTime
	)
	new noticeMessage[MAX_MODE_VAR_LENGTH]
	get_mode_var(mode, "notice_message", noticeMessage, charsmax(noticeMessage))
	if (noticeMessage[0]) {
		if (target) {
			send_hud(TO_ALL, NOTICE_CHANNEL, descHudParams, "%l", noticeMessage, target)
		} else {
			send_hud(TO_ALL, NOTICE_CHANNEL, descHudParams, "%l", noticeMessage)
		}
	}
}

@round_end(EndRoundEvent:endRoundEvent, GameMode:gameMode, delay) {
	new color[32]
	new text[128]
	for (new player = 1; player <= MaxClients; ++player) {
		if (!is_user_connected(player)) {
			continue
		}
		switch (endRoundEvent) {
			case end_round_warmup_end: {
				color = "white"
				text = "Разминка окончена!"
			}
			case end_round_game_commence: {
				color = "white"
				text = "Игроки подключились!"
			}
			case end_round_game_restart: {
				color = "white"
				text = "Рестарт игры!"
			}
			case end_round_game_over: {
				color = "white"
				text = "Игра окончена!"
			}
			case end_round_humans_win: {
				switch (get_player_var(player, "team")) {
					case TEAM_HUMAN: {
						color = "green"
						text = "ROUND_VICTORY"
						add_player_currency(player, MoneyCurrency, 2000, "за победу")
					}
					case TEAM_ZOMBIE: {
						color = "red"
						text = "ROUND_DEFEAT"
						add_player_currency(player, AmmoPacksCurrency, 2, "за поражение")
					}
					default: {
						color = "blue"
						text = "ROUND_HUMANS_WIN"
					}
				}
			}
			case end_round_zombies_win: {
				switch (get_player_var(player, "team")) {
					case TEAM_HUMAN: {
						color = "red"
						text = "ROUND_DEFEAT"
						add_player_currency(player, MoneyCurrency, 500, "за поражение")
					}
					case TEAM_ZOMBIE: {
						color = "green"
						text = "ROUND_VICTORY"
						add_player_currency(player, AmmoPacksCurrency, 5, "за победу")
					}
					default: {
						color = "blue"
						text = "ROUND_ZOMBIES_WIN"
					}
				}
			}
			case end_round_end_draw: {
			}
			default: {
				continue
			}
		}
		send_large_hud(player, large_hud_params(-1.0, 0.2, color, .holdTime = float(delay)), "%l", text)
	}
	switch (endRoundEvent) {
		case end_round_warmup_end: {
		}
		case end_round_game_commence: {
		}
		case end_round_game_restart: {
		}
		case end_round_game_over: {
		}
		case end_round_humans_win: {
			play_ambience_sound(SOUND_WIN_HUMANS[random(sizeof(SOUND_WIN_HUMANS))])
		}
		case end_round_zombies_win: {
			play_ambience_sound(SOUND_WIN_ZOMBIES[random(sizeof(SOUND_WIN_ZOMBIES))])
		}
		case end_round_end_draw: {
		}
	}
}

@round_timer(timer) {
	new Float:holdTime = 1.1
	switch (get_game_var("game_state")) {
		case game_state_warmup: {
			if (timer == 0) {
				return
			}
			if (get_game_var("round_state") != round_state_terminate) {
				send_hud(
					TO_ALL,
					TIMER_CHANNEL,
					hud_params(-1.0, 0.02, "white", "white", .holdTime = holdTime),
					"РАЗМИНКА %d:%02d", timer / 60, timer % 60
				)
			} else {
				send_hud(
					TO_ALL,
					TIMER_CHANNEL,
					hud_params(-1.0, 0.2, "red", "red", .holdTime = holdTime),
					"МАТЧ НАЧНЕТСЯ ЧЕРЕЗ %d...", timer
				)
			}
		}
		case game_state_need_players: {
			send_hud(
				TO_ALL,
				TIMER_CHANNEL,
				hud_params(-1.0, 0.02, "white", "white", .holdTime = holdTime),
				"ОЖИДАНИЕ ИГРОКОВ"
			)
		}
		case game_state_playing: {
/*
			send_hud(
				TO_ALL,
				TIMER_CHANNEL,
				hud_params(-1.0, 0.01, "white", "white", .holdTime = holdTime),
				"%02d     |   %s   |    %02d^n\
				людей | %02d | %02d | зомби%s",
				humansAlive, roundTimer, zombiesAlive,
				humansWins, zombiesWins,
				modeFormat
			)
*/
			new RoundState:roundState = get_game_var("round_state")
			switch (roundState) {
				case round_state_prepare: {
					send_hud(
						TO_ALL,
						TIMER_CHANNEL,
						hud_params(-1.0, 0.01, "white", "white", .holdTime = holdTime),
						"%02d люди | зомби %02d^n\
						людей Ω: %d/%d^n\
						заражения нет",
						get_game_var("team_wins", TEAM_HUMAN), get_game_var("team_wins", TEAM_ZOMBIE),
						get_players_count(), MaxClients
					)
				}
				case round_state_playing: {
					new humansAlive = get_players_count(PlayersCount_Alive | PlayersCount_Humans)
					new zombiesAlive = get_players_count(PlayersCount_Alive | PlayersCount_Zombies)
					send_hud(
						TO_ALL,
						TIMER_CHANNEL,
						hud_params(-1.0, 0.01, "white", "white", .holdTime = holdTime),
						"%02d люди [ %d:%02d ] зомби %02d^n\
						%02d Ω | Ω %02d^n\
						%s",
						get_game_var("team_wins", TEAM_HUMAN), timer / 60, timer % 60, get_game_var("team_wins", TEAM_ZOMBIE),
						humansAlive, zombiesAlive,
						getModeFormat(get_game_var("mode"))
					)
				}
				case round_state_terminate: {
					send_hud(
						TO_ALL,
						TIMER_CHANNEL,
						hud_params(-1.0, 0.01, "white", "white", .holdTime = holdTime),
						"%02d люди | зомби %02d",
						get_game_var("team_wins", TEAM_HUMAN), get_game_var("team_wins", TEAM_ZOMBIE)
					)
				}
			}

			if (roundState == round_state_prepare) {
				if (timer) {
					client_print(0, print_center, "Заражение начнется через %d сек.", timer)
				} else {
					client_print(0, print_center, "")
				}
			}
		}
		case game_state_game_over: {
			send_hud(
				TO_ALL,
				TIMER_CHANNEL,
				hud_params(-1.0, 0.0, "white", "white", .holdTime = holdTime),
				"Игра окончена!^nСмена карты через %d сек.^nСледующая карта: $2000$",
				timer
			)
		}
	}
}

getModeFormat(Mode:mode) {
	new modeFormat[MAX_MODE_VAR_LENGTH]
	new Class:class
	if (mode == SurvivorMode) {
		class = SurvivorClass
	} else if (mode == NemesisMode) {
		class = NemesisClass
	} else if (mode == SniperMode) {
		class = SniperClass
	} else if (mode == AssassinMode) {
		class = AssassinClass
	} else if (mode == PlagueMode) {
		new survivors = getModeTargets(SurvivorClass)
		new nemeses = getModeTargets(NemesisClass)
		if (survivors || nemeses) {
			formatex(
				modeFormat, charsmax(modeFormat),
				"%L (%d %L, %d %L)",
				LANG_PLAYER, get_mode_var_string(mode, "name"),
				survivors, LANG_PLAYER, get_class_var_string(SurvivorClass, "name"),
				nemeses, LANG_PLAYER, get_class_var_string(NemesisClass, "name")
			)
		}
	}
	if (class) {
		if (!CurrentModeClassTarget || !is_user_alive(CurrentModeClassTarget) || get_player_var(CurrentModeClassTarget, "class") != class) {
			CurrentModeClassTarget = getModeSingleTarget(class)
		}
		if (CurrentModeClassTarget) {
			formatex(
				modeFormat, charsmax(modeFormat),
				"%L: %n - %d HP",
				LANG_PLAYER, get_class_var_string(class, "name"), CurrentModeClassTarget, get_player_var(CurrentModeClassTarget, "health")
			)
		}
	}
	if (!modeFormat[0] && mode) {
		formatex(modeFormat, charsmax(modeFormat), "%L", LANG_PLAYER, get_mode_var_string(mode, "name"))
	}
	return modeFormat
}

getModeSingleTarget(Class:class) {
	new candidate
	new candidatesCount
	for (new player = 1; player <= MaxClients; ++player) {
		if (!is_user_alive(player)) {
			continue
		}
		if (get_player_var(player, "class") != class) {
			continue
		}
		candidate = player
		candidatesCount++
	}
	if (candidatesCount != 1) {
		return 0
	}
	return candidate
}

getModeTargets(Class:class) {
	new candidatesCount
	for (new player = 1; player <= MaxClients; ++player) {
		if (!is_user_alive(player)) {
			continue
		}
		if (get_player_var(player, "class") != class) {
			continue
		}
		candidatesCount++
	}
	return candidatesCount
}
