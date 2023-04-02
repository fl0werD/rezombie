#pragma once

#include <string>

namespace common {

    template<typename E>
    constexpr auto toInt(E e) noexcept {
        return static_cast<std::underlying_type_t<E>>(e);
    }

    enum class Team : int
    {
        Unassigned,
        Zombie,
        Human,
        Spectator,
    };

    constexpr auto MAX_TEAMS = toInt(Team::Spectator) + 1;

    auto getTeamName(Team team) -> const std::string_view;
}
