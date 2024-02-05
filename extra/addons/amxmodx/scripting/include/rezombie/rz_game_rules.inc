#if defined _rz_game_rules_included
	#endinput
#endif

#define _rz_game_rules_included

enum GameState {
	game_state_warmup,
	game_state_need_players,
	game_state_playing,
	game_state_game_over, // it is round state?
	game_state_intermission, // to be deleted
};

forward game_state_changed(GameState:oldGameState, GameState:newGameState);

enum RoundState {
	round_state_none,
	round_state_prepare,
	round_state_playing,
	round_state_terminate,
};

forward round_state_changed(RoundState:oldRoundState, RoundState:newRoundState);

forward @round_timer(timer);

native any:get_game_var(const var[], any:...);
native set_game_var(const var[], any:...);

forward @round_start(bool:isReset);

enum EndRoundEvent { // Status?
	end_round_none,
	end_round_warmup_end,
	end_round_game_commence,
	end_round_game_restart,
	end_round_game_over,
	end_round_humans_win,
	end_round_zombies_win,
	end_round_end_draw,
};

forward @round_end(EndRoundEvent:endRoundEvent, GameMode:gameMode, delay);

/*

boss support?

forward rz_next_map() {
	return nextMap
}

forward rz_player_impulse(player, impulse)

forward rz_weapon_primary_attack(impulse, weapon, player, clip?)

*/
