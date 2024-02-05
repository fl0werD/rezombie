#pragma once

#include "rezombie/core/api/amxx_feature.h"
#include "rezombie/entity/weapons/melee.h"

namespace rz
{
    enum class MeleeForward : int {
        AttackPre,
        AttackPost,
        MAX_FORWARDS,
    };

    enum MeleeAttackType {
        Primary,
        Secondary,
    };

    enum class MeleeVars : int {
        Handle,
        ViewModel,
        PlayerModel,
        WorldModel,
        WeaponList,
        Name,
    };

    class AmxxMelee : public AmxxFeature<MeleeForward, MeleeForward::MAX_FORWARDS> {
      public:
        AmxxMelee() : AmxxFeature("melee") {}

        auto registerForwards() -> void override;
        auto registerNatives() const -> void override;

        auto AttackPre(int player, MeleeAttackType attackType) const -> ForwardReturn;
        auto AttackPost(int player, MeleeAttackType attackType, MeleeAttackResult result) const -> void;
    };

    inline AmxxMelee MeleeApi;
}
