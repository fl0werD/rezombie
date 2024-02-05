#include <amxmodx>
#include <rezombie>

// bug when change map have previuous
public plugin_precache() {
	register_plugin("Environment", "1.0.0", "fl0wer")
	
	set_env_var("sky_name", "space")
	set_env_var("light", "f")
	set_env_var("fog", create_fog("grey", 15))
	set_env_var("weather_type", weather_type_rain)
}
