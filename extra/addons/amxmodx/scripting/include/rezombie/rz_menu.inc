#if defined _rz_menu_included
	#endinput
#endif

#define _rz_menu_included

const MAX_MENU_PAGE_ITEMS = 7;
const MAX_MENU_ID_LENGTH = 64;

const Page:START_PAGE = any:0;

enum {
	MenuKey_Back = 7,
	MenuKey_Next = 8,
	MenuKey_Exit = 9,
};

stock MenuKeys;
stock MenuLength;
stock MenuText[MAX_MENU_LENGTH];
stock MenuId[MAX_MENU_ID_LENGTH];
stock bool:MenuIsSinglePage;
stock MenuMaxPages;
stock MenuItemsCount;
stock MenuItem;
stock MenuStart;
stock MenuEnd;
stock Page:MenuPage[MAX_PLAYERS + 1];
stock MenuItemsPerPage[MAX_PLAYERS + 1];
stock Array:MenuItems[MAX_PLAYERS + 1];

#define createMenu(%0,%1) 	MenuText[0] = 0; MenuLength = 0; MenuKeys = 0; \
							copy(MenuId, charsmax(MenuId), %1); \
							ArrayClear(MenuItems[%0]); \
							SetGlobalTransTarget(%0)

#define addText(%0)			MenuLength += formatex(MenuText[MenuLength], charsmax(MenuText) - MenuLength, %0)

#define addItem(%0,%1)		addText(%1); MenuKeys |= %0

#define showMenu(%0)		show_menu(%0, MenuKeys, MenuText, -1, MenuId)

#define closeMenu(%0)		show_menu(%0, 0, NULL_STRING, 0)

stock Menu:register_menu(const id[], const handler[], const keys = 1023) {
	new Menu:menu = any:register_menuid(id);
	register_menucmd(any:menu, keys, handler);
	return menu;
}

stock Menu:get_opened_menu(player) {
	new Menu:menu, keys;
	get_user_menu(player, any:menu, keys);
	return menu;
}




stock const ITEMS_SINGLE_PAGE_COUNT = 9
stock const ITEMS_MULTI_PAGE_COUNT = 7

stock createMenuItems(Array:items) {
	for (new player = 1; player <= MaxClients; ++player) {
		MenuItems[player] = ArrayClone(items)
	}
	ArrayDestroy(items)
}

stock calculateMenuPage(player, Page:page, itemsPerPage = MAX_MENU_PAGE_ITEMS) {
	MenuItem = 0
	MenuItemsPerPage[player] = itemsPerPage
	MenuIsSinglePage = MenuItemsCount <= itemsPerPage
	if (MenuIsSinglePage) {
		MenuStart = 0
		MenuEnd = MenuItemsCount
		MenuPage[player] = START_PAGE
	} else {
		new i = min(_:page * itemsPerPage, MenuItemsCount)
		MenuStart = i - (i % itemsPerPage)
		MenuEnd = min(MenuStart + itemsPerPage, MenuItemsCount)
		MenuMaxPages = ((MenuItemsCount - 1) / itemsPerPage) + 1
		MenuPage[player] = any:(MenuStart / itemsPerPage)
	}
}

/*
stock calculateMenuPage(itemsCount, page, &menuItem, &start, &end, &maxPages, &isSinglePage) {
	menuItem = 0
	isSinglePage = itemsCount <= ITEMS_SINGLE_PAGE_COUNT
	if (isSinglePage) {
		start = 0
		end = itemsCount
		return 0
	} else {
		new itemsPerPage = ITEMS_MULTI_PAGE_COUNT
		new i = min(page * itemsPerPage, itemsCount)
		start = i - (i % itemsPerPage)
		end = min(start + itemsPerPage, itemsCount)
		maxPages = ((itemsCount - 1) / itemsPerPage) + 1
		return start / itemsPerPage
	}
}
*/

stock addMenuEmptyItem(const itemCapacity[] = "^n") {
	addText("%s\d%l", itemCapacity, "EMPTY")
}

stock addMenuCloseItem() {
	addItem(MENU_KEY_0, "^n\r[0] \w%l", "CLOSE")
}

stock addMenuSpacingAfterItems(player, const itemCapacity[] = "^n") {
	if (MenuItemsCount > MenuItemsPerPage[player]) {
		for (new i = MenuItem; i <= MenuItemsPerPage[player]; ++i) {
			addText(itemCapacity)
		}
	}
}

stock addMenuNavigation(player) {
	if (MenuItemsCount > MenuItemsPerPage[player]) {
		/*if (page) {
			addItem(MENU_KEY_8, "^n\r[8] \w%l", "BACK")
		} else {
			addText("^n\d[8] %l", "BACK")
		}
		if (MenuEnd < MenuItemsCount) {
			addItem(MENU_KEY_9, "^n\r[9] \w%l", "NEXT")
		} else {
			addText("^n\d[9] %l", "NEXT")
		}*/
		addText("^n")
		if (MenuPage[player]) {
			addItem(MENU_KEY_8, "\r[8] \w«")
		} else {
			addText("\d[8] «")
		}
		addText(" \y%d/%d ", _:MenuPage[player] + 1, MenuMaxPages)
		if (MenuEnd < MenuItemsCount) {
			addItem(MENU_KEY_9, "\w» \r[9]")
		} else {
			addText("\d» [9]")
		}
	}
	addText("^n")
	addMenuCloseItem()
}

/*
stock addMenuNavigation(itemsCount, page, menuItem, end, &keys, text[MAX_MENU_LENGTH], &len) {
	if (itemsCount > ITEMS_SINGLE_PAGE_COUNT) {
		for (new i = menuItem; i < ITEMS_MULTI_PAGE_COUNT; ++i) {
			addText("^n")
		}
		if (end < itemsCount) {
			addItem(MENU_KEY_8, "^n\r[8] \w%l", "NEXT")
		} else {
			addText("^n\d[8] %l", "NEXT")
		}
		if (page) {
			addItem(MENU_KEY_9, "^n\r[9] \w%l", "BACK")
		} else {
			addText("^n\d[9] %l", "BACK")
		}
		addText("^n")
	}
	addItem(MENU_KEY_0, "^n\r[0] \w%l", "CLOSE")
}
*/

stock any:getMenuItem(player, key) {
	return ArrayGetCell(MenuItems[player], _:MenuPage[player] * MenuItemsPerPage[player] + key)
}
