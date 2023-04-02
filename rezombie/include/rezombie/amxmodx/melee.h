#pragma once

namespace rz
{
    enum class MeleeVars : int
    {
        Handle,
        ViewModel,
        PlayerModel,
        WorldModel,
        WeaponList,
        Name,
    };

    auto RegisterMeleeNatives() -> void;
}
