#include <amxmodx>
#include <rezombie>

// Возродить за ?

new const ADMIN_MENU_COMMANDS[][] = {
	"admin",
}

new const ADMIN_MENU_ID[] = "AdminMod"

enum LaunchModeType {
	LaunchMode_AfterPrepare,
	LaunchMode_Instantly,
	MAX_LAUNCH_MODE_TYPES
}

new const LAUNCH_MODE_TYPES[MAX_LAUNCH_MODE_TYPES][] = {
	"LAUNCH_MODE_AFTER_PREPARE",
	"LAUNCH_MODE_AFTER_INSTANTLY",
}

new const MODES_MENU_ID[] = "Modes"
new Menu:ModesMenu
new Mode:CurrentLaunchMode[MAX_PLAYERS + 1]
new LaunchModeType:CurrentLaunchModeType[MAX_PLAYERS + 1]

new const SELECT_MODE_TARGET_MENU_ID[] = "ModeTarget"
new Menu:SelectModeTargetMenu

new const CHANGE_CLASS_MENU_ID[] = "ChangeClass"
new Menu:ChangeClassMenu
new Class:CurrentChangeClass[MAX_PLAYERS + 1]

new const RESPAWN_MENU_ID[] = "Respawn"
new Menu:RespawnMenu

new const COMPLETE_ROUND_MENU_ID[] = "CompleteRound"
new Menu:CompleteRoundMenu

public plugin_precache() {
	register_plugin("Menu: Admin", "1.0.0", "fl0wer")
	add_translate("menus/admin")
}

public plugin_init() {
	adminInit()
	modesInit()
	changeClassInit()
	respawnInit()
	completeRoundInit()
	createMenuItems(ArrayCreate(1, MAX_PLAYERS))
}

public client_putinserver(player) {
	CurrentLaunchModeType[player] = LaunchMode_AfterPrepare
	CurrentChangeClass[player] = classes_begin()
}

adminInit() {
	register_menu(ADMIN_MENU_ID, "@AdminMenu")
	register_commands(ADMIN_MENU_COMMANDS, "@Command_AdminMenu")
}

@Command_AdminMenu(player) {
	openAdminMenu(player)
	return PLUGIN_HANDLED
}

openAdminMenu(player) {
	createMenu(player, ADMIN_MENU_ID)
	addText("\y%l", "ADMIN_MENU_TITLE")
	addText("^n")
	addText("^n\r%l", "ADMIN_GAME_SUBTITLE")
	addItem(MENU_KEY_1, "^n\y1. \w%l", "MODES_MENU_ITEM")
	if (get_game_var("game_state") == game_state_warmup) {
		addItem(MENU_KEY_2, "^n\y2. \w%l", "COMPLETE_WARMUP_ITEM")
	} else {
		addItem(MENU_KEY_2, "^n\y2. \w%l", "COMPLETE_ROUND_ITEM")
	}
	addText("^n")
	addText("^n\r%l", "ADMIN_PLAYERS_SUBTITLE")
	addItem(MENU_KEY_3, "^n\y3. \w%l", "CHANGE_CLASS_MENU_ITEM")
	addItem(MENU_KEY_4, "^n\y4. \w%l", "RESPAWN_MENU_ITEM")
	addText("^n")
	addMenuCloseItem()
	showMenu(player)
}

@AdminMenu(player, key) {
	if (key == MenuKey_Exit) {
		return PLUGIN_HANDLED
	}
	switch (key) {
		case 0: {
			openModesMenu(player)
		}
		case 1: {
			if (get_game_var("game_state") == game_state_warmup) {
			} else {
				openCompleteRoundMenu(player)
			}
		}
		case 2: {
			openChangeClassMenu(player)
		}
		case 3: {
			openRespawnMenu(player)
		}
	}
	return PLUGIN_HANDLED
}

modesInit() {
	ModesMenu = register_menu(MODES_MENU_ID, "@ModesMenu")
	SelectModeTargetMenu = register_menu(SELECT_MODE_TARGET_MENU_ID, "@SelectModeTargetMenu")
}

