#pragma once

#include "rezombie/gamerules/consts.h"
#include "rezombie/main/util.h"
#include <cssdk/public/regamedll.h>
#include <amxx/api.h>
#include <string>
#include <unordered_map>
#include <optional>
#include <functional>

namespace rz
{
    using namespace cssdk;
    using namespace amxx;

    template<typename E>
    constexpr auto toInt(E e) noexcept {
        return static_cast<std::underlying_type_t<E>>(e);
    }

    enum class Team : int {
        Unassigned,
        Zombie,
        Human,
        Spectator,
    };

    constexpr auto MAX_TEAMS = toInt(Team::Spectator) + 1;

    auto getTeamName(Team team) -> const std::string_view;

    auto UTIL_fromRangeToRange(
        float oldValue,
        std::pair<float, float> oldRange,
        std::pair<float, float> newRange
    ) -> float;

    enum class PlayersCountFlags {
        All = 0,
        Alive = (1 << 0),
        Dead = (1 << 1),
        Humans = (1 << 2),
        Zombies = (1 << 3),
    };

    constexpr PlayersCountFlags operator|(const PlayersCountFlags lhs, const PlayersCountFlags rhs) {
        return static_cast<PlayersCountFlags>(toInt(lhs) | toInt(rhs));
    }

    constexpr PlayersCountFlags operator&(const PlayersCountFlags lhs, const PlayersCountFlags rhs) {
        return static_cast<PlayersCountFlags>(toInt(lhs) & toInt(rhs));
    }

    constexpr auto toUChar(int value) -> unsigned char {
        return static_cast<unsigned char>(value);
    }

    enum class FindEntityType {
        ClassName,
        TargetName,
    };

    auto isPlayableTeam(Team team) -> bool;

    auto getPlayersCount(PlayersCountFlags flags = PlayersCountFlags::All) -> int;

    auto CellToString(const cell* array, int size) -> std::string;

    auto isFileExists(std::string_view filePath) -> bool;

    auto precacheSkyByName(std::string_view name) -> void;

    auto UTIL_EmitSound(
        Edict* entity,
        SoundChannel channel,
        const std::string& sample,
        float volume = VOL_NORM,
        float attenuation = ATTN_NORM,
        int flags = 0,
        int pitch = PITCH_NORM
    ) -> void;

    auto UTIL_SendMotd(Edict* receiver, const std::string& text) -> void;

    auto UTIL_SendShowMenu(Edict* receiver, int keys, int time, const std::string& text) -> void;

    auto FindEntity(EntityBase* startEntity, FindEntityType findEntityType, std::string_view className) -> EntityBase*;

    auto ForEachEntity(
        FindEntityType findEntityType,
        std::string_view className,
        const std::function<void(EntityBase*)>& action
    ) -> void;

    auto UTIL_CreateNamedEntity(Strind allocatedClassName) -> Edict*;

    template<typename Map>
    auto getMapValue(const Map& map, const typename Map::key_type& key) -> std::optional<typename Map::mapped_type> {
        auto it = map.find(key);
        if (it != map.end()) {
            return it->second;
        } else {
            return std::nullopt;
        }
    }
}
