#include "rezombie/player/player.h"
#include "rezombie/player/api/player.h"
#include "rezombie/player/api/player_class.h"
#include "rezombie/player/api/player_subclass.h"
#include "rezombie/player/modules/player_class.h"
#include "rezombie/player/modules/player_subclass.h"
#include "rezombie/player/modules/player_sounds.h"
#include "rezombie/player/modules/flashlight.h"
#include "rezombie/models/modules/models.h"
#include "rezombie/models/modules/models_pack.h"
#include "rezombie/weapons/modules/weapon.h"
#include "rezombie/player/players.h"
#include "rezombie/entity/wrappers/player_base_wrapper.h"
#include "rezombie/main/util.h"
#include "rezombie/map/environment.h"
#include "rezombie/messages/user_message.h"
#include "rezombie/messages/temporary_entity.h"
#include <metamod/engine.h>
#include <metamod/gamedll.h>
#include <mhooks/reapi.h>

namespace rz
{
    auto Player::ChangeNightVision(int nightVisionId) -> bool {
        const auto& nightVisionRef = NightVisions[nightVisionId];
        if (!nightVisionRef) {
            return false;
        }
        getNightVision().setId(nightVisionId);
        SwitchNightVision(true);
        return true;
    }

    auto Player::SwitchNightVision(bool isEnabled) -> void {
        if (isEnabled) {
            const auto& nightVisionRef = NightVisions[getNightVision().getId()];
            if (!nightVisionRef) {
                return;
            }
            getNightVision().setEnabled(true);
            const auto& nightVision = nightVisionRef->get();
            netLightStyle(*this, 0, "z");
            sendFog(*this, nightVision.getFog());
            sendScreenFade(
                *this,
                0,
                0,
                FADE_MODULATE | FADE_STAY_OUT,
                nightVision.getColor(),
                nightVision.getAlphaPercentage()
            );
        } else {
            getNightVision().setEnabled(false);
            netLightStyle(*this, 0, Environment.getLight());
            sendFog(*this);
            sendScreenFade(*this);
        }
    }
}