openModesMenu(player, Page:page = START_PAGE) {
	createMenu(player, MODES_MENU_ID)
	addText("\y%l", "MODES_MENU_TITLE")
	MenuItemsCount = collectAvailableModes(player)
	calculateMenuPage(player, page, 4)
	if (MenuItemsCount) {
		new Mode:mode
		new name[MAX_MODE_VAR_LENGTH]
		new desc[MAX_MODE_VAR_LENGTH]
		for (new i = MenuStart; i < MenuEnd; ++i) {
			mode = ArrayGetCell(MenuItems[player], i)
			name = get_mode_var_string(mode, "name")
			desc = get_mode_var_string(mode, "description")
			if (get_mode_var(mode, "support_target")) {
				addItem((1<<MenuItem), "^n^n\y%d. \w%l \y[%l]", MenuItem + 1, name, "MODE_WITH_TARGET")
			} else {
				addItem((1<<MenuItem), "^n^n\y%d. \w%l", MenuItem + 1, name)
			}
			addText("^n\d%l", desc)
			++MenuItem
		}
		addMenuSpacingAfterItems(player, "^n^n^n")
	} else {
		addMenuEmptyItem("^n^n")
	}
	addItem(MENU_KEY_6, "^n\r[6] \w«")
	addText( \
		" \w%l \y(%d/%d) ", \
		LAUNCH_MODE_TYPES[CurrentLaunchModeType[player]], \
		CurrentLaunchModeType[player] + any:1, MAX_LAUNCH_MODE_TYPES \
	)
	addItem(MENU_KEY_7, "\w» \r[7]")
	addText("^n")
	addMenuNavigation(player)
	showMenu(player)
}

collectAvailableModes(player) {
	new Mode:end = modes_end()
	for (new Mode:item = modes_begin(); item < end; ++item) {
		ArrayPushCell(MenuItems[player], item)
	}
	return ArraySize(MenuItems[player])
}

@ModesMenu(player, key) {
	if (key == MenuKey_Exit) {
		return PLUGIN_HANDLED
	}
	switch (key) {
		case 5: {
			if (--CurrentLaunchModeType[player] < any:0) {
				CurrentLaunchModeType[player] = MAX_LAUNCH_MODE_TYPES - any:1
			}
		}
		case 6: {
			if (++CurrentLaunchModeType[player] >= MAX_LAUNCH_MODE_TYPES) {
				CurrentLaunchModeType[player] = any:0
			}
		}
		case MenuKey_Back: {
			--MenuPage[player]
		}
		case MenuKey_Next: {
			++MenuPage[player]
		}
		default: {
			new Mode:mode = getMenuItem(player, key)
			if (get_mode_var(mode, "support_target")) {
				CurrentLaunchMode[player] = mode
				openSelectModeTargetMenu(player)
			} else {
				launch_mode(mode)
			}
			return PLUGIN_HANDLED
		}
	}
	openModesMenu(player, MenuPage[player])
	return PLUGIN_HANDLED
}

openSelectModeTargetMenu(player, Page:page = START_PAGE) {
	new name[MAX_MODE_VAR_LENGTH]
	name = get_mode_var_string(CurrentLaunchMode[player], "name")
	createMenu(player, SELECT_MODE_TARGET_MENU_ID)
	addText("\y%l", "SELECT_MODE_TARGET_TITLE")
	addText("^n\w%l", "SELECT_MODE_TARGET_SUBTITLE", name)
	addText("^n")
	MenuItemsCount = collectAvailableModeTargets(player)
	calculateMenuPage(player, page, 6)
	if (MenuItemsCount) {
		new target
		for (new i = MenuStart; i < MenuEnd; ++i) {
			target = ArrayGetCell(MenuItems[player], i)
			addItem((1<<MenuItem), "^n\y%d. \w%n", MenuItem + 1, target)
			++MenuItem
		}
		addMenuSpacingAfterItems(player)
	} else {
		addMenuEmptyItem()
	}
	addText("^n")
	if (MenuItemsCount) {
		addItem(MENU_KEY_7, "^n\r7. \y%l", "SELECT_MODE_TARGET_RANDOM")
	} else {
		addText("^n\d7. %l", "SELECT_MODE_TARGET_RANDOM")
	}
	addText("^n")
	addMenuNavigation(player)
	showMenu(player)
}

