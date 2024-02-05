#include "rezombie/player/player.h"
#include "rezombie/player/modules/flashlight.h"
#include "rezombie/messages/user_message.h"
#include "rezombie/messages/temporary_entity.h"

namespace rz
{
    auto Player::FlashlightUpdate() -> void {
        auto& flashlight = Flashlight();
        const auto& flashlightRef = Flashlights[flashlight.getId()];
        if (flashlightRef) {
            setHideHud(getHideHud() & ~HIDE_HUD_FLASHLIGHT);
        } else {
            setHideHud(getHideHud() | HIDE_HUD_FLASHLIGHT);
            return;
        }
        const auto& fl = flashlightRef->get();
        if (flashlight.getTime() <= g_global_vars->time) {
            if (flashlight.isEnabled()) {
                if (flashlight.getBattery()) {
                    flashlight.setBattery(flashlight.getBattery() - 1);
                    if (flashlight.getBattery() <= 0) {
                        flashlight.setEnabled(false);
                        flashlight.setTime(g_global_vars->time + fl.getChargeTime());
                        flashlight.setNextDynamicUpdate(0);
                        sendFlashlight(*this, false, flashlight.getBattery());
                    } else {
                        flashlight.setTime(g_global_vars->time + fl.getDrainTime());
                        sendFlashBattery(*this, flashlight.getBattery());
                    }
                }
            } else {
                if (flashlight.getBattery() < 100) {
                    flashlight.setBattery(flashlight.getBattery() + 1);
                    flashlight.setTime(g_global_vars->time + fl.getChargeTime());
                    sendFlashBattery(*this, flashlight.getBattery());
                } else {
                    flashlight.setTime(0);
                }
            }
        }
        if (!flashlight.getNextDynamicUpdate() || flashlight.getNextDynamicUpdate() > g_global_vars->time) {
            return;
        }
        flashlight.setNextDynamicUpdate(g_global_vars->time + 0.1f);
        TraceResult trace;
        MakeVectors(getViewAngle());
        const auto end = getGunPosition() + g_global_vars->vec_forward * static_cast<float>(fl.getDistance());
        TraceLine(getGunPosition(), end, TR_IGNORE_NONE, *this, &trace);
        if (trace.fraction == 1.f) {
            return;
        }
        SendNetMessage(*this, SvcMessage::TempEntity, [&] {
            TE_DynamicLight(trace.end_position, fl.getSize(), fl.getColor(), 3, 0);
        });
    }

    auto Player::SwitchFlashlight(bool isEnabled) -> void {
        auto& flashlight = Flashlight();
        const auto& flashlightRef = Flashlights[flashlight.getId()];
        if (!flashlightRef) {
            return;
        }
        const auto& fl = flashlightRef->get();
        if (isEnabled) {
            flashlight.setEnabled(true);
            flashlight.setTime(g_global_vars->time + fl.getDrainTime());
            sendFlashlight(*this, true, flashlight.getBattery());
            if (fl.getSize()) {
                flashlight.setNextDynamicUpdate(1.f);
            }
        } else {
            flashlight.setEnabled(false);
            flashlight.setTime(g_global_vars->time + fl.getChargeTime());
            flashlight.setNextDynamicUpdate(0);
            sendFlashlight(*this, false, flashlight.getBattery());
        }
    }
}
