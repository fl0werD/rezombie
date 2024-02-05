#if defined _rz_map_included
	#endinput
#endif

#define _rz_map_included

enum WeatherType {
	weather_type_none,
	weather_type_rain,
	weather_type_snow,
};

native any:get_env_var(const var[], ...);
native set_env_var(const var[], any:...);

native any:get_override_env_var(const var[], ...);
native set_override_env_var(const var[], any:...);

native reset_env();

native Fog:create_fog(const color[], densityPercentage);

native any:get_fog_var(Fog:fog, const var[], ...);
native set_fog_var(Fog:fog, const var[], any:...);

native any:get_map_camera_var(player, const var[], ...);
native set_map_camera_var(player, const var[], any:...);

native map_cameras_count();
native MapCamera:map_cameras_begin();
native MapCamera:map_cameras_end();

native bool:is_map_extra_exists(ExtraType:type, const key[]);

native any:get_map_extra(ExtraType:type, const key[], any:...);
native set_map_extra(ExtraType:type, const key[], any:...);

stock get_map_extra_string(const key[]) {
	new value[MAX_EXTRA_VAR_LENGTH];
	get_map_extra(extra_string, key, value, charsmax(value));
	return value;
}
