#include <amxmodx>
#include <rezombie>

/**
 * Start settings
 */

new const PRIMARY_WEAPONS[][] = {
	"csgo_p90",
	"csgo_m4a1s",
	"csgo_ak47",
	"csgo_awp",
	"csgo_negev",
}

new const SECONDARY_WEAPONS[][] = {
	"csgo_glock",
	"csgo_usp",
	"csgo_deagle",
}

/**
 * End of settings
 */

new const PRIMARY_MENU_ID[] = "PrimaryWeapon"
new bool:IsRememberPrimary[MAX_PLAYERS + 1]
new Weapon:PrimaryWeapon[MAX_PLAYERS + 1]
new Array:Primaries

new const SECONDARY_MENU_ID[] = "SecondaryWeapon"
new bool:IsRememberSecondary[MAX_PLAYERS + 1]
new Weapon:SecondaryWeapon[MAX_PLAYERS + 1]
new Array:Secondaries

new Class:HumanClass

/*
	1. M4A1-S
	2. M4A4
	3. AK-47
	4. MAC-10
	5. MP7
	6. MP9
	7. MP5-SD
	8. PP-Bizon
	9. P90
	10. UMP-45

	1. Glock
	2. USP-S
	3. 
	4. 
	5. Desert Eagle
	6. Dual Berettas
*/

public plugin_precache() {
	register_plugin("Human Weapons", "1.0.0", "fl0wer")
}

public plugin_init() {
	HumanClass = checkClassExists("human")
	register_menu(PRIMARY_MENU_ID, "@Menu_Primary")
	register_menu(SECONDARY_MENU_ID, "@Menu_Secondary")
	register_clcmd("radio3", "@Command_Weapons") // to be deleted
	parsePrimary()
	parseSecondary()
	if (!ArraySize(Primaries) && !ArraySize(Secondaries)) {
		log(Log_Error, "No weapons found")
	}
}

public client_putinserver(player) {
	if (is_user_bot(player)) {
		IsRememberPrimary[player] = true
		IsRememberSecondary[player] = true
		PrimaryWeapon[player] = ArrayGetCell(Primaries, 0)
		SecondaryWeapon[player] = ArrayGetCell(Secondaries, 0)
	} else {
		IsRememberPrimary[player] = false
		IsRememberSecondary[player] = false
		PrimaryWeapon[player] = null
		SecondaryWeapon[player] = null
	}
}

@player_give_default_items(player, Class:class) {
	if (class != HumanClass) {
		return
	}
	new bool:primaryOpened
	if (IsRememberPrimary[player] && PrimaryWeapon[player] != null) {
		give_weapon_by_id(player, PrimaryWeapon[player])
	} else {
		primaryOpened = openPrimaryMenu(player)
	}
	if (IsRememberSecondary[player] && SecondaryWeapon[player] != null) {
		give_weapon_by_id(player, SecondaryWeapon[player])
	} else if (!primaryOpened) {
		openSecondaryMenu(player)
	}
}

@Command_Weapons(player) {
	openPrimaryMenu(player)
	return PLUGIN_HANDLED
}

bool:openPrimaryMenu(player) {
	new weaponsCount = ArraySize(Primaries)
	if (!weaponsCount) {
		return false
	}
	createMenu(player, PRIMARY_MENU_ID)
	//addText("Выберите оружие^n")
	addText("Выберите^n\yоружие^n\wиз CS:GO^n")
	for (new i = 0; i < weaponsCount; ++i) {
		new Weapon:weapon = ArrayGetCell(Primaries, i)
		addItem((1<<i), "^n\y%d. \w%l", i + 1, get_weapon_var_string(weapon, "name"))
	}
	addText("^n")
	addItem(MENU_KEY_9, "^n\y9. \wЗапомнить оружие [\y%s\w]", IsRememberPrimary[player] ? "да" : "нет")
	addText("^n")
	if (ArraySize(Secondaries)) {
		addItem(MENU_KEY_0, "^n\r[0] \w%l", "Пропустить")
	} else {
		addItem(MENU_KEY_0, "^n\r[0] \w%l", "CLOSE")
	}
	showMenu(player)
	return true
}

