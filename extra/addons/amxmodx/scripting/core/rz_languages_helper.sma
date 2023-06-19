#include <amxmodx>

new const LANG_SUBFOLDER[] = "rezombie"

new Array:g_langFiles

public plugin_precache() {
	g_langFiles = ArrayCreate(PLATFORM_MAX_PATH)
}

public plugin_init() {
	register_plugin("[RZ] Languages Helper", "1.0.0", "fl0wer")

	// Lazy loading because AmxModX can't call register_dictionary, get_langsnum and get_lang on plugin_precache
	loadLanguages()
}

loadLanguages() {
	new dataFolder[PLATFORM_MAX_PATH]
	get_localinfo("amxx_datadir", dataFolder, charsmax(dataFolder))
	new filesCount = ArraySize(g_langFiles)
	new langFile[PLATFORM_MAX_PATH]
	new langsCount = get_langsnum()
	new langNameShort[3]
	for (new i = 0; i < filesCount; ++i) {
		ArrayGetString(g_langFiles, i, langFile, charsmax(langFile))
		for (new lang = 0; lang < langsCount; ++lang) {
			get_lang(lang, langNameShort)
			if (!file_exists(fmt("%s/lang/%s/%s/%s", dataFolder, LANG_SUBFOLDER, langNameShort, langFile))) {
				continue
			}
			register_dictionary(fmt("%s/%s/%s", LANG_SUBFOLDER, langNameShort, langFile))
		}
	}
	ArrayClear(g_langFiles)
}

public plugin_natives() {
	register_native("rz_add_translate", "@native_add_translate")
}

@native_add_translate(plugin, argc) {
	enum { arg_lang_file = 1 }
	new langFile[32]
	get_string(arg_lang_file, langFile, charsmax(langFile))
	if (!langFile[0]) {
		return false
	}
	static const LANG_FILE_EXTENSION[] = "txt"
	ArrayPushString(g_langFiles, fmt("%s.%s", langFile, LANG_FILE_EXTENSION))
	return true
}
