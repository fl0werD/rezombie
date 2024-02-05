#if defined _rz_items_included
	#endinput
#endif

#define _rz_items_included

native Item:create_item(const handle[], const giveForward[]);

native any:get_item_var(Item:item, const var[], ...);
native set_item_var(Item:item, const var[], any:...);

const MAX_ITEM_VAR_LENGTH = 32;

stock get_item_var_string(Item:item, const var[]) {
	new value[MAX_ITEM_VAR_LENGTH];
	get_item_var(item, var, value, charsmax(value));
	return value;
}

native items_count();
native Item:items_begin();
native Item:items_end();
native Item:find_item(const handle[]);

native bool:give_item(player, const handle[]);
native bool:give_item_by_id(player, Item:item);

forward @give_item_pre(player, Item:item);
forward @give_item_post(player, Item:item);

stock Item:checkItemExists(const handle[]) {
	new Item:item = find_item(handle);
	if (item == null) {
		log(Log_Warning, "Item '%s' not found", handle);
	}
	return item;
}
