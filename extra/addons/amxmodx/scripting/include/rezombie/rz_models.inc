#if defined _rz_models_included
	#endinput
#endif

#define _rz_models_included

native Model:create_model(const path[], body = 0, skin = 0, const handle[] = "");

native get_model_var(Model:model, const var[], ...);
native set_model_var(Model:model, const var[], any:...);

native models_pack_add_model(ModelsPack:modelsPack, Model:model);
