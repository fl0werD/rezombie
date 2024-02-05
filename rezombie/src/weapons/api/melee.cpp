#include "rezombie/weapons/api/melee.h"
#include "rezombie/player/players.h"
#include "rezombie/weapons/modules/weapon.h"
#include <amxx/api.h>

namespace rz
{
    using namespace amxx;

    auto AmxxMelee::AttackPre(int player, MeleeAttackType type) const -> ForwardReturn {
        return executeForward(MeleeForward::AttackPre, player, toInt(type));
    }

    auto AmxxMelee::AttackPost(int player, MeleeAttackType type, MeleeAttackResult result) const -> void {
        executeForward(MeleeForward::AttackPost, player, toInt(type), toInt(result));
    }

    auto AmxxMelee::registerForwards() -> void {
        using e = ForwardExecType;
        using p = ForwardParam;

        setForward(
            MeleeForward::AttackPre,
            RegisterForward("@melee_attack_pre", e::Ignore, p::Cell, p::Cell, p::Done)
        );
        setForward(
            MeleeForward::AttackPost,
            RegisterForward("@melee_attack_post", e::Ignore, p::Cell, p::Cell, p::Cell, p::Done)
        );
    }

    auto melee_add_sound(Amx* amx, cell* params) -> cell {
        enum {
            arg_count,
            arg_melee,
            arg_type,
            arg_path,
        };

        const int meleeId = params[arg_melee];
        const auto meleeRef = Weapons[meleeId];
        if (!meleeRef) {
            // Invalid index
            return false;
        }
        const auto& melee = dynamic_cast<Melee*>(&meleeRef->get());
        if (melee == nullptr) {
            return false;
        }
        const auto type = static_cast<MeleeSound>(params[arg_type]);
        const auto path = GetAmxString(amx, params[arg_path]);
        melee->add(type, path);
        return true;
    }

    auto AmxxMelee::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"melee_add_sound", melee_add_sound},

            {nullptr, nullptr},
        };
        AddNatives(natives);
    }
}
