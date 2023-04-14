#include "rezombie/weapons/weapon_box.h"
#include "rezombie/modules/player_props.h"
#include "rezombie/player/players.h"
#include <vhooks/vhooks.h>

namespace rz::weapon
{
    using namespace cssdk;
    using namespace vhooks;
    using namespace rz::player;

    VirtualHook WeaponBoxVirtuals::touch(
        "weaponbox",
        HookIndex::Touch,
        &WeaponBoxVirtuals::WeaponBoxTouch
    );

    auto WeaponBoxVirtuals::WeaponBoxTouch(EntityBase* other) -> void {
        if (!other->IsPlayer()) {
            touch.Call(this, other);
            return;
        }
        const auto& player = players[other];
        const auto propsRef = playerPropsModule[player.getProps()];
        if (propsRef) {
            const auto& props = propsRef->get();
            if (!props.getWeaponsInteraction()) {
                return;
            }
        }
        touch.Call(this, other);
    }
}
