#include <amxmodx>
#include <hamsandwich>
#include <reapi>
#include <rezombie>

// do menu dynamically

new const ADMIN_MENU_ACCESS_FLAGS[] = "b"

new const GAME_MENU_COMMANDS[][] = {
	"chooseteam",
	"gamemenu",
	"say /menu",
}

new const GAME_MENU_ID[] = "RZ_GameMenu"

new Menu:MenuGame

new Class:HumanClass
new Class:ZombieClass

public plugin_precache() {
	register_plugin("Menu: Game", "1.0.0", "fl0wer")
	add_translate("menus/game")
}

public plugin_init() {
	register_commands(GAME_MENU_COMMANDS, "@Command_GameMenu")
	MenuGame = register_menu(GAME_MENU_ID, "@Menu_Game")
	createMenuItems(ArrayCreate())
	HumanClass = checkClassExists("human")
	ZombieClass = checkClassExists("zombie")
}

@Command_GameMenu(player) {
	if (is_nullent(player)) {
		return PLUGIN_CONTINUE
	}
	if (get_member(player, m_iJoiningState) != JOINED) {
		return PLUGIN_CONTINUE
	}
	if (get_opened_menu(player) != MenuGame) {
		openGameMenu(player)
	} else {
		closeMenu(player)
	}
	return PLUGIN_HANDLED
}

openGameMenu(player) {
	new bool:isTerminateRoundState = get_game_var("round_state") == round_state_terminate
	new bool:isAlive = bool:is_user_alive(player)
	createMenu(player, GAME_MENU_ID)
	addText("\y%l", "GAME_MENU_TITLE")
	addText("^n")
	addItem(MENU_KEY_1, "^n\y1. \w%l", "GAME_MENU_SELECT_WPNS")
	if (!isTerminateRoundState && isAlive) {
		addItem(MENU_KEY_2, "^n\y2. \w%l", "GAME_MENU_BUY_EXTRA") // add available items count 
	} else {
		addText("^n\d2. %l", "GAME_MENU_BUY_EXTRA")
	}
	addItem(MENU_KEY_3, "^n\y3. \wСменить подкласс %l", get_class_var_string(HumanClass, "name"))
	addItem(MENU_KEY_4, "^n\y4. \wСменить подкласс %l", get_class_var_string(ZombieClass, "name"))
	// check last
	if (get_member(player, m_iTeam) == TEAM_SPECTATOR) {
		addItem(MENU_KEY_5, "^n\y5. \w%l", "GAME_MENU_JOIN_GAME")
	} else {
		addItem(MENU_KEY_5, "^n\y5. \w%l", "GAME_MENU_JOIN_SPECS")
	}
	addText("^n")
	addText("^n")
	// always can click
	// every item "IsAvaliable" and in handler
	if (get_user_flags(player) & read_flags(ADMIN_MENU_ACCESS_FLAGS)) {
		addItem(MENU_KEY_9, "^n\y9. \w%l", "ADMIN_MENU_ITEM")
	}
	addText("^n")
	addItem(MENU_KEY_0, "^n\r[0] \w%l \y(M)", "CLOSE")
	addText("^n")
	addText("^n\d%l", "YOUR_AUTH_ID", getAuthId(player))
	showMenu(player)
}

@Menu_Game(player, key) {
	gameMenuHandler(player, ++key)
	return PLUGIN_HANDLED
}

gameMenuHandler(player, key) {
	if (key == 10) {
		return
	}
	switch (key) {
		case 1: {
			amxclient_cmd(player, "guns")
		}
		case 2: {
			amxclient_cmd(player, "items")
		}
		case 3: {
			amxclient_cmd(player, "human")
		}
		case 4: {
			amxclient_cmd(player, "zombie")
		}
		case 5: {
			if (get_member(player, m_iTeam) == TEAM_SPECTATOR) {
				rg_set_user_team(player, TEAM_CT, .check_win_conditions = true)
			} else {
				if (is_user_alive(player)) {
					ExecuteHamB(Ham_Killed, player, player, GIB_NEVER)
				}
				rg_set_user_team(player, TEAM_SPECTATOR, .check_win_conditions = true)
			}
		}
		case 9: {
			amxclient_cmd(player, "admin")
		}
	}
}

getAuthId(player) {
	new authId[MAX_AUTHID_LENGTH]
	get_user_authid(player, authId, charsmax(authId))
	return authId
}
