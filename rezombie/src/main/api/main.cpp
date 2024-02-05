#include "rezombie/main/main.h"
#include "rezombie/main/api/main.h"
#include "rezombie/entity/extras.h"
#include "rezombie/colors/colors.h"
#include "rezombie/core/api/amxx_helper.h"
#include <filesystem>
#include <core/strings/path.h>
#include <core/type_conversion.h>
#include <amxx/api.h>

namespace rz
{
    using namespace amx;
    using namespace amxx;
    using namespace core;

    auto register_translate(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_lang_file,
        };

        const auto langFile = GetAmxString(amx, params[arg_lang_file]);
        const auto filePath = str::Format("rezombie/translates/%s.txt", langFile);
        const std::filesystem::path path = str::BuildPathName(filePath);
        if (!exists(path)) {
            return false;
        }
        MergeDefinitionFile(path.string().c_str());
        return true;
    }

    auto is_extra_exists(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_type,
            arg_key,
        };

        const auto type = static_cast<ExtraType>(params[arg_type]);
        const auto key = GetAmxString(amx, params[arg_key]);
        return GlobalExtras.isKeyExists(type, key);
    }

    auto HandleExtra(Amx* amx, cell* params, bool isGetter) -> cell {
        enum {
            arg_count,
            arg_type,
            arg_key,
            arg_3,
            arg_4,
        };

        const auto type = static_cast<ExtraType>(params[arg_type]);
        const auto* key = GetAmxString(amx, params[arg_key], 0);
        switch (type) {
            case ExtraType::Int: {
                auto& extra = GlobalExtras.getInts();
                if (isGetter) {
                    const auto& var = getMapValue(extra, key);
                    CHECK_VAR_EXISTS("Invalid global extra '%s' var (int)", key)
                    return extra[key];
                } else {
                    extra[key] = *Address(amx, params[arg_3]);
                }
                break;
            }
            case ExtraType::Float: {
                auto& extra = GlobalExtras.getFloats();
                if (isGetter) {
                    const auto& var = getMapValue(extra, key);
                    CHECK_VAR_EXISTS("Invalid global extra '%s' var (float)", key)
                    return FloatToCell(extra[key]);
                } else {
                    extra[key] = CellToFloat(*Address(amx, params[arg_3]));
                }
                break;
            }
            case ExtraType::String: {
                auto& extra = GlobalExtras.getStrings();
                if (isGetter) {
                    const auto& var = getMapValue(extra, key);
                    CHECK_VAR_EXISTS("Invalid global extra '%s' var (string)", key)
                    SetAmxString(amx, params[arg_3], extra[key].c_str(), *Address(amx, params[arg_4]));
                } else {
                    extra[key] = GetAmxString(amx, params[arg_3], 1);
                }
                break;
            }
        }
        return true;
    }

    auto get_extra(Amx* amx, cell* params) -> cell {
        return HandleExtra(amx, params, true);
    }

    auto set_extra(Amx* amx, cell* params) -> cell {
        return HandleExtra(amx, params, false);
    }

    //native set_entity_render(entity, mode = kRenderNormal, amount = 0, const color[] = "", fx = kRenderFxNone)

    auto set_entity_render(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_entity,
            arg_mode,
            arg_amount,
            arg_color,
            arg_fx,
        };

        using namespace type_conversion;

        const auto color = GetAmxString(amx, params[arg_color]);
        const auto colorRef = Colors.parse(color);
        if (!colorRef) {
            return false;
        }
        const auto colors = *colorRef;
        const int entityIndex = params[arg_entity];
        const auto mode = static_cast<Rendering>(params[arg_mode]);
        const int amount = params[arg_amount];
        const auto fx = static_cast<RenderingFx>(params[arg_fx]);
        const auto& entityEdict = EdictByIndex(entityIndex);
        const auto& vars = &entityEdict->vars;
        vars->render_mode = mode;
        vars->render_amount = static_cast<float>(amount);
        vars->render_color = Vector(colors.getRed(), colors.getGreen(), colors.getBlue());
        vars->render_fx = fx;
        return true;
    }

    auto AmxxMain::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"register_translate", register_translate},
            {"is_extra_exists", is_extra_exists},
            {"get_extra", get_extra},
            {"set_extra", set_extra},

            {"set_entity_render", set_entity_render},

            {nullptr, nullptr},
        };
        AddNatives(natives);
    }
}
