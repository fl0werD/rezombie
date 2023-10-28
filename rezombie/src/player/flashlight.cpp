#include "rezombie/player/player.h"
#include "rezombie/player/modules/flashlight.h"
#include "rezombie/messages/user_message.h"
#include "rezombie/messages/temporary_entity.h"

namespace rz
{
    auto Player::FlashlightUpdate() -> void {
        const auto& flashlightRef = Flashlights[getFlashlight().getId()];
        if (flashlightRef) {
            setHideHud(getHideHud() & ~HIDE_HUD_FLASHLIGHT);
        } else {
            setHideHud(getHideHud() | HIDE_HUD_FLASHLIGHT);
            return;
        }
        const auto& flashlight = flashlightRef->get();
        auto& playerFlashlight = getFlashlight();
        if (playerFlashlight.getTime() <= g_global_vars->time) {
            if (playerFlashlight.isEnabled()) {
                if (playerFlashlight.getBattery()) {
                    playerFlashlight.setBattery(playerFlashlight.getBattery() - 1);
                    if (playerFlashlight.getBattery() <= 0) {
                        playerFlashlight.setEnabled(false);
                        playerFlashlight.setTime(g_global_vars->time + flashlight.getChargeTime());
                        playerFlashlight.setNextDynamicUpdate(0);
                        sendFlashlight(*this, false, playerFlashlight.getBattery());
                    } else {
                        playerFlashlight.setTime(g_global_vars->time + flashlight.getDrainTime());
                        sendFlashBattery(*this, playerFlashlight.getBattery());
                    }
                }
            } else {
                if (playerFlashlight.getBattery() < 100) {
                    playerFlashlight.setBattery(playerFlashlight.getBattery() + 1);
                    playerFlashlight.setTime(g_global_vars->time + flashlight.getChargeTime());
                    sendFlashBattery(*this, playerFlashlight.getBattery());
                } else {
                    playerFlashlight.setTime(0);
                }
            }
        }
        if (playerFlashlight.getNextDynamicUpdate() <= g_global_vars->time) {
            playerFlashlight.setNextDynamicUpdate(g_global_vars->time + 0.1f);
            TraceResult trace;
            MakeVectors(getViewAngle());
            auto end = getGunPosition() + g_global_vars->vec_forward * static_cast<float>(flashlight.getDistance());
            TraceLine(getGunPosition(), end, TR_IGNORE_MONSTERS, *this, &trace);
            if (trace.fraction != 1.f) {
                SendNetMessage(*this, SvcMessage::TempEntity, [&]() {
                    TE_DynamicLight(trace.end_position, flashlight.getSize(), flashlight.getColor(), 3, 0);
                });
            }
        }
    }

    auto Player::SwitchFlashlight(bool isEnabled) -> void {
        const auto& flashlightRef = Flashlights[getFlashlight().getId()];
        if (!flashlightRef) {
            return;
        }
        const auto& flashlight = flashlightRef->get();
        auto& playerFlashlight = getFlashlight();
        if (isEnabled) {
            playerFlashlight.setEnabled(true);
            playerFlashlight.setTime(g_global_vars->time + flashlight.getDrainTime());
            sendFlashlight(*this, true, playerFlashlight.getBattery());
            if (flashlight.getSize()) {
                playerFlashlight.setNextDynamicUpdate(1.f);
            }
        } else {
            playerFlashlight.setEnabled(false);
            playerFlashlight.setTime(g_global_vars->time + flashlight.getChargeTime());
            playerFlashlight.setNextDynamicUpdate(0);
            sendFlashlight(*this, false, playerFlashlight.getBattery());
        }
    }
}
