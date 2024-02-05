#include <amxmodx>
#include <fakemeta>
#include <rezombie>

// fatal not work

// For example: "^4[RZ] ^1"
new const CHAT_PREFIX[] = ""

new Array:TranslateFiles

public plugin_precache() {
	register_plugin("[RZ] Main", "1.0.0", "fl0wer")
	TranslateFiles = ArrayCreate(PLATFORM_MAX_PATH)
	
	add_translate("common")

	addColors()
}

public plugin_init() {
	internalLog(Log_Info, "Loaded %d classes, %d subclasses, %d modes, %d weapons, %d items",
		classes_count(),
		subclasses_count(),
		modes_count(),
		weapons_count(),
		items_count()
	)
	// Lazy loading because AmxModX can't call get_langsnum and get_lang on plugin_precache
	loadLanguages()
}

addColors() {
	//create_color("team_human", {0, 0, 255, 1})
	//create_color("team_zombie", {255, 255, 0, 1})
}

loadLanguages() {
	new translatesCount = ArraySize(TranslateFiles)
	new translateFile[PLATFORM_MAX_PATH]
	new filePath[PLATFORM_MAX_PATH]
	new langsCount = get_langsnum()
	new langNameShort[3]
	for (new i = 0; i < translatesCount; ++i) {
		ArrayGetString(TranslateFiles, i, translateFile, charsmax(translateFile))
		for (new lang = 0; lang < langsCount; ++lang) {
			get_lang(lang, langNameShort)
			format(filePath, charsmax(filePath), "%s/%s", langNameShort, translateFile)
			register_translate(filePath)
		}
	}
	ArrayClear(TranslateFiles)
}

public plugin_natives() {
	//ExecuteForward(CreateMultiForward("__rezp_version_check", ET_IGNORE, FP_STRING, FP_STRING), _, REZP_VERSION_MAJOR, REZP_VERSION_MINOR)

	register_native("log", "@log")
	register_native("print_to_chat", "@print_to_chat")
	register_native("add_translate", "@add_translate")
}

@log(plugin, argc) {
	enum {
		arg_level = 1,
		arg_message,
		arg_arguments,
	}

	new level = get_param(arg_level)
	new buffer[512]
	new fileName[64]
	new pluginName[64]
	vdformat(buffer, charsmax(buffer), arg_message, arg_arguments)
	get_plugin(plugin, fileName, charsmax(fileName), pluginName, charsmax(pluginName))
	switch (level) {
		case Log_Fatal: {
			dllfunc(DLLFunc_Sys_Error, fmt("[ReZombie] %s (%s): %s", fileName, fileName, buffer))
		}
		case Log_Error: {
			server_print("[ERROR] %s: %s. Plugin %s has stopped", fileName, buffer, fileName)
			pause("c", fileName)
		}
		case Log_Warning: {
			server_print("[WARN] %s: %s", fileName, buffer)
		}
		case Log_Info: {
			server_print("[INFO] %s: %s", fileName, buffer)
		}
	}
}

@print_to_chat(plugin, argc) {
	enum {
		arg_player = 1,
		arg_sender,
		arg_text,
		arg_arguments,
	}

	new player = get_param(arg_player)
	new sender = get_param(arg_sender)
	new buffer[190]
	vdformat(buffer, charsmax(buffer), arg_text, arg_arguments)
	client_print_color(player, sender, "%s%s", CHAT_PREFIX, buffer)
}

@add_translate(plugin, argc) {
	enum {
		arg_translate_file = 1
	}

	new translateFile[PLATFORM_MAX_PATH]
	get_string(arg_translate_file, translateFile, charsmax(translateFile))
	if (!translateFile[0]) {
		return false
	}
	ArrayPushString(TranslateFiles, translateFile)
	return true
}

internalLog(LogLevel:level, const message[], any:...) {
	// static?
	new buffer[512]
	vformat(buffer, charsmax(buffer), message, 3)
	switch (level) {
		case Log_Fatal: {
			dllfunc(DLLFunc_Sys_Error, fmt("[ReZombie]: %s", buffer))
		}
		case Log_Error: {
			server_print("[ERROR] %s", buffer)
		}
		case Log_Warning: {
			server_print("[WARN] %s", buffer)
		}
		case Log_Info: {
			server_print("[INFO] %s", buffer)
		}
	}
}
