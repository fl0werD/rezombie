#include <amxmodx>
#include <reapi>
#include <rezombie>

new const ITEMS_MENU_COMMANDS[][] = {
	"items",
	"say /items",
}

new const ITEMS_MENU_ID[] = "ExtraItems"

public plugin_precache() {
	register_plugin("Menu: Extra Items", "1.0.0", "fl0wer")
	register_commands(ITEMS_MENU_COMMANDS, "@Command_Items")
	register_menu(ITEMS_MENU_ID, "@ItemsMenu")
	createMenuItems(ArrayCreate(1))
	add_translate("menus/items")
}

@Command_Items(player) {
	openItemsMenu(player)
	return PLUGIN_HANDLED
}

openItemsMenu(player, Page:page = START_PAGE) {
	if (isTerminateRoundState()) {
		return
	}
	createMenu(player, ITEMS_MENU_ID)
	addText("\y%l", "ITEMS_MENU_TITLE")
	addText("^n")
	MenuItemsCount = collectAvailableItems(player)
	calculateMenuPage(player, page)
	if (MenuItemsCount) {
		new Item:item
		new Price:price
		new Currency:currency
		new name[MAX_ITEM_VAR_LENGTH]
		new amount
		new priceFormat[MAX_CURRENCY_FORMAT_LENGTH]
		for (new i = MenuStart; i < MenuEnd; ++i) {
			item = ArrayGetCell(MenuItems[player], i)
			name = get_item_var_string(item, "name")
			price = get_item_var(item, "price")
			if (price) {
				currency = get_price_var(price, "currency")
				amount = get_price_var(price, "amount")
				priceFormat = format_currency(currency, amount)
				if (get_player_var(player, "currency", currency) >= amount) {// && rz_item_player_get_status(player, item) == RZ_CONTINUE)
					addItem((1<<MenuItem), "^n\y%d. \w%l \y%s", MenuItem + 1, name, priceFormat)
				} else {
					addItem((1<<MenuItem), "^n\d%d. %l \d%s", MenuItem + 1, name, priceFormat)
				}
			} else {
				addItem((1<<MenuItem), "^n\y%d. \w%l", MenuItem + 1, name)
			}
			++MenuItem
		}
		addMenuSpacingAfterItems(player)
	} else {
		addText("^n\d%l", "EMPTY")
	}
	addMenuNavigation(player)
	showMenu(player)
}

collectAvailableItems(player) {
	new Item:end = items_end()
	for (new Item:item = items_begin(); item < end; ++item) {
		//if (rz_item_player_get_status(player, i) >= RZ_BREAK)
		//	continue
		ArrayPushCell(MenuItems[player], item)
	}
	return ArraySize(MenuItems[player])
}

@ItemsMenu(player, key) {
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
			new Item:item = getMenuItem(player, key)
			processItem(player, item)
			return PLUGIN_HANDLED
		}
	}
	openItemsMenu(player, MenuPage[player])
	return PLUGIN_HANDLED
}

processItem(player, Item:item) {
	if (isTerminateRoundState()) {
		return
	}
	new Price:price = get_item_var(item, "price")
	new Currency:currency = get_price_var(price, "currency")
	new amount = get_price_var(price, "amount")
	new playerAmount = get_player_var(player, "currency", currency)
	new name[MAX_ITEM_VAR_LENGTH]
	new currencyFormat[MAX_CURRENCY_FORMAT_LENGTH]
	name = get_item_var_string(item, "name")
	if (playerAmount < amount) {
		currencyFormat = format_currency(currency, amount - playerAmount)
		print_to_chat(player, print_team_red, "%l", "ITEMS_MENU_INSUFFICIENT_FUNDS", name, currencyFormat)
		return
	}
	//if (rz_item_player_get_status(player, i) >= RZ_BREAK)
	//	continue
	if (price && !add_player_currency(player, currency, -amount, "за покупку %l", name)) {
		print_to_chat(player, print_team_red, "%l", "ITEMS_MENU_SOMETHING_WENT_WRONG")
		return
	}
	if (!give_item_by_id(player, item)) {
		return
	}
	new Class:class = get_player_var(player, "class")
	new className[MAX_ITEM_VAR_LENGTH]
	className = get_class_var_string(class, "name")
	currencyFormat = format_currency(currency, amount)
	if (price) {
		print_to_chat(TO_ALL, player, "%l", "ITEMS_MENU_PLAYER_BOUGHT", className, player, name, currencyFormat)
	} else {
		print_to_chat(TO_ALL, player, "%l", "ITEMS_MENU_PLAYER_TAKEN", className, player, name, currencyFormat)
	}
}

bool:isTerminateRoundState() {
	return get_game_var("round_state") == round_state_terminate
}
