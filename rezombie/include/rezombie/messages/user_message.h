#pragma once

#include "rezombie/messages/weather_type.h"
#include "rezombie/colors/colors.h"
#include "rezombie/main/util.h"
#include "rezombie/map/modules/fog.h"
#include <cssdk/dll/player.h>
#include <metamod/engine.h>
#include <functional>

namespace rz
{
    using namespace cssdk;
    using namespace metamod::engine;

    enum class UserMessage : int {
        VoiceMask,
        ReqState,
        CurWeapon,
        Geiger,
        Flashlight,
        FlashBat,
        Health,
        Damage,
        Battery,
        Train,
        HudTextPro,
        HudText,
        SayText,
        TextMsg,
        WeaponList,
        ResetHUD,
        InitHUD,
        ViewMode,
        GameTitle,
        DeathMsg,
        ScoreAttrib,
        ScoreInfo,
        TeamInfo,
        TeamScore,
        GameMode,
        MOTD,
        ServerName,
        AmmoPickup,
        WeapPickup,
        ItemPickup,
        HideWeapon,
        SetFOV,
        ShowMenu,
        ScreenShake,
        ScreenFade,
        AmmoX,
        SendAudio,
        RoundTime,
        Money,
        ArmorType,
        BlinkAcct,
        StatusValue,
        StatusText,
        StatusIcon,
        BarTime,
        ReloadSound,
        Crosshair,
        NVGToggle,
        Radar,
        Spectator,
        VGUIMenu,
        TutorText,
        TutorLine,
        TutorState,
        TutorClose,
        AllowSpec,
        BombDrop,
        BombPickup,
        ClCorpse,
        HostagePos,
        HostageK,
        HLTV,
        SpecHealth,
        ForceCam,
        ADStop,
        ReceiveW,
        CZCareer,
        CZCareerHUD,
        ShadowIdx,
        TaskTime,
        Scenario,
        BotVoice,
        BuyClose,
        SpecHealth2,
        BarTime2,
        ItemStatus,
        Location,
        BotProgress,
        Brass,
        Fog,
        ShowTimer,
        HudTextArgs,
        Account,
        HealthInfo,

        endIndex = HealthInfo,
    };

    constexpr auto MAX_USER_MESSAGE = toInt(UserMessage::endIndex) + 1;
    constexpr auto TO_ALL = nullptr;

    inline int userMessages[MAX_USER_MESSAGE]{};

    auto initUserMessagesIds() -> void;

    auto SendUserMessage(Edict* receiver, UserMessage message, const std::function<void()>& block = nullptr) -> void;

    inline auto sendCurWeapon(Edict* receiver, int isActive, WeaponId weaponId, int clip) -> void {
        SendUserMessage(receiver, UserMessage::CurWeapon, [&]() {
            WriteByte(isActive);
            WriteByte(toInt(weaponId));
            WriteByte(clip);
        });
    }

    inline auto sendFlashlight(Edict* receiver, bool isEnabled, int battery) -> void {
        SendUserMessage(receiver, UserMessage::Flashlight, [&]() {
            WriteByte(isEnabled);
            WriteByte(battery);
        });
    }

    inline auto sendFlashBattery(Edict* receiver, int battery) -> void {
        SendUserMessage(receiver, UserMessage::FlashBat, [&]() {
            WriteByte(battery);
        });
    }

    inline auto sendTextMsg(
        Edict* receiver,
        HudPrint printType,
        const char* text,
        const char* param1 = nullptr,
        const char* param2 = nullptr,
        const char* param3 = nullptr,
        const char* param4 = nullptr
    ) -> void {
        SendUserMessage(receiver, UserMessage::TextMsg, [&]() {
            WriteByte(static_cast<int>(printType));
            WriteString(text);
            if (param1) {
                WriteString(param1);
            }
            if (param2) {
                WriteString(param2);
            }
            if (param3) {
                WriteString(param3);
            }
            if (param4) {
                WriteString(param4);
            }
        });
    }

    inline auto sendWeaponList(
        Edict* receiver,
        const char* name,
        int ammoIndex,
        int maxAmmo,
        int ammo2Index,
        int maxAmmo2,
        int slot,
        int position,
        WeaponId weaponId,
        int flags
    ) -> void {
        SendUserMessage(receiver, UserMessage::WeaponList, [&]() {
            WriteString(name);
            WriteByte(ammoIndex);
            WriteByte(maxAmmo);
            WriteByte(ammo2Index);
            WriteByte(maxAmmo2);
            WriteByte(slot);
            WriteByte(position);
            WriteByte(toInt(weaponId));
            WriteByte(flags);
        });
    }

