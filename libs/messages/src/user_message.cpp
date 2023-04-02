#include "../include/messages/user_message.h"
#include <metamod/engine.h>
#include <metamod/utils.h>
#include <array>
#include <functional>
#include <common/util.h>

namespace message {
    using namespace cssdk;
    using namespace metamod::engine;
    using namespace metamod::utils;
    using namespace common;

    constexpr std::array<const char*, MAX_USER_MESSAGE> USER_MESSAGE_NAMES = {
        "VoiceMask", "ReqState", "CurWeapon", "Geiger", "Flashlight", "FlashBat", "Health", "Damage",
        "Battery", "Train", "HudTextPro", "HudText", "SayText", "TextMsg", "WeaponList", "ResetHUD",
        "InitHUD", "ViewMode", "GameTitle", "DeathMsg", "ScoreAttrib", "ScoreInfo", "TeamInfo", "TeamScore",
        "GameMode", "MOTD", "ServerName", "AmmoPickup", "WeapPickup", "ItemPickup", "HideWeapon", "SetFOV",
        "ShowMenu", "ScreenShake", "ScreenFade", "AmmoX", "SendAudio", "RoundTime", "Money", "ArmorType",
        "BlinkAcct", "StatusValue", "StatusText", "StatusIcon", "BarTime", "ReloadSound", "Crosshair", "NVGToggle",
        "Radar", "Spectator", "VGUIMenu", "TutorText", "TutorLine", "TutorState", "TutorClose", "AllowSpec",
        "BombDrop", "BombPickup", "ClCorpse", "HostagePos", "HostageK", "HLTV", "SpecHealth", "ForceCam",
        "ADStop", "ReceiveW", "CZCareer", "CZCareerHUD", "ShadowIdx", "TaskTime", "Scenario", "BotVoice",
        "BuyClose", "SpecHealth2", "BarTime2", "ItemStatus", "Location", "BotProgress", "Brass", "Fog",
        "ShowTimer", "HudTextArgs", "Account", "HealthInfo",
    };

    auto initMessagesId() -> void {
        for (int message = 0; message < MAX_USER_MESSAGE; ++message) {
            userMessages[message] = GetUserMsgId(USER_MESSAGE_NAMES[message]);
        }
    }

    auto SendUserMessage(Edict* receiver, UserMessage message, const std::function<void()>& block) -> void {
        if (receiver == TO_ALL) {
            MessageBegin(MessageType::All, userMessages[toInt(message)]);
        } else {
            MessageBegin(MessageType::One, userMessages[toInt(message)], nullptr, receiver);
        }
        if (block) {
            block();
        }
        MessageEnd();
    }
}