collectAvailableModeTargets(player) {
	for (new item = 1; item <= MaxClients; ++item) {
		if (!is_user_alive(item)) {
			continue
		}
		ArrayPushCell(MenuItems[player], item)
	}
	return ArraySize(MenuItems[player])
}

@SelectModeTargetMenu(player, key) {
	if (key == MenuKey_Exit) {
		return PLUGIN_HANDLED
	}
	switch (key) {
		case 6: {
			new target = random(ArraySize(MenuItems[player]))
			launch_mode(CurrentLaunchMode[player], target)
			return PLUGIN_HANDLED
		}
		case MenuKey_Back: {
			--MenuPage[player]
		}
		case MenuKey_Next: {
			++MenuPage[player]
		}
		default: {
			new target = getMenuItem(player, key)
			launch_mode(CurrentLaunchMode[player], target)
			return PLUGIN_HANDLED
		}
	}
	openSelectModeTargetMenu(player, MenuPage[player])
	return PLUGIN_HANDLED
}

changeClassInit() {
	register_menu(CHANGE_CLASS_MENU_ID, "@ChangeClassMenu")
}

openChangeClassMenu(player, Page:page = START_PAGE) {
	createMenu(player, CHANGE_CLASS_MENU_ID)
	addText("\y%l", "CHANGE_CLASS_MENU_TITLE")
	addText("^n")
	MenuItemsCount = collectAvailableChangeClass(player, CurrentChangeClass[player])
	calculateMenuPage(player, page, 5)
	if (MenuItemsCount) {
		new target
		new Class:class
		for (new i = MenuStart; i < MenuEnd; ++i) {
			target = ArrayGetCell(MenuItems[player], i)
			class = get_player_var(target, "class")
			addItem( \
				(1<<MenuItem), "^n\y%d. \w%n %s[%l]", \
				MenuItem + 1, target, menuTeamColor(class), get_class_var_string(class, "name") \
			)
			++MenuItem
		}
		addMenuSpacingAfterItems(player)
	} else {
		addMenuEmptyItem()
	}
	addText("^n")
	addItem(MENU_KEY_6, "^n\r[6] \w«")
	addText( \
		" \w%l %s%l \y(%d/%d) ", \
		"TURN_INTO_CLASS_ITEM", \
		menuTeamColor(CurrentChangeClass[player]), \
		get_class_var_string(CurrentChangeClass[player], "name"), \
		CurrentChangeClass[player] - classes_begin() + any:1, classes_count() \
	)
	addItem(MENU_KEY_7, "\w» \r[7]")
	addText("^n")
	addMenuNavigation(player)
	showMenu(player)
}

collectAvailableChangeClass(player, Class:filterClass) {
	for (new item = 1; item <= MaxClients; ++item) {
		if (!is_user_alive(item)) {
			continue
		}
		if (get_player_var(item, "class") == filterClass) {
			continue
		}
		ArrayPushCell(MenuItems[player], item)
	}
	return ArraySize(MenuItems[player])
}

@ChangeClassMenu(player, key) {
	if (key == MenuKey_Exit) {
		return PLUGIN_HANDLED
	}
	switch (key) {
		case 5: {
			if (--CurrentChangeClass[player] < classes_begin()) {
				CurrentChangeClass[player] = classes_end() - any:1
			}
		}
		case 6: {
			if (++CurrentChangeClass[player] >= classes_end()) {
				CurrentChangeClass[player] = classes_begin()
			}
		}
		case MenuKey_Back: {
			--MenuPage[player]
		}
		case MenuKey_Next: {
			++MenuPage[player]
		}
		default: {
			new target = getMenuItem(player, key)
			change_player_class(target, CurrentChangeClass[player])
			client_print(0, print_chat, "%n", target)
		}
	}
	openChangeClassMenu(player, MenuPage[player])
	return PLUGIN_HANDLED
}

