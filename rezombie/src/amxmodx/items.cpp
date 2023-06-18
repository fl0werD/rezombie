#include "rezombie/amxmodx/items.h"
#include "rezombie/modules/item.h"
#include "rezombie/player/players.h"
#include <amxx/api.h>

namespace rz
{
    using namespace amx;
    using namespace amxx;

    auto AmxxItem::Given(int playerIndex, int itemIndex) -> void {
        ExecuteForward(getForward(ItemForward::Given), playerIndex, itemIndex);
    }

    auto AmxxItem::RegisterForwards() -> void {
        using e = ForwardExecType;
        using p = ForwardParam;

        setForward(
            ItemForward::Given,
            RegisterForward("@rz_item_given", e::Ignore, p::Cell, p::Cell, p::Done)
        );
    }

    auto AMX_NATIVE_CALL rz_create_item(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_handle,
            arg_class,
            arg_give_forward,
        };

        using p = ForwardParam;

        // Check arg count
        auto handle = GetAmxString(amx, params[arg_handle]);
        const int classIndex = params[arg_class];
        const auto giveForward = RegisterSpForwardByName(amx, GetAmxString(amx, params[arg_give_forward]), p::Cell, p::Done);
        if (giveForward == -1) {
            return false;
        }
        const auto itemIndex = itemModule.add(handle, classIndex, giveForward);
        return itemIndex;
    }

    auto AMX_NATIVE_CALL rz_get_item_var(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_item,
            arg_var,
            arg_3,
            arg_4,
        };

        const int itemIndex = params[arg_item];
        const auto itemRef = itemModule[itemIndex];
        if (!itemRef) {
            // Invalid index
            return false;
        }
        const auto& item = itemRef->get();
        using var = ItemVars;
        switch (static_cast<var>(params[arg_var])) {
            case var::Handle: {
                SetAmxString(amx, params[arg_3], item.getHandle().c_str(), params[arg_4]);
                break;
            }
            case var::Name: {
                SetAmxString(amx, params[arg_3], item.getName().c_str(), *Address(amx, params[arg_4]));
                break;
            }
            case var::Class: {
                return item.getClass();
            }
            case var::Price: {
                return item.getPrice();
            }
            default: {
                // Invalid WeaponVar
                return false;
            }
        }
        return true;
    }

    auto AMX_NATIVE_CALL rz_set_item_var(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_item,
            arg_var,
            arg_value,
        };

        const int itemIndex = params[arg_item];
        const auto itemRef = itemModule[itemIndex];
        if (!itemRef) {
            // Invalid index
            return false;
        }
        auto& item = itemRef->get();
        using var = ItemVars;
        switch (static_cast<var>(params[arg_var])) {
            case var::Handle: {
                // Invalid set var
                break;
            }
            case var::Name: {
                item.setName(GetAmxString(amx, params[arg_value]));
                break;
            }
            case var::Class: {
                item.setClass(*Address(amx, params[arg_value]));
                break;
            }
            case var::Price: {
                item.setPrice(*Address(amx, params[arg_value]));
                break;
            }
            default: {
                // Invalid WeaponVar
                return false;
            }
        }
        return true;
    }

    auto AMX_NATIVE_CALL rz_item_begin(Amx*, cell*) -> cell {
        return itemModule.begin();
    }

    auto AMX_NATIVE_CALL rz_item_end(Amx*, cell*) -> cell {
        return itemModule.end();
    }

    auto AMX_NATIVE_CALL rz_find_item(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_handle,
        };

        const auto handle = GetAmxString(amx, params[arg_handle]);
        return itemModule[handle];
    }

    auto AMX_NATIVE_CALL rz_give_item(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_handle,
        };

        const int playerIndex = params[arg_player];
        const auto& player = players[playerIndex];
        const auto handle = GetAmxString(amx, params[arg_handle]);
        const auto itemIndex = itemModule[handle];
        const auto itemRef = itemModule[itemIndex];
        if (!itemRef) {
            // Invalid index
            return false;
        }
        auto& item = itemRef->get();
        if (item.executeGive(playerIndex)) {
            amxxItem.Given(playerIndex, itemIndex);
        }
        return 1;
    }

    auto AMX_NATIVE_CALL rz_give_item_fast(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_item,
        };

        const int playerIndex = params[arg_player];
        const auto& player = players[playerIndex];
        const auto itemIndex = params[arg_item];
        const auto itemRef = itemModule[itemIndex];
        if (!itemRef) {
            // Invalid index
            return false;
        }
        auto& item = itemRef->get();
        if (item.executeGive(playerIndex)) {
            amxxItem.Given(playerIndex, itemIndex);
        }
        return 1;
    }

    auto AmxxItem::RegisterNatives() -> void {
        static AmxNativeInfo natives[] = {
            {"rz_create_item",    rz_create_item},
            {"rz_get_item_var",   rz_get_item_var},
            {"rz_set_item_var",   rz_set_item_var},
            {"rz_item_begin",     rz_item_begin},
            {"rz_item_end",       rz_item_end},
            {"rz_find_item",      rz_find_item},
            {"rz_give_item",      rz_give_item},
            {"rz_give_item_fast", rz_give_item_fast},

            {nullptr,             nullptr},
        };
        AddNatives(natives);
    }
}
