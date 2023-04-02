#include "common/util.h"
#include <string_view>
#include <array>

namespace common {

    auto getTeamName(Team team) -> const std::string_view {
        static constexpr std::array<const char*, static_cast<size_t>(MAX_TEAMS)> TEAM_NAMES = {
            "UNASSIGNED",
            "TERRORIST",
            "CT",
            "SPECTATOR",
        };
        return TEAM_NAMES[toInt(team)];
    }
}
