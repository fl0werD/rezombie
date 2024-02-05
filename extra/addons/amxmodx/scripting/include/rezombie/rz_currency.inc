#if defined _rz_currency_included
	#endinput
#endif

#define _rz_currency_included

native Currency:create_currency(
	const currencyName[],
	const getFunction[],
	const setFunction[],
	const formatFunction[]
);

const MAX_CURRENCY_FORMAT_LENGTH = 32;

native [MAX_CURRENCY_FORMAT_LENGTH]format_currency(Currency:currency, amount, bool:isShort = true);

native Currency:find_currency(const handle[]);

native add_player_currency(player, Currency:currency, addAmount, const reason[] = "", any:...);

stock Currency:checkCurrencyExists(const handle[]) {
	new Currency:currency = find_currency(handle);
	if (currency == null) {
		log(Log_Warning, "Currency '%s' not found", handle);
	}
	return currency;
}


native Price:create_price(Currency:currency, amount);

native any:get_price_var(Price:price, const var[], ...);
native set_price_var(Price:price, const var[], any:...);

const MAX_PRICE_VAR_LENGTH = 32;

stock get_price_var_string(Price:price, const var[]) {
	new value[MAX_PRICE_VAR_LENGTH];
	get_price_var(price, var, value, charsmax(value));
	return value;
}
