#include "rezombie/messages/user_message.h"
#include <metamod/utils.h>
#include <array>
#include <functional>

namespace rz
{
    using namespace cssdk;
    using namespace metamod::utils;

    constexpr std::array<std::string_view, MAX_USER_MESSAGE> USER_MESSAGE_NAMES = {
        "VoiceMask", "ReqState", "CurWeapon", "Geiger", "Flashlight", "FlashBat", "Health", "Damage",
        "Battery", "Train", "HudTextPro", "HudText", "SayText", "TextMsg", "WeaponList", "ResetHUD",
        "InitHUD", "ViewMode", "GameTitle", "DeathMsg", "ScoreAttrib", "ScoreInfo", "TeamInfo", "TeamScore",
        "Mode", "MOTD", "ServerName", "AmmoPickup", "WeapPickup", "ItemPickup", "HideWeapon", "SetFOV",
        "ShowMenu", "ScreenShake", "ScreenFade", "AmmoX", "SendAudio", "RoundTime", "Money", "ArmorType",
        "BlinkAcct", "StatusValue", "StatusText", "StatusIcon", "BarTime", "ReloadSound", "Crosshair", "NVGToggle",
        "Radar", "Spectator", "VGUIMenu", "TutorText", "TutorLine", "TutorState", "TutorClose", "AllowSpec",
        "BombDrop", "BombPickup", "ClCorpse", "HostagePos", "HostageK", "HLTV", "SpecHealth", "ForceCam",
        "ADStop", "ReceiveW", "CZCareer", "CZCareerHUD", "ShadowIdx", "TaskTime", "Scenario", "BotVoice",
        "BuyClose", "SpecHealth2", "BarTime2", "ItemStatus", "Location", "BotProgress", "Brass", "Fog",
        "ShowTimer", "HudTextArgs", "Account", "HealthInfo",
    };

    auto initUserMessagesIds() -> void {
        for (int message = 0; message < MAX_USER_MESSAGE; ++message) {
            userMessages[message] = GetUserMsgId(USER_MESSAGE_NAMES[message].data());
        }
    }

    auto SendUserMessage(Edict* receiver, UserMessage message, const std::function<void()>& block) -> void {
        if (!userMessages[toInt(message)]) {
            return;
        }
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
