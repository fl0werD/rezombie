#include "rezombie/player/player.h"
#include "rezombie/player/modules/nightvision.h"
#include "rezombie/map/environment.h"
#include "rezombie/messages/user_message.h"

namespace rz
{
    auto Player::ChangeNightVision(const int nightVisionId) -> bool {
        const auto& nightVisionRef = NightVisions[nightVisionId];
        if (!nightVisionRef) {
            return false;
        }
        auto& nightVision = NightVision();
        nightVision.setId(nightVisionId);
        SwitchNightVision(true);
        return true;
    }

    auto Player::SwitchNightVision(const bool isEnabled) -> void {
        auto& nightVision = NightVision();
        if (isEnabled) {
            const auto& nightVisionRef = NightVisions[nightVision.getId()];
            if (!nightVisionRef) {
                return;
            }
            nightVision.setEnabled(true);
            const auto& nv = nightVisionRef->get();
            netLightStyle(*this, 0, "z");
            sendFog(*this, nv.getFog());
            sendScreenFade(
                *this,
                0,
                0,
                FADE_MODULATE | FADE_STAY_OUT,
                nv.getColor()
            );
        } else {
            nightVision.setEnabled(false);
            netLightStyle(*this, 0, Environment.getLight());
            sendFog(*this);
            sendScreenFade(*this);
        }
    }
}