@Menu_Primary(player, key) {
	primaryMenuHandler(player, ++key)
	return PLUGIN_HANDLED
}

primaryMenuHandler(player, key) {
	if (!is_user_alive(player)) {
		return
	}
	switch (key) {
		case 9: {
			IsRememberPrimary[player] = !IsRememberPrimary[player]
			openPrimaryMenu(player)
		}
		case 10: {
			openSecondaryMenu(player)
		}
		default: {
			PrimaryWeapon[player] = ArrayGetCell(Primaries, _:(key - 1))
			give_weapon_by_id(player, PrimaryWeapon[player])
			if (IsRememberSecondary[player] && SecondaryWeapon[player] != null) {
				return
			}
			openSecondaryMenu(player)
		}
	}
}

openSecondaryMenu(player) {
	new weaponsCount = ArraySize(Secondaries)
	if (!weaponsCount) {
		return
	}
	createMenu(player, SECONDARY_MENU_ID)
	//addText("Выберите пистолет^n")
	addText("Выберите^n\yпистолет^n\wиз CS:GO^n")
	for (new i = 0; i < weaponsCount; ++i) {
		new Weapon:weapon = ArrayGetCell(Secondaries, i)
		addItem((1<<i), "^n\y%d. \w%l", i + 1, get_weapon_var_string(weapon, "name"))
	}
	//addItem(MENU_KEY_1, "\y1. \wGlock-18^n")
	//addItem(MENU_KEY_2, "\y2. \wUSP^n")
	//addItem(MENU_KEY_3, "\y3. \wP228^n")
	//addItem(MENU_KEY_4, "\y4. \wFive-SeveN^n")
	//addItem(MENU_KEY_5, "\y5. \wDeagle^n")
	//addItem(MENU_KEY_6, "\y6. \wDual Elites^n")
	addText("^n")
	addItem(MENU_KEY_9, "^n\y9. \wЗапомнить пистолет [\y%s\w]", IsRememberSecondary[player] ? "да" : "нет")
	addText("^n")
	addItem(MENU_KEY_0, "^n\r[0] \w%l", "CLOSE")
	showMenu(player)
}

@Menu_Secondary(player, key) {
	secondaryMenuHandler(player, ++key)
	return PLUGIN_HANDLED
}

secondaryMenuHandler(player, key) {
	if (!is_user_alive(player)) {
		return
	}
	if (get_player_var(player, "class") != HumanClass) {
		return
	}
	switch (key) {
		case 9: {
			IsRememberSecondary[player] = !IsRememberSecondary[player]
			openSecondaryMenu(player)
		}
		case 10: {
			// Nothing
		}
		default: {
			SecondaryWeapon[player] = ArrayGetCell(Secondaries, _:(key - 1))
			give_weapon_by_id(player, SecondaryWeapon[player])
		}
	}
}

parsePrimary() {
	Primaries = ArrayCreate(1)
	for (new i = 0; i < sizeof(PRIMARY_WEAPONS); ++i) {
		new Weapon:weapon = find_weapon(PRIMARY_WEAPONS[i])
		if (!weapon) {
			log(Log_Warning, "Cannot find '%s' weapon", PRIMARY_WEAPONS[i])
			continue
		}
		ArrayPushCell(Primaries, weapon)
	}
	return ArraySize(Primaries)
}

parseSecondary() {
	Secondaries = ArrayCreate(1)
	for (new i = 0; i < sizeof(SECONDARY_WEAPONS); ++i) {
		new Weapon:weapon = find_weapon(SECONDARY_WEAPONS[i])
		if (!weapon) {
			log(Log_Warning, "Cannot find '%s' weapon", SECONDARY_WEAPONS[i])
			continue
		}
		ArrayPushCell(Secondaries, weapon)
	}
	return ArraySize(Secondaries)
}