    inline auto sendViewMode(Edict* receiver) -> void {
        SendUserMessage(receiver, UserMessage::ViewMode);
    }

    inline auto sendDeathMsg(
        Edict* receiver,
        int attacker,
        int victim,
        std::string_view weaponName,
        bool isHeadshot = false
    ) -> void {
        SendUserMessage(receiver, UserMessage::DeathMsg, [&]() {
            WriteByte(attacker);
            WriteByte(victim);
            WriteByte(isHeadshot);
            WriteString(weaponName.data());
        });
    }

    inline auto sendScoreAttrib(Edict* receiver, int player, byte attribFlags) -> void {
        SendUserMessage(receiver, UserMessage::ScoreAttrib, [&]() {
            WriteByte(player);
            WriteByte(attribFlags);
        });
    }

    inline auto sendScoreInfo(Edict* receiver, int player, int frags, int deaths, Team team, int classId = 0) -> void {
        SendUserMessage(receiver, UserMessage::ScoreInfo, [&]() {
            WriteByte(player);
            WriteShort(frags);
            WriteShort(deaths);
            WriteShort(classId);
            WriteShort(toInt(team));
        });
    }

    inline auto sendTeamInfo(Edict* receiver, int player, Team team) -> void {
        SendUserMessage(receiver, UserMessage::TeamInfo, [&]() {
            WriteByte(player);
            WriteString(getTeamName(team).data());
        });
    }

    inline auto sendTeamScore(Edict* receiver, Team team, short score) -> void {
        SendUserMessage(receiver, UserMessage::TeamScore, [&]() {
            WriteString(getTeamName(team).data());
            WriteShort(score);
        });
    }

    inline auto sendGameMode(Edict* receiver, byte gameMode) -> void {
        SendUserMessage(receiver, UserMessage::GameMode, [&]() {
            WriteByte(gameMode);
        });
    }

    inline auto sendMotd(Edict* receiver, byte flag, const char* text) -> void {
        SendUserMessage(receiver, UserMessage::MOTD, [&]() {
            WriteByte(flag);
            WriteString(text);
        });
    }

    inline auto sendServerName(Edict* receiver, const char* serverName) -> void {
        SendUserMessage(receiver, UserMessage::ServerName, [&]() {
            WriteString(serverName);
        });
    }

    inline auto sendAmmoPickup(Edict* receiver, int ammoId, int amountAdd) -> void {
        SendUserMessage(receiver, UserMessage::AmmoPickup, [&]() {
            WriteByte(ammoId);
            WriteByte(amountAdd);
        });
    }

    inline auto sendWeapPickup(Edict* receiver, WeaponId weaponId) -> void {
        SendUserMessage(receiver, UserMessage::WeapPickup, [&]() {
            WriteByte(toInt(weaponId));
        });
    }

    inline auto sendItemPickup(Edict* receiver, const char* className) -> void {
        SendUserMessage(receiver, UserMessage::ItemPickup, [&]() {
            WriteString(className);
        });
    }

    inline auto sendHideWeapon(Edict* receiver, int hideHudFlags) -> void {
        SendUserMessage(receiver, UserMessage::HideWeapon, [&]() {
            WriteByte(hideHudFlags);
        });
    }

    inline auto sendShowMenu(Edict* receiver, int keys, int time, bool more, const char* text) -> void {
        SendUserMessage(receiver, UserMessage::ShowMenu, [&]() {
            WriteShort(keys);
            WriteChar(time);
            WriteByte(more);
            WriteString(text);
        });
    }

    enum FadeFlags {
        FADE_IN = 0x0000,
        FADE_OUT = 0x0001,
        FADE_MODULATE = 0x0002,
        FADE_STAY_OUT = 0x0004,
    };

