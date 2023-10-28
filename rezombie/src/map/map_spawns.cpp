#include "rezombie/map/map_spawns.h"
#include "rezombie/main/util.h"

namespace rz
{
    using namespace cssdk;

    const std::vector<std::string_view> SPAWN_CLASSNAMES{
        "info_player_start",
        "info_player_deathmatch",
    };

    auto MapSpawns::collect() -> void {
        for (auto& className: SPAWN_CLASSNAMES) {
            ForEachEntity(FindEntityType::ClassName, className, [this](const auto spawn) {
                add(spawn->vars->origin, spawn->vars->angles);
            });
        }
    }
}
