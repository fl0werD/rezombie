#include <amxmodx>
#include <rezombie>

new const SUBCLASS_MENU_ID[] = "ZombieSubclass"

new Subclass:StartingSubclass

new Class:ClassZombie

new bool:IsSubclassChoosen[MAX_PLAYERS + 1]

public plugin_precache() {
	register_plugin("[RZ] Menu: Zombie Subclass", "1.0.0", "fl0wer")
	ClassZombie = checkClassExists("zombie")
}

public plugin_init() {
	register_menu(SUBCLASS_MENU_ID, "@SubclassMenu")
	createMenuItems(ArrayCreate(1))
	setStartingSubclass()
}

public client_putinserver(player) {
	if (is_user_bot(player)) {
		IsSubclassChoosen[player] = false
	} else {
		IsSubclassChoosen[player] = false
	}
	set_player_var(player, "keep_subclass", ClassZombie, StartingSubclass)
}

@change_class_post(player, Class:class, attacker) {
	if (class != ClassZombie) {
		return
	}
	if (IsSubclassChoosen[player]) {
		return
	}
	openSubclassMenu(player)
}

openSubclassMenu(player, Page:page = START_PAGE) {
	createMenu(player, SUBCLASS_MENU_ID)
	addText("\yВыбери подкласс %l^n", get_class_var_string(ClassZombie, "name"))
	addText("^n")
	MenuItemsCount = collectAvailableItems(player)
	calculateMenuPage(player, page)
	new Subclass:currentSubclass = get_player_var(player, "keep_subclass", ClassZombie)
	new Subclass:subclass
	new name[MAX_SUBCLASS_VAR_LENGTH]
	for (new i = MenuStart; i < MenuEnd; ++i) {
		subclass = ArrayGetCell(MenuItems[player], i)
		name = get_subclass_var_string(subclass, "name")
		if (subclass == currentSubclass) {
			addItem((1<<MenuItem), "^n\r%d. \w%l \y[%l]", MenuItem + 1, name, "CURRENT")
		} else {
			addItem((1<<MenuItem), "^n\r%d. \w%l", MenuItem + 1, name)
		}
		++MenuItem
	}
	addMenuSpacingAfterItems(player)
	addText("^n")
	addMenuNavigation(player)
	showMenu(player)
}

collectAvailableItems(player) {
	new Subclass:end = subclasses_end()
	for (new Subclass:item = subclasses_begin(); item < end; ++item) {
		if (get_subclass_var(item, "class") != ClassZombie) {
			continue
		}
		ArrayPushCell(MenuItems[player], item)
	}
	return ArraySize(MenuItems[player])
}

@SubclassMenu(player, key) {
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
			new Subclass:subclass = getMenuItem(player, key)
			IsSubclassChoosen[player] = true
			new Subclass:currentSubclass = get_player_var(player, "keep_subclass", ClassZombie)
			if (currentSubclass != subclass) {
				set_player_var(player, "keep_subclass", ClassZombie, subclass)
				change_player_subclass(player, subclass)
			}
			return PLUGIN_HANDLED
		}
	}
	openSubclassMenu(player, MenuPage[player])
	return PLUGIN_HANDLED
}

setStartingSubclass() {
	new Subclass:end = subclasses_end()
	for (new Subclass:i = subclasses_begin(); i < end; ++i) {
		if (get_subclass_var(i, "class") != ClassZombie) {
			continue
		}
		StartingSubclass = i
		break
	}
}
