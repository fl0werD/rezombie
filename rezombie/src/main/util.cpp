#include "rezombie/main/util.h"
#include "rezombie/gamerules/consts.h"
#include "rezombie/messages/user_message.h"
#include "rezombie/player/players.h"
#include "rezombie/map/modules/fog.h"
#include "cssdk/dll/game_rules.h"
#include "core/strings.h"
#include <string>
#include <array>
#include <filesystem>

namespace rz
{
    using namespace cssdk;
    using namespace core;

    auto getTeamName(Team team) -> const std::string_view {
        static constexpr std::array<const char*, static_cast<size_t>(MAX_TEAMS)> TEAM_NAMES = {
            "UNASSIGNED",
            "TERRORIST",
            "CT",
            "SPECTATOR",
        };
        return TEAM_NAMES[toInt(team)];
    }

    auto UTIL_fromRangeToRange(
        float oldValue,
        std::pair<float, float> oldRange,
        std::pair<float, float> newRange
    ) -> float {
        const auto oldRangeDiff = oldRange.second - oldRange.first;
        const auto newRangeDiff = newRange.second - newRange.first;
        return (((oldValue - oldRange.first) * newRangeDiff) / oldRangeDiff) + newRange.first;
    }

    auto isPlayableTeam(Team team) -> bool {
        return (team == Team::Human || team == Team::Zombie);
    }

    auto getPlayersCount(PlayersCountFlags flags) -> int {
        int playersCount = 0;
        Players.forEachConnected(
            [&](const auto& player) {
                if (toInt(flags & PlayersCountFlags::Alive) && !player.isAlive()) {
                    return;
                }
                if (toInt(flags & PlayersCountFlags::Dead) && player.isAlive()) {
                    return;
                }
                const auto team = player.getTeam();
                if (toInt(flags & PlayersCountFlags::Humans) && toInt(flags & PlayersCountFlags::Zombies)) {
                    if (team != Team::Human && team != Team::Zombie) {
                        return;
                    }
                } else if (toInt(flags & PlayersCountFlags::Humans) && team != Team::Human) {
                    return;
                } else if (toInt(flags & PlayersCountFlags::Zombies) && team != Team::Zombie) {
                    return;
                }
                ++playersCount;
            }
        );
        return playersCount;
    }

    auto CellToString(const cell* array, int size) -> std::string {
        std::string str{};
        str.reserve(size);
        for (int i = 0; i < size && array[i] != 0; ++i) {
            str.push_back(static_cast<std::string::value_type>(array[i]));
        }
        return str;
    }

    auto isFileExists(const std::string_view filePath) -> bool {
        std::filesystem::path path{str::BuildPathName(filePath.data())};
        path = (std::filesystem::current_path() /= path);
        return std::filesystem::exists(path);
    }

    auto precacheSkyByName(const std::string_view name) -> void {
        static constexpr std::array<std::string_view, 6> SKY_SIDES = {"bk", "dn", "ft", "lf", "rt", "up"};
        for (auto& side: SKY_SIDES) {
            auto skySideFilePath = str::Format("gfx/env/%s%s.tga", name, side);
            if (!isFileExists(skySideFilePath)) {
                continue;
            }
            PrecacheGeneric(skySideFilePath.c_str());
        }
    }

    auto UTIL_EmitSound(
        Edict* entity,
        SoundChannel channel,
        const std::string& sample,
        float volume,
        float attenuation,
        int flags,
        int pitch
    ) -> void {
        EmitSound(entity, channel, sample.c_str(), volume, attenuation, flags, pitch);
    }

    auto UTIL_SendMotd(Edict* receiver, const std::string& text) -> void {
        auto length = text.length();
        if (length > MAX_MOTD_LENGTH) {
            length = MAX_MOTD_LENGTH;
        }
        bool more;
        auto sent = 0;
        do {
            constexpr auto chunkSize = MAX_MOTD_CHUNK;
            const auto chunk = text.substr(sent, chunkSize);
            sent += chunkSize;
            more = sent < length;
            sendMotd(receiver, !more, chunk.c_str());
        } while (more);
    }

    auto UTIL_SendShowMenu(Edict* receiver, int keys, int time, const std::string& text) -> void {
        auto more = false;
        auto sent = 0;
        do {
            constexpr auto chunkSize = 172;
            const auto chunk = std::string{text.substr(sent, chunkSize)};
            sent += chunkSize;
            more = sent < static_cast<int>(text.length());
            sendShowMenu(receiver, keys, time, more, chunk.c_str());
        } while (more);
    }

    auto FindEntity(
        EntityBase* startEntity,
        FindEntityType findEntityType,
        std::string_view className
    ) -> EntityBase* {
        std::string_view findType;
        switch (findEntityType) {
            case FindEntityType::ClassName: {
                findType = "classname";
                break;
            }
            case FindEntityType::TargetName: {
                findType = "targetname";
                break;
            }
        }
        return regamedll_api::Funcs()->find_entity_by_string(startEntity, findType.data(), className.data());
    }

    auto ForEachEntity(
        FindEntityType findEntityType,
        std::string_view className,
        const std::function<void(EntityBase*)>& action
    ) -> void {
        EntityBase* entity = nullptr;
        while ((entity = FindEntity(entity, findEntityType, className.data()))) {
            if (!IsValidEntity(entity)) {
                break;
            }
            action(entity);
        }
    }

    auto UTIL_CreateNamedEntity(Strind allocatedClassName) -> Edict* {
        return regamedll_api::Funcs()->create_named_entity2(allocatedClassName);
    }
}
