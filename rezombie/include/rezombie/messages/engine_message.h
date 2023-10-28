#pragma once

#include "rezombie/messages/hud_message_params.h"
#include <core/strings/mutation.h>
#include <cssdk/dll/player.h>
#include <cssdk/public/utils.h>
#include <metamod/engine.h>
#include <functional>

namespace rz
{
    using namespace core;
    using namespace cssdk;
    using namespace metamod::engine;

    auto SendNetMessage(Edict* receiver, SvcMessage message, const std::function<void()>& block = nullptr) -> void;

    inline auto netIntermission(Edict* receiver) {
        SendNetMessage(receiver, SvcMessage::Intermission);
    }

    inline auto netLightStyle(Edict* receiver, int lightId, const std::string& lightInfo) {
        SendNetMessage(
            receiver, SvcMessage::LightStyle,
            [&]() {
                WriteByte(lightId);
                WriteString(lightInfo.c_str());
            });
    }

    inline auto netWeaponAnim(Edict* receiver, int animNumber, int body = 0) {
        SendNetMessage(
            receiver, SvcMessage::WeaponAnim,
            [&]() {
                WriteByte(animNumber);
                WriteByte(body);
            });
    }

    inline auto netHudMessage(Edict* receiver, const HudMessageParams& params, const std::string& text) {
        SendNetMessage(
            receiver,
            SvcMessage::TempEntity,
            [&]() {
                WriteByte(TE_TEXT_MESSAGE);
                WriteByte(params.channel_ & 0xFF);
                WriteShort(FixedSigned16(params.x_, 1 << 13));
                WriteShort(FixedSigned16(params.y_, 1 << 13));
                WriteByte(params.effect_);
                WriteByte(params.color1_.getRed());
                WriteByte(params.color1_.getGreen());
                WriteByte(params.color1_.getBlue());
                WriteByte(params.color1_.getAlpha());
                WriteByte(params.color2_.getRed());
                WriteByte(params.color2_.getGreen());
                WriteByte(params.color2_.getBlue());
                WriteByte(params.color2_.getAlpha());
                WriteShort(FixedUnsigned16(params.fadeInTime_, 1 << 8));
                WriteShort(FixedUnsigned16(params.fadeOutTime_, 1 << 8));
                WriteShort(FixedUnsigned16(params.holdTime_, 1 << 8));
                if (params.effect_ == 2) {
                    WriteShort(FixedUnsigned16(params.fxTime_, 1 << 8));
                }
                if (text.length() > 489) {
                    const auto textCopy = str::Utf8Truncate(text, 489);
                    WriteString(textCopy.c_str());
                } else {
                    WriteString(text.c_str());
                }
            });
    }

    inline auto netDHudMessage(Edict* receiver, const LargeHudMessageParams& params, const std::string& text) {
        SendNetMessage(
            receiver,
            SvcMessage::Director,
            [&]() {
                constexpr auto DRC_CMD_MESSAGE = 6;
                WriteByte((text.length() > 128 ? 128 : text.length()) + 31);
                WriteByte(DRC_CMD_MESSAGE);
                WriteByte(params.effect_);
                WriteLong(params.color_.getBlue() + (params.color_.getGreen() << 8) + (params.color_.getRed() << 16));
                WriteLong(*((int*) &params.x_));
                WriteLong(*((int*) &params.y_));
                WriteLong(*((int*) &params.fadeInTime_));
                WriteLong(*((int*) &params.fadeOutTime_));
                WriteLong(*((int*) &params.holdTime_));
                WriteLong(*((int*) &params.fxTime_));
                if (text.length() > 128) {
                    const auto textCopy = str::Utf8Truncate(text, 128);
                    WriteString(textCopy.c_str());
                } else {
                    WriteString(text.c_str());
                }
            });
    }
}
