#include <amxmodx>
#include <rezombie>

new const PREVIEW_SKELETON_MODEL[] = "rezombie/other/shop.mdl";
new const PREVIEW_WEAPON[] = "models/p_m4a1.mdl";

new const JOINING_MENU_ID[] = "Joining"

new Subclass:StartingSubclass

new Class:HumanClass
new Subclass:SelectedSubclass[MAX_PLAYERS + 1]

// add 0 - spectating (+flags)

new Model:SkeletonModel
new Model:WeaponModel

public plugin_precache() {
	register_plugin("Joining", "1.0.0", "fl0wer")
	HumanClass = checkClassExists("human")
	SkeletonModel = create_model(PREVIEW_SKELETON_MODEL, .handle = "preview_skeleton")
	WeaponModel = create_model(PREVIEW_WEAPON, .handle = "preview_weapon")
}

public plugin_init() {
	createMenuItems(ArrayCreate(1))
	register_menu(JOINING_MENU_ID, "@JoiningMenu")
	setStartingSubclass()
}

public client_putinserver(player) {
	SelectedSubclass[player] = StartingSubclass
	if (is_user_bot(player)) {
		return
	}
	set_player_var(player, "map_camera", map_cameras_begin())
	if (map_cameras_count() > 1) {
		set_player_var(player, "next_map_camera_time", get_gametime() + 2.0)
	}
}

// Advice: here you can add your own steps
@player_joining(player) {
	if (is_user_bot(player)) {
		player_joined(player)
		return
	}
	if (StartingSubclass == null) {
		player_joined(player)
		return
	}
	openSubclassMenu(player)
}

@player_joined(player) {
	if (get_player_var(player, "team") == TEAM_SPECTATOR) {
		return
	}
	respawn_player(player, 5)
}

openSubclassMenu(player, Page:page = START_PAGE) {
	set_preview_var(player, "enabled", true)
	set_preview_var(player, "parent_model", SkeletonModel)
	set_preview_var(player, "attach_model", get_subclass_var(SelectedSubclass[player], "model"))
	set_preview_var(player, "extra_attach_model", WeaponModel)
	createMenu(player, JOINING_MENU_ID)
	addText("\yВыбери подкласс %l^n^n", get_class_var_string(HumanClass, "name"))
	addText("\rНажмите ещё раз для выбора.^n")
	MenuItemsCount = collectAvailableSubclasses(player)
	calculateMenuPage(player, page, 7)
	new Subclass:subclass
	new name[MAX_SUBCLASS_VAR_LENGTH]
	for (new i = MenuStart; i < MenuEnd; ++i) {
		subclass = ArrayGetCell(MenuItems[player], i)
		name = get_subclass_var_string(subclass, "name")
		if (SelectedSubclass[player] == subclass) {
			addItem((1<<MenuItem), "^n\r%d. \y>> \r[\w%l\r] \y<<", MenuItem + 1, name)
		} else {
			addItem((1<<MenuItem), "^n\r%d. \w%l", MenuItem + 1, name)
		}
		++MenuItem
	}
	addMenuSpacingAfterItems(player)
	addText("^n")
	addItem(MENU_KEY_0, "^n\r[0] \w%l", "SPECTATING")
	showMenu(player)
}

collectAvailableSubclasses(player) {
	new Subclass:end = subclasses_end()
	for (new Subclass:item = subclasses_begin(); item < end; ++item) {
		if (get_subclass_var(item, "class") != HumanClass) {
			continue
		}
		ArrayPushCell(MenuItems[player], item)
	}
	return ArraySize(MenuItems[player])
}

@JoiningMenu(player, key) {
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
			if (SelectedSubclass[player] == subclass) {
				set_preview_var(player, "enabled", false)
				set_player_var(player, "map_camera", null)
				set_player_var(player, "keep_subclass", HumanClass, subclass)
				player_joined(player)
				return PLUGIN_HANDLED
			}
			SelectedSubclass[player] = subclass
		}
	}
	openSubclassMenu(player, MenuPage[player])
	return PLUGIN_HANDLED
}

setStartingSubclass() {
	for (new Subclass:i = subclasses_begin(); i < subclasses_end(); ++i) {
		if (get_subclass_var(i, "class") != HumanClass) {
			continue
		}
		StartingSubclass = i
		break
	}
}