menuTeamColor(Class:class) {
	new color[3]
	switch (get_class_var(class, "team")) {
		case TEAM_HUMAN: {
			color = "\y"
		}
		case TEAM_ZOMBIE: {
			color = "\r"
		}
		default: {
			color = "\d"
		}
	}
	return color
}

respawnInit() {
	RespawnMenu = register_menu(RESPAWN_MENU_ID, "@RespawnMenu")
}

openRespawnMenu(player, Page:page = START_PAGE) {
	createMenu(player, RESPAWN_MENU_ID)
	MenuItemsCount = collectAvailableRespawn(player)
	addText("\y%l", "RESPAWN_MENU_TITLE")
	if (MenuItemsCount) {
		addText("^n\w%l", "RESPAWN_MENU_SUBTITLE", MenuItemsCount)
	}
	addText("^n")
	calculateMenuPage(player, page, 4)
	if (MenuItemsCount) {
		new target
		for (new i = MenuStart; i < MenuEnd; ++i) {
			target = ArrayGetCell(MenuItems[player], i)
			addItem((1<<MenuItem), "^n\y%d. \w%n", MenuItem + 1, target)
			++MenuItem
		}
		addMenuSpacingAfterItems(player)
	} else {
		addMenuEmptyItem()
	}
	addMenuNavigation(player)
	showMenu(player)
}

collectAvailableRespawn(player) {
	for (new item = 1; item <= MaxClients; ++item) {
		if (!is_user_connected(item)) {
			continue
		}
		if (is_user_alive(item)) {
			continue
		}
		ArrayPushCell(MenuItems[player], item)
	}
	return ArraySize(MenuItems[player])
}

@RespawnMenu(player, key) {
	if (key == MenuKey_Exit) {
		return PLUGIN_HANDLED
	}
	switch (key) {
		case MenuKey_Back: {
			--MenuPage[player]
		}
		case MenuKey_Next: {
			++MenuPage[player]
		}
		default: {
			new target = getMenuItem(player, key)
			client_print(0, print_chat, "%n", target)
		}
	}
	openRespawnMenu(player, MenuPage[player])
	return PLUGIN_HANDLED
}

completeRoundInit() {
	CompleteRoundMenu = register_menu(COMPLETE_ROUND_MENU_ID, "@CompleteRoundMenu")
}

openCompleteRoundMenu(player) {
	if (get_game_var("game_state") == game_state_warmup) {
		openAdminMenu(player)
		return
	}
	createMenu(player, COMPLETE_ROUND_MENU_ID)
	addText("\y%l", "COMPLETE_ROUND_TITLE")
	addText("^n")
	addItem(MENU_KEY_1, "^n\y1. \wЗа людей")
	addItem(MENU_KEY_2, "^n\y2. \wЗа зомби")
	addItem(MENU_KEY_3, "^n\y3. \wНичья") // ?
	addText("^n")
	addMenuCloseItem()
	showMenu(player)
}

@CompleteRoundMenu(player, key) {
	if (key == MenuKey_Exit) {
		return PLUGIN_HANDLED
	}
	if (get_game_var("game_state") == game_state_warmup) {
		openAdminMenu(player)
		return PLUGIN_HANDLED
	}
	switch (key) {
		case 0: {
		}
		case 1: {
		}
		case 2: {
		}
	}
	return PLUGIN_HANDLED
}

updatePlayersMenus() {
	new Menu:menu
	for (new player = 1; player <= MaxClients; ++player) {
		if (!is_user_connected(player)) {
			continue
		}
		menu = get_opened_menu(player)
		if (menu == ModesMenu) {
			openModesMenu(player, MenuPage[player])
		} else if (menu == ChangeClassMenu) {
			openChangeClassMenu(player, MenuPage[player])
		} else if (menu == RespawnMenu) {
			openRespawnMenu(player, MenuPage[player])
		} else if (menu == EndRoundMenu) {
			openEndRoundMenu(player, MenuPage[player])
		}
	}
}