    inline auto sendScreenFade(
        Edict* receiver,
        short duration = 0,
        short holdTime = 0,
        short flags = 0,
        const std::string& color = "",
        byte alphaPercentage = 0
    ) -> void {
        SendUserMessage(receiver, UserMessage::ScreenFade, [&]() {
            WriteShort(duration);
            WriteShort(holdTime);
            WriteShort(flags);
            const auto colorRef = Colors.parse(color);
            if (colorRef) {
                const auto realColor = *colorRef;
                WriteByte(realColor.getRed());
                WriteByte(realColor.getGreen());
                WriteByte(realColor.getBlue());
                WriteByte(255 * alphaPercentage / 100);
            } else {
                WriteByte(0);
                WriteByte(0);
                WriteByte(0);
                WriteByte(0);
            }
        });
    }

    inline auto sendRoundTime(Edict* receiver, short time) -> void {
        SendUserMessage(receiver, UserMessage::RoundTime, [&]() {
            WriteShort(time);
        });
    }

    inline auto sendMoney(Edict* receiver, long amount, bool isTrackChange = true) -> void {
        SendUserMessage(receiver, UserMessage::Money, [&]() {
            WriteLong(amount);
            WriteByte(isTrackChange);
        });
    }

    inline auto sendArmorType(Edict* receiver, byte flag) -> void {
        SendUserMessage(receiver, UserMessage::ArmorType, [&]() {
            WriteByte(flag);
        });
    }

    inline auto sendStatusIcon(Edict* receiver, byte status, std::string sprite, std::string color) -> void {
        SendUserMessage(receiver, UserMessage::StatusIcon, [&]() {
            WriteByte(status);
            WriteString(sprite.data());
            if (status != 0) {
                const auto colorRef = Colors.parse(color);
                if (!colorRef) {
                    return;
                }
                const auto spriteColor = *colorRef;
                WriteByte(spriteColor.getRed());
                WriteByte(spriteColor.getGreen());
                WriteByte(spriteColor.getBlue());
            }
        });
    }

    inline auto sendRadar(Edict* receiver, int player, Vector origin) -> void {
        SendUserMessage(receiver, UserMessage::Radar, [&]() {
            WriteByte(player);
            WriteCoord(origin.x);
            WriteCoord(origin.y);
            WriteCoord(origin.z);
        });
    }

    inline auto sendForceCam(Edict* receiver, byte forceCamera, byte forceChaseCam, byte fadeToBlack) -> void {
        SendUserMessage(receiver, UserMessage::ForceCam, [&]() {
            WriteByte(forceCamera);
            WriteByte(forceChaseCam);
            WriteByte(fadeToBlack);
        });
    }

    inline auto sendReceiveW(Edict* receiver, WeatherType type) -> void {
        SendUserMessage(receiver, UserMessage::ReceiveW, [&]() {
            WriteByte(toInt(type));
        });
    }

    inline auto sendShadowIdx(Edict* receiver, long spriteIndex) -> void {
        SendUserMessage(receiver, UserMessage::ShadowIdx, [&]() {
            WriteLong(spriteIndex);
        });
    }

    inline auto sendFog(Edict* receiver, int fogId = 0) -> void {
        const auto fogRef = Fogs[fogId];
        float density = 0.f;
        if (fogRef) {
            auto& fog = fogRef->get();
            const auto colorRef = Colors.parse(fog.getColor());
            if (!colorRef) {
                return;
            }
            const auto fogColor = *colorRef;
            if (fog.getDensityPercentage() != 0) {
                density = UTIL_fromRangeToRange(
                    static_cast<float>(fog.getDensityPercentage()),
                    {0.f, 100.f},
                    {0.f, 0.01f}
                );
            }
            SendUserMessage(receiver, UserMessage::Fog, [&]() {
                WriteByte(fogColor.getRed());
                WriteByte(fogColor.getGreen());
                WriteByte(fogColor.getBlue());
                WriteLong(*(int*) &density);
            });
        } else {
            SendUserMessage(receiver, UserMessage::Fog, [&]() {
                WriteByte(0);
                WriteByte(0);
                WriteByte(0);
                WriteLong(*(int*) &density);
            });
        }
    }

    inline auto sendShowTimer(Edict* receiver) -> void {
        SendUserMessage(receiver, UserMessage::ShowTimer);
    }

    inline auto sendAccount(Edict* receiver, int player, long account) -> void {
        SendUserMessage(receiver, UserMessage::Account, [&]() {
            WriteByte(player);
            WriteLong(account);
        });
    }

    inline auto sendHealthInfo(Edict* receiver, int player, long health) -> void {
        SendUserMessage(receiver, UserMessage::HealthInfo, [&]() {
            WriteByte(player);
            WriteLong(health);
        });
    }
}
