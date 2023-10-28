#include "rezombie/items/api/items.h"
#include "rezombie/items/modules/item.h"
#include "rezombie/player/players.h"
#include "rezombie/core/api/amxx_helper.h"
#include <filesystem>

namespace rz
{
    using namespace amx;
    using namespace amxx;

    auto AmxxItem::GivePre(int playerId, int itemId) -> ForwardReturn {
        return executeForward(ItemForward::Give_Pre, playerId, itemId);
    }

    auto AmxxItem::GivePost(int playerId, int itemId) -> void {
        executeForward(ItemForward::Give_Post, playerId, itemId);
    }

    auto AmxxItem::registerForwards() -> void {
        using e = ForwardExecType;
        using p = ForwardParam;

        registerForward(ItemForward::Give_Pre, "@give_item_pre", e::Ignore, p::Cell, p::Cell, p::Done);
        registerForward(ItemForward::Give_Post, "@give_item_post", e::Ignore, p::Cell, p::Cell, p::Done);
    }

    enum class ItemVars : int {
        Handle,
        Name,
        Price,
        Class,
    };

    const std::unordered_map<std::string, ItemVars> ItemVarsMap = {
        {"handle", ItemVars::Handle},
        {"name",   ItemVars::Name},
        {"price",  ItemVars::Price},
        {"class",  ItemVars::Class},
    };

    auto create_item(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_handle,
            arg_give_forward,
        };

        using p = ForwardParam;

        // Check arg count
        auto handle = GetAmxString(amx, params[arg_handle]);
        const auto giveForward = RegisterSpForwardByName(
            amx, GetAmxString(amx, params[arg_give_forward]),
            p::Cell, p::Done
        );
        if (giveForward == FORWARD_INVALID) {
            return false;
        }
        const auto itemId = Items.add(handle, giveForward);
        return itemId;
    }

    auto HandleItemVar(Amx* amx, cell* params, bool isGetter) -> cell {
        enum {
            arg_count,
            arg_item,
            arg_var,
            arg_3,
            arg_4,
        };

        using vars = ItemVars;

        const int itemId = params[arg_item];
        const auto itemRef = Items[itemId];
        if (!itemRef) {
            // Invalid index
            return false;
        }
        const auto key = GetAmxString(amx, params[arg_var]);
        const auto& var = getMapValue(ItemVarsMap, key);
        CHECK_VAR_EXISTS("Invalid item '%s' var", key)
        auto& item = itemRef->get();
        switch (*var) {
            case vars::Handle: {
                if (isGetter) {
                    SetAmxString(amx, params[arg_3], item.getHandle().c_str(), params[arg_4]);
                } else {
                    // Invalid set vars
                }
                break;
            }
            case vars::Name: {
                if (isGetter) {
                    SetAmxString(amx, params[arg_3], item.getName().c_str(), *Address(amx, params[arg_4]));
                } else {
                    item.setName(GetAmxString(amx, params[arg_3]));
                }
                break;
            }
            case vars::Price: {
                if (isGetter) {
                    return item.getPrice();
                } else {
                    item.setPrice(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::Class: {
                if (isGetter) {
                    return item.getClass();
                } else {
                    item.setClass(*Address(amx, params[arg_3]));
                }
                break;
            }
        }
        return true;
    }

    auto get_item_var(Amx* amx, cell* params) -> cell {
        return HandleItemVar(amx, params, true);
    }

    auto set_item_var(Amx* amx, cell* params) -> cell {
        return HandleItemVar(amx, params, false);
    }

    auto item_begin(Amx*, cell*) -> cell {
        return Items.begin();
    }

    auto item_end(Amx*, cell*) -> cell {
        return Items.end();
    }

    auto find_item(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_handle,
        };

        const auto handle = GetAmxString(amx, params[arg_handle]);
        return Items[handle];
    }

    auto give_item(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_handle,
        };

        const auto& player = Players[params[arg_player]];
        const auto handle = GetAmxString(amx, params[arg_handle]);
        const auto itemId = Items[handle];
        const auto itemRef = Items[itemId];
        if (!itemRef) {
            // Invalid index
            return false;
        }
        auto& item = itemRef->get();
        return item.executeGive(player, itemId);
    }

    auto give_item_by_id(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_item,
        };

        const auto& player = Players[params[arg_player]];
        const auto itemId = params[arg_item];
        const auto itemRef = Items[itemId];
        if (!itemRef) {
            // Invalid index
            return false;
        }
        auto& item = itemRef->get();
        return item.executeGive(player, itemId);
    }

    auto register_translate(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_lang_file,
        };

        const auto langFile = GetAmxString(amx, params[arg_lang_file]);
        const auto filePath = str::Format("rezombie/translates/%s.txt", langFile);
        std::filesystem::path path{str::BuildPathName(filePath)};
        if (!std::filesystem::exists(path)) {
            return false;
        }
        MergeDefinitionFile(path.string().c_str());
        return true;
    }

    auto AmxxItem::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"create_item",        create_item},
            {"get_item_var",       get_item_var},
            {"set_item_var",       set_item_var},
            {"item_begin",         item_begin},
            {"item_end",           item_end},
            {"find_item",          find_item},
            {"give_item",          give_item},
            {"give_item_by_id",    give_item_by_id},

            {"register_translate", register_translate},

            {nullptr,              nullptr},
        };
        AddNatives(natives);
    }
}
