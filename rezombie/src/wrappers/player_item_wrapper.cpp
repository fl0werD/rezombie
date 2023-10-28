#include "rezombie/entity/wrappers/player_item_wrapper.h"
#include "rezombie/gamerules/game_rules.h"
#include "rezombie/player/players.h"
#include "rezombie/player/modules/player_props.h"

namespace rz
{
    auto PlayerItem::SetObjectCollisionBox() -> void {
        setAbsMin(getOrigin() + Vector(-16, -16, 0));
        setAbsMax(getOrigin() + Vector(16, 16, 16));
    }

    auto PlayerItem::RemoveThink() -> void {
        setSolid(SolidType::NotSolid);
        setFlags(getFlags() | FL_KILL_ME);
    }

    auto PlayerItem::TouchWeapon(EntityBase* other) -> void {
        if (!(getFlags() & FL_ON_GROUND)) {
            return;
        }
        if (!other->IsPlayer() && !other->IsAlive()) {
            return;
        }
        auto& player = Players[other];
        const auto propsRef = Props[player.getProps()];
        if (propsRef) {
            const auto& props = propsRef->get();
            if (!props.getWeaponsInteraction()) {
                return;
            }
        }
        if (!player.getBase()->AddPlayerItem(this)) {
            return;
        }
        AttachToPlayer(player);
        UTIL_EmitSound(player, SoundChannel::Item, "items/gunpickup2.wav");
        if (player.getActiveItem() != this && gameRules->ShouldSwitchWeapon(player, this)) {
            player.SwitchWeapon(this);
        }
        SetTouch(nullptr);
    }
}
