#include "rezombie/map/api/map_cameras.h"
#include "rezombie/map/modules/map_cameras.h"
#include "rezombie/player/players.h"
#include <amxx/api.h>
#include <unordered_map>

namespace rz
{
    using namespace amx;
    using namespace amxx;

    enum class MapCamerasVars : int {
        Camera,
        NextCameraTime,
    };

    const std::unordered_map<std::string, MapCamerasVars> MapCamerasVarsMap = {
        {"camera",           MapCamerasVars::Camera},
        {"next_camera_time", MapCamerasVars::NextCameraTime},
    };

    auto HandleMapCamerasVar(Amx* amx, cell* params, bool isGetter) -> cell {
        enum {
            arg_count,
            arg_player,
            arg_var,
            arg_3,
        };

        using vars = MapCamerasVars;

        auto& player = Players[params[arg_player]];
        const auto key = GetAmxString(amx, params[arg_var]);
        const auto& var = getMapValue(MapCamerasVarsMap, key);
        if (!var) {
            // Invalid index
            return false;
        }
        auto& camera = player.getMapCameraVars();
        switch (*var) {
            case vars::Camera: {
                if (isGetter) {
                    return camera.getCamera();
                } else {
                    camera.setCamera(*Address(amx, params[arg_3]));
                }
                break;
            }
            case vars::NextCameraTime: {
                if (isGetter) {
                    return FloatToCell(camera.getNextCameraTime());
                } else {
                    camera.setNextCameraTime(CellToFloat(*Address(amx, params[arg_3])));
                }
                break;
            }
        }
        return true;
    }

    auto get_map_camera_var(Amx* amx, cell* params) -> cell {
        return HandleMapCamerasVar(amx, params, true);
    }

    auto set_map_camera_var(Amx* amx, cell* params) -> cell {
        return HandleMapCamerasVar(amx, params, false);
    }

    auto map_cameras_count(Amx*, cell*) -> cell {
        return MapCameras.count();
    }

    auto map_cameras_begin(Amx*, cell*) -> cell {
        return MapCameras.begin();
    }

    auto map_cameras_end(Amx*, cell*) -> cell {
        return MapCameras.end();
    }

    auto AmxxMapCameras::registerNatives() const -> void {
        static AmxNativeInfo natives[] = {
            {"get_map_camera_var", get_map_camera_var},
            {"set_map_camera_var", set_map_camera_var},
            {"map_cameras_count",  map_cameras_count},
            {"map_cameras_begin",  map_cameras_begin},
            {"map_cameras_end",    map_cameras_end},

            {nullptr,              nullptr},
        };
        AddNatives(natives);
    }
}
